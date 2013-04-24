//
//  VideoFile.cpp
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#include "VideoFile.h"



static string formatToString(VideoFormat iformat){
  switch (iformat) {
    case F4K:
      return "4K";
      break;
    case FHD:
      return "FHD";
      break;
    case HD:
      return "HD";
      break;
    case SD:
      return "SD";
      break;
    case LQ:
      return "Low Quality";
      break;
    case MQ:
      return "Mobile";
      break;
    default:
      return "Mobile";
      break;
  }
}

static VideoFormat stringToFormat(const string& irformat){
  if (irformat == "4k") {
    return F4K;
  }else if (irformat == "FHD"){
    return FHD;
  }else if (irformat == "HD"){
    return HD;
  }else if (irformat == "SD"){
    return SD;
  }else if (irformat == "Low Quality"){
    return LQ;
  }else if (irformat == "Mobile"){
    return MQ;
  }else{
    return SD;
  }
}



