#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "ANotify.h"
#include "ANotifyWatch.h"
#include "ANotifyEvent.h"
#include "ANotifyException.h"
#include "ANotifyDaemon.h"


ANotifyDaemon* dae = NULL;

void sigint_handler(int sig){
  /*try{
    if(dae != NULL) {
      dae->restart();
    }
  }catch(ANotifyException e){
    cout << e.GetMessage() << endl;
    } */ 
}

int main(){
  pid_t pid;
  std::string msg;
  
  pid = fork();

  if(pid > 0){
    exit(EXIT_SUCCESS);
  }
  else if(pid == 0){
    try{
      signal(SIGTERM, sigint_handler);

      ANotifyDaemon dae;
      
      dae.start();
      sleep(1);
      dae.waitForClients();
      //dae.start();

      while(dae.isAlive()){
	//dae.printLog(str);
	sleep(1);
      }
    }catch(ANotifyException e2){
      std::cout << e2.GetMessage() << std::endl;
    }
  
    exit(EXIT_SUCCESS);
  }
  else{
    std::cout << "fork failed" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  //exit(EXIT_SUCCESS);
  return 0;
}
