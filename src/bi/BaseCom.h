#ifndef __BASE_COM_H__
#define __BASE_COM_H__

#include "BaseException.h"
#include <pthread.h>

class BaseCom {
 private:
  pthread_mutex_t baseAccess;

 public:
  /* Tentative de connexion a la base, creation si elle n'existe pas */
  BaseCom() throw (BaseException);

  /* Trouver le bon argument et le bon type de retour */
  std::string executeSearchQuery();
  std::string executeUpdateQuery();
};

#endif
