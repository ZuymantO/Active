#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>

#include "ANotifyDaemon.h"
#include "ANotify.h"
#include "ANotifyException.h"

//using namespace std;

ANotifyDaemon::ANotifyDaemon(){
  pid_t pid = fork();

  if(pid > 0){
    //On ne fait rien ?
    std::cout << "daemon starting ..." << std::endl;
  }
  else if(pid == 0){
    this->start();
  }
  else{
    std::string msg = "";

    switch(errno){
    case ENOMEM:  
      throw ANotifyException("Unable to allocate enough memory for child", errno, this);
      //break;

    case EAGAIN:
      throw ANotifyException("No empty space in process table", errno, this);
      //break;
    }   
  }
}

ANotifyDaemon::~ANotifyDaemon(){
  
}

void ANotifyDaemon::start(){
  int fd, port = -1, i;
  struct sockaddr_in addr;
  std::string portStr = "";

  this->sock = socket(AF_INET, SOCK_STREAM, 0);

  if(this->sock == -1){
    /* TODO: renvoyer le bon message d'exception */
    throw ANotifyException("Failed to open socket", errno, this);
  }

  /* TODO: Modifier le chemin si besoin est + poser un verrou sur le fichier */
  fd = open("daemon_port.prop", O_WRONLY | O_CREAT | O_TRUNC);

  if(fd == -1){
    throw ANotifyException("Unable to open or create daemon properties file", errno, this);
  }

  addr.sin_family = AF_INET;  
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  for(i=5000; i<6000; i++){    
    addr.sin_port = htons(i);

    if(bind(this->sock, (struct sockaddr*)(&addr), sizeof(addr)) != -1
       && listen(this->sock, 0)){
      port = i;
      break;
    }
  } 

  if(port == -1){
    close(this->sock);
    throw ANotifyException("No port available");
  }

  portStr = "" + port;
  write(fd, portStr.c_str(), portStr.length());
}

void ANotifyDaemon::restart(){
  this->stop();
  this->start();
}

void ANotifyDaemon::stop(){
  
}

bool removeWatch(std::string& path){
  return true;
}

bool recRemoveWatch(std::string& path){
  return true;
}

int main(int argc, char** argv){
  return 0;
}
