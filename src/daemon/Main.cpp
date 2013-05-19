#include <stdio.h>
#include <stdlib.h>

#include "ANotify.h"
#include "ANotifyWatch.h"
#include "ANotifyEvent.h"
#include "ANotifyException.h"
#include "ANotifyDaemon.h"

int main(){
  pid_t pid;
  std::string msg;
  
  pid = fork();

  if(pid > 0){
    
  }
  else if(pid == 0){
    try{
      ANotifyDaemon dae;

      try{
	dae.initDaemon();
	//dae.forkInit();
      }catch(ANotifyException e1){      
	std::cout << e1.GetMessage() << std::endl;
	msg = e1.GetMessage();
	//dae.printLog(msg);
	dae.kill();
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
  
  exit(EXIT_SUCCESS);
  //return 0;
}
