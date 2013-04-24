//
//  ObjectFile.h
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#ifndef __BI__ObjectFile__
#define __BI__ObjectFile__

#include <iostream>
#include "AnyFile.h"
using namespace std;
class ObjectFile : public AnyFile {
private:
  
protected:
  string title;
  string subject;
  string keyword;
  string authors;
  unsigned int   nb_pages;
public:
  ObjectFile(){};
  ~ObjectFile(){};
};
#endif /* defined(__BI__ObjectFile__) */
