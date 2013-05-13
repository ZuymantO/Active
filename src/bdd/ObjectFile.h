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

namespace acommon {

using namespace std;

class ObjectFile : public AnyFile {
private:
  
protected:
  string title;
  string subject;
  string keyword;
  string authors;
  unsigned int   nb_page;
public:
  ObjectFile(){};
  ~ObjectFile(){};
  
  bool majField(const string& irname, const string& irvalue){
    if(!AnyFile::majField(irname, irvalue)){  // Si aucune valeur est modifie chez le pere ...
      if (irname == "nb_page") {
        nb_page = atoi(irvalue.c_str());
        return true;
      }else if(irname == "title"){
        title = irvalue;
        return true;
      }else if(irname == "subject"){
        subject = irvalue;
        return true;
      }else if(irname == "authors"){
        authors = irvalue;
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
#endif /* defined(__BI__ObjectFile__) */
