//
//  SQLQuery.cpp
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//
#include <iostream>
#include <vector>
#include <map>
#include "common.h"
#include "SQLQuery.h"

  
using namespace std;
using namespace acommon;
namespace asqlite {
  
acommon::AQuery* SQLQuery::getAQuery() throw(asqlite::SQLite3DBException){
  if (SQLResult == NULL) {
    throw SQLite3DBException("Please call SQLQuery::prepare methode \nthen call the fetch methode before looking for an AQuery object");
  }
  acommon::AQuery* rslt = new acommon::AQuery(aqt, aobj);
  rslt->AQResult = new vector<acommon::AnyFile>();
  vector<map<string, string> >::iterator it = SQLResult->begin();
  for (; it != SQLResult->end(); it++) {
    map<string, string> val = *it;
    map<string, string>::iterator tmpIt = val.begin();
    AnyFile obj = createFile(aobj);
    for (; tmpIt != val.end(); tmpIt++) {
      pair<string, string> v = *tmpIt;
      if(!obj.majField(v.first, v.second)){
        cout << "Impossible de mettre a jour la donnee " << v.first << ", " << v.second << endl;}
    }
    rslt->AQResult->push_back(obj);

  }
  return rslt;
}

AnyFile SQLQuery::createFile(AQueryObject iAQO){
  switch (iAQO) {
    case ANY:
      return AnyFile();
      break;
    case TEXT:
      return TextFile();
      break;
    case IMAGE:
      return ImageFile();
      break;
    case AUDIO:
      return AudioFile();
      break;
    case VIDEO:
      return VideoFile();
      break;
    case ARCHIVE:
      return ArchiveFile();
      break;
    case OBJECT:
      return ObjectFile();
      break;
    default:  // Normalement Erreur ...
      return AnyFile();
      break;
  }
  
};
}

