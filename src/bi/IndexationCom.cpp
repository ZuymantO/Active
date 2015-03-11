#include <pthread.h>

#include "IndexationCom.h"

IndexationCom::IndexationCom() throw (BaseException) : client_connected(false) {
  //Mettre le serveur en ecoute
  
}

void IndexationCom::waitForClients(){

}

void* IndexationCom::waitForClients(void* arg){
  IndexationCom* iCom = (IndexationCom*)arg;

  if(iCom->server_sock != -1){

  }

  pthread_exit(NULL);
}

void IndexationCom::waitForEvents(){
  
}

void* IndexationCom::waitForEvents(void* arg){
  std::pair<IndexationCom*, int>* com_sock = (std::pair<IndexationCom*, int>*)arg;
  IndexationCom* iCom = com_sock->first;
  int mi_sock = com_sock->second;

  if(iCom == NULL){
    pthread_exit(NULL);
  }

  if(mi_sock != -1){
    /* while(true){

       }*/
  }

  pthread_exit(NULL);
}
