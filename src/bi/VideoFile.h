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
  FD  = 128,  // Hight definition
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
  unsigned int fps;
  bool  stereoSound;
  unsigned int year;
  unsigned int duration; // en seconde;

public:
  VideoFile();
  ~VideoFile();

};
#endif /* defined(__BI__VideoFile__) */
