#ifndef __SEARCH_COM_H__
#define __SEARCH_COM_H__

#include "BaseException.h"

class SearchCom {
 private:
  int xml_sock;

 public:
  SearchCom() throw (BaseException);
};

#endif
