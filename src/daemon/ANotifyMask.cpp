//
//  ANotifyMask.cpp
//  ANotify
//
//  Created by J. Charles N. MBIADA on 10/03/13.
//  Copyright (c) 2013 Active All rights reserved.
//
// TODO 1.
/*********************************************************************
 *
 *
 *
 *
 *
 *
 **********************************************************************/

#include <iostream>
#include "ANotifyMask.h"

 ANMask ANotifyMask::getMaskByName(const std::string& irName) {
  if (irName == "IN_ATTRIB" || irName == "ATTRIB" || irName == "ATTRIBUT")
    return ANOTIFY_ATTRIBUT;
  else if (irName == "IN_MODIFY" || irName == "MODIFY")
    return ANOTIFY_WRITE;
  else if (irName == "IN_CLOSE_WRITE" || irName == "CLOSE_WRITE")
    return ANOTIFY_WRITE;
  else if (irName == "IN_CREATE" || irName == "CREATE")
    return ANOTIFY_WRITE;
  else if (irName == "IN_MOVED_FROM" || irName == "MOVED_FROM")
    return ANOTIFY_RENAME;
  else if (irName == "IN_MOVED_TO" || irName == "MOVED_TO")
    return ANOTIFY_RENAME;
  else if (irName == "IN_MOVE" || irName == "MOVE")
    return ANOTIFY_RENAME;
  else if (irName == "IN_DELETE" || irName == "DELETE")
    return ANOTIFY_DELETE;
  else if (irName == "IN_DELETE_SELF" || irName == "DELETE_SELF")
    return ANOTIFY_DELETE;
  else if (irName == "IN_UNMOUNT" || irName == "UNMOUNT")
    return ANOTIFY_DELETE;
  else if (irName == "IN_Q_OVERFLOW" || irName == "OVERFLOW")
    return ANOTIFY_REVOKE;
  else if (irName == "IN_IGNORED" || irName == "IGNORED")
    return ANOTIFY_REVOKE;
  else if (irName == "IN_CLOSE" || irName == "CLOSE")
    return ANOTIFY_NONE;
  else if (irName == "IN_CLOSE_NOWRITE" || irName == "CLOSE_NOWRITE")
    return ANOTIFY_NONE;
  else if (irName == "IN_ISDIR" || irName == "ISDIR")
    return ANOTIFY_NONE;
  else if (irName == "IN_ALL_EVENTS" || irName == "EVENTS")
    return ANOTIFY_NONE;
  else if (irName == "IN_OPEN" || irName == "OPEN")
    return ANOTIFY_NONE;
  else if (irName == "IN_ONESHOT" || irName == "ONESHOT")
    return ANOTIFY_ONESHOT;
  else if (irName == "NOTE_EXTEND" || irName == "EXTEND" || irName == "IN_EXTEND")
    return ANOTIFY_EXTEND;
  else if (irName == "ISDIR" || irName == "ACCESS" || irName == "OPEN" || irName == "OTHER" ) ///< USEFULL ON LINUX
    return ANOTIFY_OTHER;
  
#ifdef IN_MOVE_SELF
  else if (irName == "IN_MOVE_SELF" || irName == "MOVE_SELF")
    return ANOTIFY_RENAME;
#endif // UNKNOWN CASE
  else
    return (ANMask) ANOTIFY_OTHER;
}



void ANotifyMask::dumpTypes(ANMask iValue, std::string* orStr)
{
  *orStr = "";

    // Lecture d'event
  /*if (isType(iValue, ANOTIFY_OTHER)) {
    orStr->append("OTHER OR ACCESS OR ISDIR OR OPEN OR NOP");
  }
  else {*/

    if (isType(iValue, ANOTIFY_DELETE)) {
      orStr->append(",");
      orStr->append("DELETE OR DELETE_SELF OR UNMOUNT OR MOVED_FROM");
    }
    if (isType(iValue, ANOTIFY_RENAME)) {
      orStr->append(",");
      //orStr->append("RENAME OR MOVE OR MOVE_TO OR MOVE_FROM");
      orStr->append("RENAME");
    }
    if (isType(iValue, ANOTIFY_ATTRIBUT)) {
      orStr->append(",");
      orStr->append("ATTRIBUT");
    }
    if (isType(iValue, ANOTIFY_WRITE)) {
      orStr->append(",");
      //orStr->append("WRITE OR MODIFY OR CREATED");
      orStr->append("WRITE OR MODIFY OR CREATED OR MOVED_TO");
    }
  //}
    // cas d'ecriture et lecture
  if (isType(iValue, ANOTIFY_ONESHOT)) {
    orStr->append(",");
    orStr->append("ONESHOT");
  }
  if (isType(iValue, ANOTIFY_NONE)) {
    orStr->append(",");
    orStr->append("NONE OR NOP");
  }
  if (isType(iValue, ANOTIFY_EXTEND)) {
    orStr->append(",");
    orStr->append("EXTEND");
  }
  if (isType(iValue, ANOTIFY_REVOKE)) {
    orStr->append(",");
    orStr->append("IGNORED OR Q_OVERFLOW");
  }

}


