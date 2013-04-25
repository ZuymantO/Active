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

namespace acommon {

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
  
  bool majField(const string& irname, const string& irvalue){
    if(!AnyFile::majField(irname, irvalue)){  // Si aucune valeur est modifie chez le pere ...
      if (irname == "actors") {
        artists = irvalue;
        return true;
      }else if(irname == "title"){
        title = irvalue;
        return true;
      }else if(irname == "albums"){
        albums = irvalue;
        return true;
      }else if(irname == "genre"){
        genre = irvalue;
        return true;
      }else if(irname == "bitrate"){
        double d = 0.;
//        stringstream istr = irvalue.c_str();
//        
//        if (!(istr >> d))
//          return false;
        d = (double) atoi(irvalue.c_str());
        bitrate = d;
        return true;
      }else if(irname == "stereo"){
        stereo = atoi(irvalue.c_str()) == 1;
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
}

#endif /* defined(__BI__AudioFile__) */
