#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>

//#include "ANotifyDaemon.h"
#include "Daemon.h"
#include "ANotify.h"
#include "ANotifyException.h"


ANotifyDaemon::ANotifyDaemon() throw (ANotifyException) {
  int fd, port = -1, i, client, length;
  struct sockaddr_in addr;
  std::string str = "";
  pid_t current_pid = getpid();  
  
  this->running = false;

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
    closeProsFile(fd);
    close(this->sock);
    throw ANotifyException("No port available");
  }

  /*
    Contenu du fichier daemon.prop:

    pid:pid_du_daemon
    port:port_du_serveur
  */

  str = "pid:" + current_pid + "\n";
  str += "port:" + port;

  if(write(fd, str.c_str(), str.length()) < 0){
    closePropsFile(fd);
    close(this->sock);
    
    throw ANotifyException("Cannot write into properties file", errno, this);
  }

  closePropsFile(fd);
}

ANotifyDaemon::~ANotifyDaemon(){
  
}

void ANotifyDaemon::start(){
  if(this->running){
    return;
  }


  this->running = true;
}

void ANotifyDaemon::waitForClients(){
  pid_t child_pid;
  int client;
 
  /* Attente de la connexion d'un ou plusieurs clients */
  while(true){
    client = accept(this->sock, (struct sockaddr*)(&addr), &length);
    
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
      break;
    }
    else{
      close(client);
      close(this->sock);
      throw ANotifyException("Fork failed", errno, this);
    }
  }
}

void ANotifyDaemon::restart(){
  this->stop();
  this->start();
}

void ANotifyDaemon::stop(){
  if(!this->running){
    return;
  }  

  if(notify != NULL){
    delete notify;
    notify = NULL;
  }

  this->running = false;
}

void ANotifyDaemon::kill(){  
  stop();
  deletePropsFile();
  close(this->sock);
}

void ANotifyDaemon::communicate(int client_socket){
  int readLength, length;
  char c;

  while(true){
    readLength = read(client_socket, &c, 1, 0);

    if(readLength == -1){
      /* TODO: voir ce qu'il faut faire dans ce cas la */
      break;
    }

    switch(c){
    case 'D'://delete (remove watch) 
      break;

    case 'S'://start (starts daemon) [A voir]
      break;

    case 'K'://kill (definitively kill the daemon) [A voir]
      break;

    case 'E'://end (stops daemon)
      this->stop();
      break;

    case 'R'://restart (restarts daemon)
      this->restart();
      break;    
    }
  }
}

int ANotifyDaemon::openPropsFile(){
  /* TODO: Modifier le chemin si besoin est + poser un verrou sur le fichier */
  fd = open(propsPath, O_RDWR | O_CREAT);
}

int ANotifyDaemon::closePropsFile(int fd){
  close(fd);  
  /* TODO: Liberer le verrou sur le fichier de proprietes */
}

int ANotifyDaemon::deletePropsFile(){
  return remove(propsPath);
}

bool ANotifyDaemon::isActiveDaemon(int props_fd, pid_t pid){
  char buffer[50];
  int length; 

  length = read(props_fd, buffer, 50);

  if(length == 0){
    return true;
  }

  buffer[length] = '\0';

  if(length <strncmp(buffer, "pid:", 4) != 0
     || length < 4){
    return false;
  }

  std::string firstLine(buffer);

  
}

bool removeWatch(std::string& path){
  return true;
}

bool recRemoveWatch(std::string& path){
  return true;
}
