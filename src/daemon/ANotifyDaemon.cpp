#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <string.h>
#include <sys/file.h>

#include "ANotifyDaemon.h"
#include "ANotify.h"
#include "ANotifyException.h"

const std::string ANotifyDaemon::propsPath = "daemon.prop";
const std::string ANotifyDaemon::logPath = "daemon.log";

ANotifyDaemon::ANotifyDaemon() throw (ANotifyException) :
  sock(-1), running(false), notify(NULL), watchPath("/")
{
  int port = -1, i, client, length;
  struct sockaddr_in addr;
  std::string str = "";
  pid_t current_pid = getpid(), pid;  

  this->propsFd = openPropsFile();
  
  if(this->propsFd == -1){
    throw ANotifyException("Unable to open or create daemon properties file", errno, this);
  }

  if(!isActiveDaemon(current_pid)){
    closePropsFile();
    throw ANotifyException("A daemon is already running");
  }

  this->sock = socket(AF_INET, SOCK_STREAM, 0);

  if(this->sock == -1){
    /* TODO: renvoyer le bon message d'exception */
    throw ANotifyException("Failed to open socket", errno, this);
  }

  addr.sin_family = AF_INET;  
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* Trouver un port de libre ou ecouter */
  for(i=5000; i<6000; i++){    
    addr.sin_port = htons(i);

    if(bind(this->sock, (struct sockaddr*)(&addr), sizeof(addr)) != -1
       && listen(this->sock, 0) != -1){
      port = i;
      break;
    }
  } 

  /* Si aucun port libre n'a ete trouve */
  if(port == -1){
    closePropsFile();
    close(this->sock);
    throw ANotifyException("No port available");
  }

  /*
    Contenu du fichier daemon.prop:

    pid:pid_du_daemon
    port:port_du_serveur
  */

  str.append("pid:");
  str += current_pid;
  str.append("\n");
  str.append("port:");
  str += port;

  if(write(this->propsFd, str.c_str(), str.length()) < 0){
    closePropsFile();
    close(this->sock);
    
    throw ANotifyException("Cannot write into properties file", errno, this);
  }

  closePropsFile();

  notify = new ANotify();
  pthread_mutex_init(&runningAccess, NULL);

  pid = fork();

  if(pid > 0){    
    std::cout << "daemon waiting for incoming connections" << std::endl;
    this->waitForClients(&addr);
  }
  else if(pid == 0){
    std::cout << "daemon started successfully" << std::endl;
    this->start();
  }
  else{
    /* TODO */
    throw ANotifyException("Fork failed", errno, this);
  }
}

ANotifyDaemon::~ANotifyDaemon(){
  this->stop();
  delete this->notify;
}

bool ANotifyDaemon::start(){
  return start(this->watchPath);
}

bool ANotifyDaemon::start(std::string& path){
  pid_t pid;

  if(this->isRunning()){
    return false;
  }  

  this->setRunning(true);

  /* TODO: lancer la surveillance sur Path */

  pid = fork();
  
  if(pid > 0){
    this->waitForEvents();
  }
  else if(pid == 0){
    this->run();
    exit(EXIT_SUCCESS);
  }
  else{
    /* TODO */
    return false;
  }

  return true;
}

void ANotifyDaemon::run(){
  ANotifyEvent pEvt;

  while(this->isRunning()){
    if(this->notify->getEvent(pEvt)){
      //traiter l'event
    }

    sleep(1);
  }
}

void ANotifyDaemon::waitForEvents(){
  while(this->isRunning()){
    try{
      this->notify->waitForEvents();
    }catch(ANotifyException e){
      //continue;
      break;
    }
  }
}

void ANotifyDaemon::waitForClients(struct sockaddr_in* addr){
  pid_t child_pid;
  int client;
  socklen_t len;
 
  /* Attente de la connexion d'un ou plusieurs clients */
  while(true){
    client = accept(this->sock, (struct sockaddr*)(addr), &len);
    
    /* Fermeture de la socket serveur */
    if(client == -1){
      break;
    }

    child_pid = fork();

    if(child_pid > 0){
      close(client);
    }
    else if(child_pid == 0){
      close(this->sock);
      communicate(client);
      //break;
      exit(EXIT_SUCCESS);
    }
    else{
      close(client);
      close(this->sock);
      throw ANotifyException("Fork failed", errno, this);
    }
  }
}

bool ANotifyDaemon::restart(){
  return restart(this->watchPath);
}

bool ANotifyDaemon::restart(std::string& path){
  bool startRes, stopRes;

  stopRes = this->stop();
  startRes = this->start(path);

  return startRes && stopRes;
}

bool ANotifyDaemon::stop(){
  if(!this->isRunning()){
    return false;
  }  

  /*if(notify != NULL){
    delete notify;
    notify = NULL;
    }*/

  this->setRunning(false);
  this->notify->AClose();

  return true;
}

bool ANotifyDaemon::kill(){
  /* TODO: voir quand il faut renvoyer false */
  stop();
  deletePropsFile();
  close(this->sock);
  pthread_mutex_destroy(&runningAccess);

  if(notify != NULL){
    delete notify;
  }

  return true;
}

void ANotifyDaemon::communicate(int client_socket){
  int readLength;
  char c;
  bool res = true;
  bool alive = true;

  while(alive){
    readLength = recv(client_socket, &c, 1, 0);

    if(readLength <= 0){
      /* TODO: voir ce qu'il faut faire dans ce cas la */
      return;
    }

    switch(c){
    case 'D'://delete (removes a watch) 
      bool rec;

      //Recursivite
      readLength = recv(client_socket, &c, 1, 0);
      if(readLength <= 0){
	return;
      }
      rec = (c == 0) ? false : true;      
      
      int length;

      //Taille du chemin
      readLength = recv(client_socket, &c, 1, 0);
      if(readLength <= 0){
	return;
      }
      length = (int)c;
      
      char path[length + 1];

      //Chemin
      readLength = recv(client_socket, path, length, 0);
      if(readLength <= 0){
	return;
      }
      if(readLength < length){
	res = false;
      }

      if(res){
	std::string pathStr(path);
	res = removeWatch(pathStr, rec);
      }
      break;

    case 'S'://start (starts the daemon) [A voir]
      res = this->start();
      break;

    case 'K'://kill (definitively kill the daemon) [A voir]
      res = this->kill();
      alive = false;
      break;

    case 'E'://end (stops the daemon)
      res = this->stop();
      break;

    case 'R'://restart (restarts the daemon)
      res = this->restart();
      break; 

    default:
      continue;
    }

    c = res ? 'Y' : 'N';
    send(client_socket, &c, 1, 0);
  }
}

int ANotifyDaemon::openPropsFile(){
  /* TODO: Modifier le chemin si besoin est + poser un verrou sur le fichier */
  int ret;  
  
  //this->propsFd = open(propsPath, O_RDWR | O_CREAT, 0777);  
  this->propsFd = open(propsPath.c_str(), O_RDWR | O_CREAT);

  if(this->propsFd == -1){
    return -1;
  }

  ret = flock(this->propsFd, LOCK_EX | LOCK_NB);

  if(ret == -1){
    return -1;
  }

  return this->propsFd;
}

int ANotifyDaemon::closePropsFile(){   
  /* TODO: Liberer le verrou sur le fichier de proprietes */
  flock(this->propsFd, LOCK_UN);
  return close(this->propsFd);  
}

int ANotifyDaemon::deletePropsFile(){
  return remove(propsPath.c_str());
}

bool ANotifyDaemon::isRunning(){
  bool res = false;

  pthread_mutex_lock(&runningAccess);

  res = this->running;

  pthread_mutex_unlock(&runningAccess);

  return res;
}

void ANotifyDaemon::setRunning(bool run){
  pthread_mutex_lock(&runningAccess);

  this->running = run;

  pthread_mutex_unlock(&runningAccess);
}

bool ANotifyDaemon::isActiveDaemon(pid_t pid){
  char buffer[50];
  int length; 
  int newlinePos;
  pid_t pid_read;

  if(this->propsFd == -1){
    return false;
  }

  length = read(this->propsFd, buffer, 50);

  if(length == 0){
    return true;
  }

  buffer[length] = '\0';

  if(length < 5
     || strncmp(buffer, "pid:", 4) != 0){
    return false;
  }

  std::string pidStr(&buffer[4]);

  newlinePos = pidStr.find('\n');
  
  if(newlinePos != std::string::npos){
    pidStr = pidStr.substr(0, newlinePos);
  }

  pid_read = (pid_t)(std::atoi(pidStr.c_str()));

  return (pid_read == pid);
}

bool ANotifyDaemon::removeWatch(std::string& path, bool rec){
  if(rec){
    return removeOneWatch(path);
  }
  else{
    return recRemoveWatch(path);
  }
}

bool ANotifyDaemon::removeOneWatch(std::string& path){
  return true;
}

bool ANotifyDaemon::recRemoveWatch(std::string& path){
  return true;
}
