//
//  TextFile.h
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#ifndef __BI__TextFile__
#define __BI__TextFile__

#include <iostream>
#include "AnyFile.h"
using namespace std;
class TextFile : public AnyFile {
private:
  
protected:
  
public:
  unsigned int nb_word;
  unsigned int nb_line;
  string keyword;
  TextFile();
  ~TextFile();
};
#endif /* defined(__BI__TextFile__) */
