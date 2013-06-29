#ifndef __INDEXATION_COM_H__
#define __INDEXATION_COM_H__

#include "BaseException.h"

using namespace std;

class IndexationCom {
 private:
  int xml_sock;

 public:
  IndexationCom() throw (BaseException);
};

#endif
