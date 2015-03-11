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
#include <sstream>


#include "ANotifyDaemon.h"
#include "ANotify.h"
#include "ANotifyException.h"
#include "../common/XMLGeneration.h"

using namespace acommon;

const std::string ANotifyDaemon::propsPath = "daemon.prop";
const std::string ANotifyDaemon::logPath = "daemon.log";

ANotifyDaemon::ANotifyDaemon() throw (ANotifyException) :
  sock(-1), xml_sock(-1), propsFd(-1), logFd(-1), running(false), notify(NULL), notifyEvent(NULL),
  /* TODO: decommenter pour la partie finale */
  watchPath("/home/")
{
  this->alive = true;
  sleep(1);
  initDaemon();
}

ANotifyDaemon::~ANotifyDaemon(){
  this->kill();
}

void ANotifyDaemon::initDaemon() throw (ANotifyException){
  int port = -1, i, client, length, res;
  //struct sockaddr_in addr;
  std::string str = "";
  std::stringstream ss(str);
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
  
  this->addr.sin_family = AF_INET;  
  this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  /* Trouver un port de libre ou ecouter */
  for(i=5000; i<6000; i++){    
    this->addr.sin_port = htons(i);
    
    if(bind(this->sock, (struct sockaddr*)(&this->addr), sizeof(this->addr)) != -1
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
  
  ss << "pid:" << current_pid << "\n" << "port:" << port;
  str = ss.str();
  
  if(write(this->propsFd, str.c_str(), str.length()) < 0){
    closePropsFile(this->propsFd);
    close(this->sock);
    
    throw ANotifyException("Cannot write into properties file", errno, this);
  }
  
  closePropsFile(this->propsFd);

  this->xml_sock = socket(AF_INET, SOCK_STREAM, 0);
  
  if(this->xml_sock == -1){
    deletePropsFile();
    close(this->sock);

    throw ANotifyException("Failed to open xml_socket", errno, this);
  }

  struct hostent* host = gethostbyname("localhost");
    
  if(host == NULL){
    deletePropsFile();
    close(this->sock);
    close(this->xml_sock);

    throw ANotifyException("No such host: localhost", errno, this);
  }
  
  memcpy(&(this->xml_addr.sin_addr.s_addr), host->h_addr_list[0], host->h_length);
  
  port = -1;

  for(i=40000; i<=40002; i++){
    this->xml_addr.sin_family = AF_INET;
    this->xml_addr.sin_port = htons(port);
    
    if(connect(this->xml_sock, (struct sockaddr*)(&this->xml_addr), sizeof(this->xml_addr)) != -1){
      port = i;
      break;
    }
  }

  if(port == -1){
    deletePropsFile();
    close(this->sock);
    close(this->xml_sock);
    
    throw ANotifyException("Connection failed", errno, this);
  }
  
  this->logFd = openLogFile();
  
  if(this->logFd == -1){
    deletePropsFile();
    close(this->sock);
    close(this->xml_sock);

    throw ANotifyException("Cannot open log file", errno, this);
  }
    
  this->notify = new ANotify();
  this->mask = ANOTIFY_RENAME | ANOTIFY_WRITE | ANOTIFY_DELETE | ANOTIFY_ATTRIBUT;
  this->notifyEvent = new ANotifyEvent(&this->sharedEvent, this->mask);
  
  pthread_mutex_init(&this->runningAccess, NULL);
  pthread_mutex_init(&this->logLock, NULL);  
  pthread_mutex_init(&this->aliveAccess, NULL);
  pthread_mutex_init(&this->xmlSockLock, NULL);
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
  pthread_t thread;

  arg.first = this;
  arg.second = path;

  return (pthread_create(&thread, NULL, startT, (void*)&arg) == 0);
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

  res = dae->addWatch(path);

  if(!res){
    str = "[ERROR] failed to start daemon on path '" + path;
    dae->printLog(str);
    res = false;
    pthread_exit(&res);
  }
  
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
  
  if(!this->isRunning()){
    /* TODO: renvoyer faux ou vrai ? */
    return false;
  }

  if(stat(path.c_str(), &st) != 0){
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
      logMsg = "[WATCH] added '" + newpath + "'";
      printLog(logMsg);
    }catch(ANotifyException e){
      logMsg = "[ERROR] failed to add '" + newpath + "'";
      this->printLog(logMsg);
      return false;
    }
    
    dir = opendir(newpath.c_str());
    
    if(dir == NULL){
      logMsg = "[ERROR] failed to open dir '" + newpath;
      printLog(logMsg);
      return false;
    }
    
    while((entry = readdir(dir)) != NULL){
      if(strcmp(entry->d_name, ".") == 0
	 || strcmp(entry->d_name, "..") == 0){
	continue;
      }

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
  std::string str, tmp = "[EVENT] ", xmlStr;
  pthread_t xmlThread;
  //XMLGeneration xmlGen;

  while(dae->isRunning()){
    if(dae->notify->getEvent(pEvt)){
      //traiter l'event
      pEvt.dumpTypes(str);
      str = tmp + str + pEvt.getName();
      dae->printLog(str);      
      
      /* TODO: remplacer par la bonne fonction */
      xmlStr = XMLGeneration::MIToBI(pEvt);
      //xmlStr = xmlGen.MIToBI(pEvt);

      std::pair<ANotifyDaemon*, std::string> daemon_xml;

      daemon_xml.first = dae;
      daemon_xml.second = xmlStr;

      pthread_create(&xmlThread, NULL, sendXmlContent, (void*)&daemon_xml);
    }

    sleep(1);
  }
}

void* ANotifyDaemon::sendXmlContent(void* arg){
  if(arg != NULL){
    std::pair<ANotifyDaemon*, std::string>* daemon_xml = (std::pair<ANotifyDaemon*, std::string>*)arg;
    ANotifyDaemon* dae = daemon_xml->first;
    std::string xmlContent = daemon_xml->second;
    
    pthread_mutex_lock(&dae->xmlSockLock);

    send(dae->xml_sock, xmlContent.c_str(), xmlContent.length(), 0);

    pthread_mutex_unlock(&dae->xmlSockLock);
  }					 
}

void* ANotifyDaemon::waitForEvents(void* arg){
  ANotifyDaemon* dae = (ANotifyDaemon*)arg;

  while(dae->isRunning()){
    try{
      dae->notify->waitForEvents(true);
    }catch(ANotifyException e){
      /* TODO: Continuer ou s'arreter en cas d'erreur ? */
      //continue;
      break;
    }
  }
}

void ANotifyDaemon::waitForClients(){
  pthread_t thread;
  pthread_create(&thread, NULL, waitForClients, (void*)this);
}

void* ANotifyDaemon::waitForClients(void* arg){
  pid_t child_pid;
  int client, res;
  std::string str;
  ANotifyDaemon* dae = (ANotifyDaemon*)arg;
  socklen_t len = sizeof(dae->addr);
 
  str = "[SOCKET] waiting for clients";
  dae->printLog(str);

  /* Attente de la connexion d'un ou plusieurs clients */
  while(true){
    client = accept(dae->sock, (struct sockaddr*)(&dae->addr), &len);
    
    /* Fermeture de la socket serveur */
    if(client == -1){
      break;
    }

    str = "[SOCKET] new client";
    dae->printLog(str);
    
    pthread_t client_thread;
    std::pair<ANotifyDaemon*, int> infos_client;
    
    infos_client.first = dae;
    infos_client.second = client;
    
    res = pthread_create(&client_thread, NULL, communicate, (void*)(&infos_client));
    if(res != 0){
      close(client);
    }
  }

  pthread_exit(NULL);
}

bool ANotifyDaemon::restart(){
  return restart(this->watchPath);
}

bool ANotifyDaemon::restart(std::string& path){
  bool startRes, stopRes;

  stopRes = this->stop();
  startRes = this->start(path);

  //return startRes && stopRes;
  return startRes;
}

bool ANotifyDaemon::list(int client){
  int len;
  std::string path;
  int pathLength;
  std::vector<std::string>::iterator it;
  std::vector<std::string> paths;
  int nb;

  if(this->notify != NULL){
    paths = this->notify->getWatchPaths();
    nb = paths.size();

    len = send(client, &nb, sizeof(int), 0);

    if(len <= 0){
      return false;
    }

    it = paths.begin();

    while(it != paths.end()){
      path = *it;
      pathLength = path.length();

      //Envoi de la longueur du chemin
      len = send(client, &pathLength, sizeof(int), 0);

      if(len <= 0){
	return false;
      }
 
      //Envoi du chemin
      len = send(client, path.c_str(), path.length(), 0);
      
      if(len <= 0){
	return false;
      }
      
      it++;
    }
  }
  else{
    return false;
  }

  return true;
}

bool ANotifyDaemon::stop(){
  if(!this->isRunning()){
    return false;
  }  

  this->setRunning(false);

  if(notify != NULL){
    try{
      this->notify->Close();
    }catch(ANotifyException e){}
  }

  return true;
}

bool ANotifyDaemon::kill(){
  if(!this->isAlive()){
    return false;
  }

  /* TODO: voir quand il faut renvoyer false */
  this->stop();
  this->deletePropsFile();
  this->closeLogFile();
  this->deleteLogFile();
  
  close(this->xml_sock);
  close(this->sock); 

  if(this->notifyEvent != NULL){
    try{
      delete this->notifyEvent;
      this->notifyEvent = NULL;
    }catch(ANotifyException e1){}
  }

  if(this->notify != NULL){
    try{
      delete this->notify;
      this->notify = NULL;
    }catch(ANotifyException e2){}
  }

  pthread_mutex_destroy(&this->runningAccess);
  pthread_mutex_destroy(&this->logLock);
  pthread_mutex_destroy(&this->aliveAccess);
  pthread_mutex_destroy(&this->xmlSockLock);

  this->setAlive(false);

  return true;
}

void* ANotifyDaemon::communicate(void* arg){
  int readLength, pathLength, length;
  char c;
  bool res = true, alive = true, withPath = false, rec = false, answer = false;
  std::pair<ANotifyDaemon*, int>* daemon_socketfd = (std::pair<ANotifyDaemon*, int>*)arg;
  ANotifyDaemon* dae = daemon_socketfd->first;
  int client_socket = daemon_socketfd->second;
  std::string str, pathStr;
  char path[300];

  while(alive){
    readLength = recv(client_socket, &c, 1, 0);

    if(readLength <= 0){
      /* TODO: voir ce qu'il faut faire dans ce cas la */
      //return;
      str = "[SOCKET] client disconnected";
      dae->printLog(str);
      
      close(client_socket);
      pthread_exit(NULL);
    }
    
    answer = true;

    switch(c){
    case 'D'://delete (removes a watch)
      //Recursivite
      readLength = recv(client_socket, &c, 1, 0);
      if(readLength <= 0){
	//return;	
	close(client_socket);
	pthread_exit(NULL);
      }
      rec = (c == 'Y') ? true : false; 

      //Taille du chemin
      readLength = recv(client_socket, &pathLength, sizeof(int), 0);
      if(readLength <= 0){
	//return;
	close(client_socket);
	pthread_exit(NULL);
      }
      
      //char path[pathLength + 1];

      //Chemin
      readLength = recv(client_socket, path, pathLength, 0);
      if(readLength <= 0){
	//return;
	close(client_socket);
	pthread_exit(NULL);
      }
      if(readLength < pathLength){
	//res = false;
	close(client_socket);
	pthread_exit(NULL);
      }
      
      path[pathLength] = '\0';

      //if(res){
      //std::string pathStr(path);
      pathStr = path;
      str = "[WATCH] removing '";
      str += pathStr + "'";
      dae->printLog(str);
      
      res = dae->removeWatch(pathStr, rec);
      //}
      break;

    case 'S'://start (starts the daemon) [A voir]
      //Chemin fourni
      readLength = recv(client_socket, &c, 1, 0);
      if(readLength <= 0){
	//return;
	close(client_socket);
	pthread_exit(NULL);
      }
      withPath = (c == 'Y') ? true : false;      
      
      if(withPath){
	//Taille du chemin
	readLength = recv(client_socket, &pathLength, sizeof(int), 0);
	if(readLength <= 0){
	  //return;
	  close(client_socket);
	  pthread_exit(NULL);
	}
	
	//char path[pathLength + 1];
	
	//Chemin
	readLength = recv(client_socket, path, pathLength, 0);
	if(readLength <= 0){
	  //return;	  
	  close(client_socket);
	  pthread_exit(NULL);
	}
	if(readLength < pathLength){	  
	  close(client_socket);
	  pthread_exit(NULL);
	  //res = false;
	}
	
	path[pathLength] = '\0';

	//if(res){
	//std::string pathStr(path);
	pathStr = path;
	str = "[PROCESS] starting daemon on '";
	str += pathStr + "'";
	dae->printLog(str);
	
	res = dae->start(pathStr);
	//}
      }
      else{
	//if(res){
	str = "[PROCESS] starting daemon on '";
	str += dae->watchPath + "'";
	dae->printLog(str);
	
	res = dae->start();
	//}
      }
      break;
      
    case 'K'://kill (definitively kill the daemon) [A voir]
      str = "[PROCESS] killing daemon";
      dae->printLog(str);

      res = dae->kill();
      alive = false;
      break;

    case 'E'://end (stops the daemon)
      str = "[PROCESS] stopping daemon";
      dae->printLog(str);

      res = dae->stop();
      break;

    case 'R'://restart (restarts the daemon)
      //Chemin fourni
      readLength = recv(client_socket, &c, 1, 0);
      if(readLength <= 0){
	//return;
	close(client_socket);
	pthread_exit(NULL);
      }
      withPath = (c == 'Y') ? true : false;      
      
      if(withPath){
	//Taille du chemin
	readLength = recv(client_socket, &pathLength, sizeof(int), 0);
	if(readLength <= 0){
	  //return;
	  close(client_socket);
	  pthread_exit(NULL);
	}
	
	//char path[pathLength + 1];
	
	//Chemin
	readLength = recv(client_socket, path, pathLength, 0);
	if(readLength <= 0){
	  //return;
	  close(client_socket);
	  pthread_exit(NULL);
	}
	if(readLength < pathLength){
	  //res = false;
	  close(client_socket);
	  pthread_exit(NULL);
	}
	
	path[pathLength] = '\0';
	
	//if(res){
	//std::string pathStr(path);
	pathStr = path;
	str = "[PROCESS] restarting daemon on '";
	str += pathStr + "'";
	dae->printLog(str);
	
	res = dae->restart(pathStr);
	//}
      }
      else{
	//if(res){
	str = "[PROCESS] restarting daemon on '";
	str += dae->watchPath + "'";
	dae->printLog(str);
	
	res = dae->restart();
	  //}
      }
      break; 

    case 'L'://list (list current watches)
      str = "[WATCH] listing watched files";
      dae->printLog(str);

      res = dae->list(client_socket);
      break;

    default:
      answer = false;
      break;
    }

    if(answer){
      c = res ? 'Y' : 'N';
      send(client_socket, &c, 1, 0);
    }
  }

  close(client_socket);
  pthread_exit(NULL);
}

int ANotifyDaemon::openPropsFile(){
  /* TODO: Modifier le chemin si besoin est + poser un verrou sur le fichier */
  int ret, fd;   
  
  //this->propsFd = open(propsPath, O_RDWR | O_CREAT, 0777);  
  fd = open(propsPath.c_str(), O_RDWR | O_CREAT | O_APPEND, 0666);

  if(fd == -1){
    return -1;
  }
  /*
    ret = flock(fd, LOCK_EX | LOCK_NB);

    if(ret == -1){
    return -1;
    }*/

  return fd;
}

int ANotifyDaemon::closePropsFile(int fd){   
  /* TODO: Liberer le verrou sur le fichier de proprietes */
  //flock(fd, LOCK_UN);
  return close(fd);  
}

int ANotifyDaemon::deletePropsFile(){
  return remove(propsPath.c_str());
  //return unlink(propsPath.c_str());
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

int ANotifyDaemon::deleteLogFile(){
  return remove(logPath.c_str());
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

bool ANotifyDaemon::isAlive(){
  bool res = false;

  pthread_mutex_lock(&aliveAccess);
  res = this->alive;
  pthread_mutex_unlock(&aliveAccess);

  return res;
}

void ANotifyDaemon::setAlive(bool alv){
  pthread_mutex_lock(&aliveAccess);
  this->alive = alv;
  pthread_mutex_unlock(&aliveAccess);
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
  /*if(!rec){
    return removeOneWatch(path);
  }
  else{
    return recRemoveWatch(path);
    }*/

  if(this->notify == NULL){
    return false;
  }

  return this->notify->remove(path, rec);
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
  //WatchPathMap::iterator it;
  //WatchPathMap paths;
  std::vector<std::string>::iterator it;
  std::vector<std::string> paths;
  std::string filepath;
  int filepathLength;
  ANotifyWatch* pWatch;

  /* Obtenir un verrou sur la map */

  if(this->notify != NULL){
    //paths = this->notify->getWatchesPathMap();
    paths = this->notify->getWatchPaths();
    it = paths.begin();

    while (it != paths.end()) {
      //filepath = it->first;
      filepath = *it;
      filepathLength = filepath.length();

      if(strncmp(filepath.c_str(), path.c_str(), path.length()) == 0){
      //paths.erase(path);
	//paths.erase(filepath);
	
	pWatch = this->notify->findWatch(filepath);
	if(pWatch != NULL){
	  this->notify->remove(pWatch);
	}
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
