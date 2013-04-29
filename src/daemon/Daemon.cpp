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

//#include "ANotifyDaemon.h"
#include "ANotifyDaemon.h"
#include "ANotify.h"
#include "ANotifyException.h"

const std::string ANotifyDaemon::propsPath = "daemon.prop";

ANotifyDaemon::ANotifyDaemon() throw (ANotifyException) {
  int fd, port = -1, i, client, length;
  struct sockaddr_in addr;
  std::string str = "";
  pid_t current_pid = getpid(), pid;  
  
  this->sock = -1;
  this->running = false;
  this->notify = NULL;

  fd = openPropsFile();
  
  if(fd == -1){
    throw ANotifyException("Unable to open or create daemon properties file", errno, this);
  }

  if(!isActiveDaemon(fd, current_pid)){
    closePropsFile(fd);
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
    closePropsFile(fd);
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

  if(write(fd, str.c_str(), str.length()) < 0){
    closePropsFile(fd);
    close(this->sock);
    
    throw ANotifyException("Cannot write into properties file", errno, this);
  }

  closePropsFile(fd);

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
  }
}

ANotifyDaemon::~ANotifyDaemon(){}

bool ANotifyDaemon::start(){
  pid_t pid;

  if(this->running){
    return false;
  }  

  this->running = true;

  pid = fork();
  
  if(pid > 0){
    
  }
  else if(pid == 0){
    this->run();
    exit(EXIT_SUCCESS);
  }
  else{
    /* TODO */
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
  this->stop();
  return this->start();
}

bool ANotifyDaemon::stop(){
  if(!this->isRunning()){
    return false;
  }  

  /*if(notify != NULL){
    delete notify;
    notify = NULL;
    }*/
  notify->AClose();

  this->setRunning(false);
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
  int ret, fd;  
  
  //fd = open(propsPath, O_RDWR | O_CREAT, 0777);  
  fd = open(propsPath.c_str(), O_RDWR | O_CREAT);

  if(fd == -1){
    return -1;
  }

  ret = flock(fd, LOCK_EX | LOCK_NB);

  if(ret == -1){
    return -1;
  }

  return fd;
}

int ANotifyDaemon::closePropsFile(int fd){
  close(fd);  
  /* TODO: Liberer le verrou sur le fichier de proprietes */
  return flock(fd, LOCK_UN);
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

bool ANotifyDaemon::isActiveDaemon(int props_fd, pid_t pid){
  char buffer[50];
  int length; 
  int newlinePos;
  pid_t pid_read;

  length = read(props_fd, buffer, 50);

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
