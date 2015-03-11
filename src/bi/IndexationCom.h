#ifndef __INDEXATION_COM_H__
#define __INDEXATION_COM_H__

#include <pthread.h>

#include "BaseException.h"

using namespace std;

class IndexationCom {
 private:
  int server_sock;
  bool client_connected;

  pthread_mutex_t queueAccess;

 public:
  IndexationCom() throw (BaseException);

  ~IndexationCom();

  /* Attend la connexion d'un MI */
  void waitForClients();
  /* Fonction thread correspondante */
  static void* waitForClients(void* iCom);


  /* Attend des evenements au format XML provenant d'un MI */
  void waitForEvents();
  /* Fonction thread correspondante */
  static void* waitForEvents(void* iCom);
};

#endif
