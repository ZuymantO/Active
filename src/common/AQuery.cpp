//
//  AQuery.cpp
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#include "AQuery.h"

namespace acommon {

  void AQuery::addFile(AnyFile* ipnewFile) {
    if (AQResult == NULL) {
      AQResult = new vector<AnyFile>(1, *ipnewFile); 
    }
    else {
      (*AQResult).push_back(*ipnewFile);
    }
    hasResult = true;
  }

  vector<AnyFile>* AQuery::results() {
    return AQResult;
  }

  void AQuery::setId(string s) {
    id = s;
  }

}
