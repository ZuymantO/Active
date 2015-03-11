#ifndef __INDEXATION_BASE_H__
#define __INDEXATION_BASE_H__

#include "BaseException.h"
#include "SearchCom.h"
#include "IndexationCom.h"
#include "BaseCom.h"

class IndexationBase {
 private:
  /* Communication avec le moteur de recherche */
  SearchCom sCom;
  /* Communication avec le moteur d'indexation */
  IndexationCom iCom;
  /* Interaction avec la base de donnees */
  BaseCom bCom;

 public:
  IndexationBase() throw (BaseException);

};

#endif
