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
};
#endif /* defined(__BI__ImageFile__) */
