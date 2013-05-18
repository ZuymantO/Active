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
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>

#include "ANotifyDaemon.h"
#include "ANotify.h"
#include "ANotifyException.h"

const std::string ANotifyDaemon::propsPath = "daemon.prop";
const std::string ANotifyDaemon::logPath = "daemon.log";

ANotifyDaemon::ANotifyDaemon() throw (ANotifyException) :
  sock(-1), running(false), notify(NULL), 
  /* TODO: decommenter pour la partie finale */
  watchPath("/")
  //watchPath("/home/cuisse/")
{
  
}

ANotifyDaemon::~ANotifyDaemon(){
  this->kill();
}

void ANotifyDaemon::initDaemon() throw (ANotifyException){
  int port = -1, i, client, length, res;
  struct sockaddr_in addr;
  std::string str = "";
  pid_t current_pid = getpid(), pid; 
  pthread_t init_thread;
  
  this->propsFd = openPropsFile();
  
  if(this->propsFd == -1){
    throw ANotifyException("Unable to open or create daemon properties file", errno, this);
  }
  
  if(!isActiveDaemon(current_pid)){
    closePropsFile(this->propsFd);
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
    closePropsFile(this->propsFd);
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
    closePropsFile(this->propsFd);
    close(this->sock);
    
    throw ANotifyException("Cannot write into properties file", errno, this);
  }
  
  closePropsFile(this->propsFd);
  
  this->logFd = openLogFile();
  
  if(this->logFd == -1){
    closePropsFile(this->propsFd);
    close(this->sock);
  }
  
  this->addr = &addr;
  
  this->notify = new ANotify();
  this->mask = ANOTIFY_RENAME | ANOTIFY_WRITE | ANOTIFY_DELETE | ANOTIFY_ATTRIBUT;
  this->notifyEvent = new ANotifyEvent(&this->sharedEvent, this->mask);
  
  pthread_mutex_init(&this->runningAccess, NULL);
  pthread_mutex_init(&this->logLock, NULL);  

  initWithThread(this);
}

int ANotifyDaemon::initWithThread(ANotifyDaemon* dae) throw (ANotifyException){
  int res;
  pthread_t init_thread;
  std::string str;
  
  res = pthread_create(&init_thread, NULL, init, (void*)dae);
  
  /* TODO: verifier le -1 en cas d'echec */
  if(res != 0){    
    close(dae->sock);
    closePropsFile(dae->propsFd);
    dae->closeLogFile();
    throw ANotifyException("thread failed", errno, dae);
  }

  str = "[SUCCESS] initWithThread " + res;
  dae->printLog(str);
  
  return res;
}

void* ANotifyDaemon::init(void* arg){
  std::string str;
  pthread_t thread;
  int res;
  ANotifyDaemon* dae = (ANotifyDaemon*)arg;  
  
  str = "[SUCCESS] init";
  dae->printLog(str);

  /* TODO: que faire quand ca foire ? */
  res = pthread_create(&thread, NULL, waitForClients, arg);
  if(res == 0){
    str = "daemon waiting for incoming connections ...";
    dae->printLog(str);
  }
  else{
    str = "failed to wait for incoming connections";
    dae->printLog(str);
    pthread_exit(NULL);
  }
  /*res = pthread_create(&threads[1], NULL, start, arg);
  if(res == 0){    
    str = "daemon started successfully";
    dae->printLog(str);
    }*/

  dae->start();
  str = "daemon started successfully";
  dae->printLog(str);
  
  pthread_exit(NULL);
}

bool ANotifyDaemon::retrieveBoolResult(void* (f)(void*), void* arg){
  pthread_t thread;
  bool* res;

  pthread_create(&thread, NULL, f, arg);
  pthread_join(thread, (void**)&res);

  return res;
}

bool ANotifyDaemon::start(){
  return start(this->watchPath);
}

bool ANotifyDaemon::start(std::string& path){
  std::pair<ANotifyDaemon*, std::string> arg;

  arg.first = this;
  arg.second = path;

  return retrieveBoolResult(startT, (void*)(&arg));
}

void* ANotifyDaemon::startT(void* arg){
  pid_t pid;
  bool res;
  int ret;
  std::string str;
  std::pair<ANotifyDaemon*, std::string>* daemon_path = (std::pair<ANotifyDaemon*, std::string>*)arg;
  ANotifyDaemon* dae = daemon_path->first;
  std::string path = daemon_path->second;
  pthread_t threads[2];
  
  if(dae->isRunning()){
    res = false;
    pthread_exit(&res);
  }  
  
  dae->setRunning(true);
  
  std::cout << "starting addWatch" << std::endl;

  /* TODO: lancer la surveillance sur Path */
  res = dae->addWatch(path);
  
  std::cout << "ended addWatch" << std::endl;

  if(!res){
    str = "[ERROR] failed to start daemon on path '" + path;
    std::cout << str << std::endl;
    dae->printLog(str);
    res = false;
    pthread_exit(&res);
  }

  std::cout << "addWatch succeeded" << std::endl;
  
  /* TODO: retirer les forks */
  
  ret = pthread_create(&threads[0], NULL, dae->waitForEvents, (void*)dae);
  if(ret != 0){
    dae->setRunning(false);
    res = false;
    pthread_exit(&res);
  }

  ret = pthread_create(&threads[1], NULL, dae->run, (void*)dae); 
  if(ret != 0){
    dae->setRunning(false);
    res = false;
    pthread_exit(&res);
  }
  
  res = true;
  pthread_exit(&res);
}

bool ANotifyDaemon::addWatch(std::string& path){
  struct stat st;
  DIR* dir = NULL;
  struct dirent* entry = NULL;
  std::string filepath, logMsg;
  
  if(stat(path.c_str(), &st) != 0){
    std::cout << "stat failed on '" + path + "'" << std::endl;
    return false;
  }
  
  if(S_ISDIR(st.st_mode)){
    std::string newpath;
    
    newpath = path;
    
    if(path[path.length() -1] != '/'){
      newpath += '/';
    }
    
    ANotifyWatch dirWatch(newpath, this->notifyEvent, true, true);
    
    dirWatch.setMonitor(this->notify);
    
    try{
      this->notify->add(dirWatch);
      logMsg = "[ADDED] " + newpath;
      //std::cout << logMsg << std::endl;
      printLog(logMsg);
    }catch(ANotifyException e){
      std::cout << e.GetMessage() << std::endl;
      //std::cout << "add failed on '" + path + "'" << std::endl;
      return false;
    }
    
    dir = opendir(newpath.c_str());
    
    if(dir == NULL){     
      //std::cout << "opendir failed on '" + path + "'" << std::endl;
      logMsg = "[ERROR] failed to open dir '" + newpath;
      printLog(logMsg);
      return false;
    }
    
    while((entry = readdir(dir)) != NULL){
      if(strcmp(entry->d_name, ".") == 0
	 || strcmp(entry->d_name, "..") == 0){
	continue;
      }
      
      /*filepath = path;
	
	if(path[path.length() - 1] != '/'){
	filepath += '/';
	}
	
	filepath += entry->d_name;*/
      
      //this->addWatch(filepath);
      filepath = newpath;
      filepath += entry->d_name;

      this->addWatch(filepath);
    }

    closedir(dir);
  }
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
  
#else
  else if(S_ISREG(st.st_mode)){
    ANotifyWatch fileWatch(path, this->notifyEvent, false, true);
    
    fileWatch.setMonitor(this->notify);

    try{
      this->notify->add(fileWatch);
    }catch(ANotifyException e){
      return false;
    }
  }
#endif  

  return true;
}

void* ANotifyDaemon::run(void* arg){
  ANotifyDaemon* dae = (ANotifyDaemon*)arg;
  ANotifyEvent pEvt;
  std::string str;

  str = "running mode";
  dae->printLog(str);

  while(dae->isRunning()){
    str = "is_running";
    dae->printLog(str);

    if(dae->notify->getEvent(pEvt)){
      //traiter l'event
      str = "[EVENT] " + pEvt.getName();
      dae->printLog(str);
      std::cout << "[EVENT]" + pEvt.getName() << std::endl;
    }

    sleep(1);
  }
}

void* ANotifyDaemon::waitForEvents(void* arg){
  ANotifyDaemon* dae = (ANotifyDaemon*)arg;

  while(dae->isRunning()){
    try{
      dae->notify->waitForEvents();
    }catch(ANotifyException e){
      /* TODO: Continuer ou s'arreter en cas d'erreur ? */
      //continue;
      break;
    }
  }
}

void* ANotifyDaemon::waitForClients(void* arg){
  pid_t child_pid;
  int client, res;
  socklen_t len;
  ANotifyDaemon* dae = (ANotifyDaemon*)arg;
 
  /* Attente de la connexion d'un ou plusieurs clients */
  while(true){
    client = accept(dae->sock, (struct sockaddr*)(dae->addr), &len);
    
    /* Fermeture de la socket serveur */
    if(client == -1){
      break;
    }
    
    pthread_t client_thread;
    std::pair<ANotifyDaemon*, int> infos_client;
    
    infos_client.first = dae;
    infos_client.second = client;
    
    res = pthread_create(&client_thread, NULL, communicate, (void*)(&infos_client));
    if(res != 0){
      close(client);
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

bool ANotifyDaemon::list(int client){
  int len;
  std::string path;
  int pathLength;
  WatchPathMap::iterator it;
  WatchPathMap paths;

  /* Obtenir un verrou sur la map */

  if(this->notify != NULL){
    paths = this->notify->getWatchesPathMap();
    it = paths.begin();

    while (it != paths.end()) {
      path = it->first;
      pathLength = path.length();

      //Envoi de la longueur du chemin
      len = send(client, &pathLength, sizeof(int), 0);

      if(len <= 0){
	/* Liberer le verrou sur la map */
	return false;
      }
 
      //Envoi du chemin
      len = send(client, path.c_str(), path.length(), 0);
      
      if(len <= 0){
	/* Liberer le verrou sur la map */
	return false;
      }
      
      it++;
    }
  }
  else{
    /* Liberer le verrou sur la map */
    return false;
  }

  /* Liberer le verrou sur la map*/
  return true;
}

bool ANotifyDaemon::stop(){
  if(!this->isRunning()){
    return false;
  }  

  this->setRunning(false);
  this->notify->AClose();

  return true;
}

bool ANotifyDaemon::kill(){
  /* TODO: voir quand il faut renvoyer false */
  this->stop();
  this->deletePropsFile();
  this->closeLogFile();
  
  close(this->sock);

  pthread_mutex_destroy(&this->runningAccess);
  pthread_mutex_destroy(&this->logLock);

  if(this->notifyEvent != NULL){
    delete this->notifyEvent;
    this->notifyEvent = NULL;
  }

  if(this->notify != NULL){
    delete this->notify;
    this->notify = NULL;
  }

  return true;
}

void* ANotifyDaemon::communicate(void* arg){
  int readLength, pathLength;
  char c;
  bool res = true, alive = true;
  std::pair<ANotifyDaemon*, int>* daemon_socketfd = (std::pair<ANotifyDaemon*, int>*)arg;
  ANotifyDaemon* dae = daemon_socketfd->first;
  int client_socket = daemon_socketfd->second;

  while(alive){
    readLength = recv(client_socket, &c, 1, 0);

    if(readLength <= 0){
      /* TODO: voir ce qu'il faut faire dans ce cas la */
      //return;
      pthread_exit(NULL);
    }

    switch(c){
    case 'D'://delete (removes a watch) 
      bool rec;

      //Recursivite
      readLength = recv(client_socket, &c, 1, 0);
      if(readLength <= 0){
	//return;
	pthread_exit(NULL);
      }
      rec = (c == 'Y') ? true : false;      
      
      int length;

      //Taille du chemin
      readLength = recv(client_socket, &pathLength, sizeof(int), 0);
      if(readLength <= 0){
	//return;
	pthread_exit(NULL);
      }
      
      char path[pathLength + 1];

      //Chemin
      readLength = recv(client_socket, path, pathLength, 0);
      if(readLength <= 0){
	//return;
	pthread_exit(NULL);
      }
      if(readLength < pathLength){
	res = false;
      }

      if(res){
	std::string pathStr(path);
	res = dae->removeWatch(pathStr, rec);
      }
      break;

    case 'S'://start (starts the daemon) [A voir]
      res = dae->start();
      break;

    case 'K'://kill (definitively kill the daemon) [A voir]
      res = dae->kill();
      alive = false;
      break;

    case 'E'://end (stops the daemon)
      res = dae->stop();
      break;

    case 'R'://restart (restarts the daemon)
      res = dae->restart();
      break; 

    case 'L':
      res = dae->list(client_socket);
      break;

    default:
      continue;
    }

    c = res ? 'Y' : 'N';
    send(client_socket, &c, 1, 0);
  }

  pthread_exit(NULL);
}

int ANotifyDaemon::openPropsFile(){
  /* TODO: Modifier le chemin si besoin est + poser un verrou sur le fichier */
  int ret, fd; 
  
  
  //this->propsFd = open(propsPath, O_RDWR | O_CREAT, 0777);  
  fd = open(propsPath.c_str(), O_RDWR | O_CREAT, 0666);

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
  /* TODO: Liberer le verrou sur le fichier de proprietes */
  flock(fd, LOCK_UN);
  return close(fd);  
}

int ANotifyDaemon::deletePropsFile(){
  return remove(propsPath.c_str());
}

int ANotifyDaemon::openLogFile(){
  this->logFd = open(logPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);

  /*if(fd == -1){
    return -1;
    }

    return fd;*/
  return this->logFd;
}

int ANotifyDaemon::closeLogFile(){
  int res;

  pthread_mutex_lock(&this->logLock);
  res = close(this->logFd);
  pthread_mutex_unlock(&this->logLock);

  return res;
}

void ANotifyDaemon::printLog(std::string& msg){
  if(this->logFd != -1){
    pthread_mutex_lock(&(this->logLock));
    write(this->logFd, msg.c_str(), msg.length());
    write(this->logFd, "\n", 1);
    pthread_mutex_unlock(&(this->logLock));
  }
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

int ANotifyDaemon::getDaemonPort(int fd){
  char buffer[50];
  int length, newlinePos, port;
  
  if(fd == -1){
    return -1;
  }

  length = read(fd, buffer, 50);

  if(length == 0){
    return true;
  }

  buffer[length] = '\0';

  if(length < 5
     || strncmp(buffer, "pid:", 4) != 0){
    return -1;
  }

  std::string pidStr(buffer), portStr = "";

  newlinePos = pidStr.find('\n');
  
  if(newlinePos != std::string::npos){
    portStr = pidStr.substr(newlinePos+1);
  }
  else{
    return -1;
  }

  length = read(fd, buffer, 50); 
  portStr = portStr + buffer;

  /* 
     Si la ligne ne commence par par "port:" ou si elle ne contient
     pas au minimum un caractere apres "port:"
  */
  if(portStr.length() < 6
     || strncmp(portStr.c_str(), "port:", 5) != 0){
    return -1;
  }

  portStr = portStr.substr(5);

  port = (int)(std::atoi(portStr.c_str()));

  return port;
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
  if(this->notify == NULL){
    return false;
  }

  ANotifyWatch* watch = this->notify->findWatch(path);

  if(watch == NULL){
    return false;
  } 

  return this->notify->remove(watch);
}

bool ANotifyDaemon::recRemoveWatch(std::string& path){
  bool res = true;
  WatchPathMap::iterator it;
  WatchPathMap paths;
  std::string filepath;
  int filepathLength;

  /* Obtenir un verrou sur la map */

  if(this->notify != NULL){
    paths = this->notify->getWatchesPathMap();
    it = paths.begin();

    while (it != paths.end()) {
      filepath = it->first;
      filepathLength = filepath.length();

      /* Trouver la bonne fonction */
      //if(filepath.startsWith(path)){
      if(strncmp(filepath.c_str(), path.c_str(), path.length()) == 0){
	paths.erase(path);
      }
      
      it++;
    }
  }
  else{
    /* Liberer le verrou sur la map */
    return false;
  }

  /* Liberer le verrou sur la map */

  return true;
}
