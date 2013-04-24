//
//  VideoFile.h
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#ifndef __BI__VideoFile__
#define __BI__VideoFile__

#include <iostream>
#include "AnyFile.h"
using namespace std;
typedef enum _vidFormat {
  F4K = 512,  // 4K definition
  FHD = 256,  // Full HD
  HD  = 128,  // Hight definition
  SD = 64,  // standar quality
  LQ = 16,  // low quality
  MQ = 8    // mobile quality
  }VideoFormat;
class VideoFile : public AnyFile {
private:
  
protected:
  string actors;
  string title;
  string realisator;
  string genre;
  VideoFormat format;
  unsigned int width;
  unsigned  int height;
  unsigned short fps;
  bool  stereoSound;
  unsigned int year;
  unsigned int duration; // en seconde;

public:
  VideoFile(){};
  ~VideoFile(){};

  bool majField(const string& irname, const string& irvalue){
    if(!AnyFile::majField(irname, irvalue)){  // Si aucune valeur est modifie chez le pere ...
      if (irname == "actors") {
        actors = irvalue;
        return true;
      }else if(irname == "title"){
        title = irvalue;
        return true;
      }else if(irname == "realisator"){
        realisator = irvalue;
        return true;
      }else if(irname == "genre"){
        genre = irvalue;
        return true;
      }else if(irname == "format"){
        format = (VideoFormat) atoi(irvalue.c_str());
        return true;
      }else if(irname == "width"){
        width = atoi(irvalue.c_str());
        return true;
      }else if(irname == "height"){
        height = atoi(irvalue.c_str());
        return true;
      }else if(irname == "fps"){
        fps = atoi(irvalue.c_str());
        return true;
      }else if(irname == "stereoSound"){
        stereoSound = atoi(irvalue.c_str()) == 1;
        return true;
      }else if(irname == "year"){
        year = atoi(irvalue.c_str());
        return true;
      }else if(irname == "duration"){
        duration = atoi(irvalue.c_str()) ;
        return true;
      }
    }
    return false;
  };
  
};
#endif /* defined(__BI__VideoFile__) */
