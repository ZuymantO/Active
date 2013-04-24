//
//  AudioFile.h
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#ifndef __BI__AudioFile__
#define __BI__AudioFile__

#include <iostream>
#include "AnyFile.h"
using namespace std;
class AudioFile : public AnyFile {
private:
  
protected:
  string artists;
  string title;
  string albums;
  string genre;
  double bitrate;
  bool  stereo;
  unsigned int year;
  unsigned int duration; // en seconde;
  
public:
  AudioFile(){};
  ~AudioFile(){};
};
#endif /* defined(__BI__AudioFile__) */
