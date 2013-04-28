//
//  ImageFile.h
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#ifndef __BI__ImageFile__
#define __BI__ImageFile__

#include <iostream>
#include "AnyFile.h"
namespace acommon {

using namespace std;
class ImageFile : public AnyFile {
private:
  
protected:
  unsigned int width;
  unsigned int height;
  unsigned int color_avg;
  
public:
  ImageFile(){};
  ~ImageFile(){};
  
  bool majField(const string& irname, const string& irvalue){
    if(!AnyFile::majField(irname, irvalue)){  // Si aucune valeur est modifie chez le pere ...
      if(irname == "width"){
        width = atoi(irvalue.c_str());
        return true;
      }else if(irname == "height"){
        height = atoi(irvalue.c_str());
        return true;
      }else if(irname == "color_avg"){
        color_avg = atoi(irvalue.c_str());
        return true;
      }
    }
    return false;
  };

  
};
}
#endif /* defined(__BI__ImageFile__) */
