#include "ANotify.h"
#include "ANotifyWatch.h"
#include "ANotifyEvent.h"
#include "ANotifyException.h"
#include "ANotifyDaemon.h"

int main(){
  
  try{
    ANotifyDaemon dae;

    try{
      dae.initDaemon();
    }catch(ANotifyException e1){      
      std::cout << e1.GetMessage() << std::endl;
      dae.kill();
    }
  }catch(ANotifyException e2){
    std::cout << e2.GetMessage() << std::endl;
  }

  return 0;
}
