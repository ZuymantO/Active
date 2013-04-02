//
//  ANotifyMask.h
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

#ifndef __ANotify__ANotifyMask__
#define __ANotify__ANotifyMask__

#include <iostream>
#include "ANotifyUtils.h"
#include <sys/event.h>

#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#include <sys/inotify.h> // If we are on a supported inotify API system

  // inotify capability

#define INOTIFY_CREATE IN_CREATE
#define INOTIFY_ACCESS IN_ACCESS
#define INOTIFY_MODIFY IN_MODIFY
#define INOTIFY_NOWRITE IN_CLOSE_NOWRITE
#define INOTIFY_OPEN IN_OPEN
#define INOTIFY_MOVED_FROM IN_MOVED_FROM
#define INOTIFY_MOVED_TO IN_MOVED_TO
#define INOTIFY_DELETE_SELF IN_DELETE_SELF
#define INOTIFY_UNMOUNT IN_UNMOUNT
#define INOTIFY_OVERFLOW IN_Q_OVERFLOW
#define INOTIFY_IGNORED IN_IGNORED
#define INOTIFY_CLOSE IN_CLOSE
#define INOTIFY_ISDIR IN_ISDIR
#define INOTIFY_ALL_EVENTS IN_ALL_EVENTS

#ifdef IN_DONT_FOLLOW
#define INOTIFY_DONT_FOLLOW IN_DONT_FOLLOW
#endif // IN_DONT_FOLLOW
#ifdef IN_ONLYDIR
#define INOTIFY_ONLYDIR IN_ONLYDIR
#endif // IN_ONLYDIR
#ifdef IN_MOVE_SELF
#define INOTIFY_MOVE_SELF IN_MOVE_SELF
#endif // IN_MOVE_SELF

  // kqueue intersection capability 10 MASK A UTILISER POUR ETRE COMPATIBLE UNIX/LINUX
#define ANOTIFY_DELETE (IN_DELETE | IN_DELETE_SELF | IN_UNMOUNT) // file deleted
#define ANOTIFY_WRITE (IN_CLOSE_WRITE | IN_MODIFY | IN_CREATE)
#define ANOTIFY_ATTRIBUT IN_ATTRIB // chmod and chowned on this file
#define ANOTIFY_RENAME (IN_MOVE | IN_MOVED_TO | IN_MOVED_FROM) // case of file moving
#define ANOTIFY_ONESHOT IN_ONESHOT
#define ANOTIFY_REVOKE (IN_Q_OVERFLOW | IN_IGNORED) // case of file revoking or qeu overflowing
#define ANOTIFY_NONE 0x00000000
#define ANOTIFY_EXTEND 0x00000000
#define ANOTIFY_LINK 0x00000000
#define ANOTIFY_OTHER (IN_ISDIR | IN_ACCESS)

#else
  // kqueue capability 10 MASK A UTILISER POUR ETRE COMPATIBLE UNIX/LINUX
#define ANOTIFY_DELETE NOTE_DELETE // file deleted
#define ANOTIFY_WRITE NOTE_WRITE
#define ANOTIFY_ATTRIBUT NOTE_ATTRIB // chmod and chowned on this file
#define ANOTIFY_RENAME NOTE_RENAME // case of file moving
#define ANOTIFY_ONESHOT EV_ONESHOT
#define ANOTIFY_REVOKE NOTE_REVOKE // case of file moving
#define ANOTIFY_NONE NOTE_NONE
#define ANOTIFY_EXTEND NOTE_EXTEND
#define ANOTIFY_LINK NOTE_LINK
#define ANOTIFY_OTHER NOTE_FFNOP

  // From inotify (compatibility)
#define INOTIFY_CREATE 0x00000000
#define INOTIFY_ACCESS 0x00000000
#define INOTIFY_MODIFY 0x00000000
#define INOTIFY_NOWRITE 0x00000000
#define INOTIFY_OPEN 0x00000000
#define INOTIFY_MOVED_FROM 0x00000000
#define INOTIFY_MOVED_TO 0x00000000
#define INOTIFY_DELETE_SELF 0x00000000
#define INOTIFY_UNMOUNT 0x00000000
#define INOTIFY_OVERFLOW 0x00000000
#define INOTIFY_IGNORED 0x00000000
#define INOTIFY_CLOSE 0x00000000
#define INOTIFY_ISDIR 0x00000000
#define INOTIFY_ALL_EVENTS 0x00000000
#define INOTIFY_DONT_FOLLOW 0x00000000
#define INOTIFY_ONLYDIR 0x00000000
#define INOTIFY_MOVE_SELF 0x00000000
#endif // __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__

typedef ANMASK ANMask;

class ANotifyMask {
private:
public:
  static ANMask getMaskByName(const std::string& irName);
  static void dumpTypes(ANMask iType, std::string* orStr);
  static inline bool isType(ANMask iValue, ANMask iType){
    return ((iValue & iType) != 0) && ((~iValue & iType) == 0);
  }
};

#endif /* defined(__ANotify__ANotifyMask__) */
