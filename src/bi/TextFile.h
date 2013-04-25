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

namespace acommon {

  
using namespace std;
class TextFile : public AnyFile {
private:
  
protected:
  
public:
  unsigned int nb_word;
  unsigned int nb_line;
  string keyword;
  TextFile(){};
  ~TextFile(){};
  
  bool majField(const string& irname, const string& irvalue){
    if(!AnyFile::majField(irname, irvalue)){  // Si aucune valeur est modifie chez le pere ...
      if (irname == "nb_word") {
        nb_word = atoi(irvalue.c_str());
        return true;
      }else if(irname == "nb_line"){
        nb_line = atoi(irvalue.c_str());
        return true;
      }else if(irname == "keyword"){
        keyword = irvalue;
        return true;
      }
      
    }
    return false;
  };

};
}
#endif /* defined(__BI__TextFile__) */
