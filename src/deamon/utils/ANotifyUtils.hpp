/*=======
 * ANotifyUtils.hpp
 *=======
 * Created by J. Charles N. MBIADA <jcharles@zuymanto.com> on 08/02/13.
 * Copyright (c) 2013 Active. All rights reserved.
 *----
 * Utils for file ANotify API.
 * 
 */

#ifndef __ACTIVE_NOTIFY_UTILS_H__
#define __ACTIVE_NOTIFY_UTILS_H__

#include <string>
#include <deque>
#include <map>
#include <sys/inotify.h>


/// Event struct size
#define INOTIFY_EVENT_SIZE (sizeof(struct inotify_event))

/// Event buffer length
#define INOTIFY_BUFLEN (1024 * (INOTIFY_EVENT_SIZE + 16))

/// Helper macro for creating exception messages.
/**
 * It prepends the message by the function name.
 */
#define IN_EXC_MSG(msg) (std::string(__PRETTY_FUNCTION__) + ": " + msg)

// Capacités limite du systeme de notification
typedef enum{
    IN_MAX_EVENTS     = 0,  ///< max. events in the kernel queue
    IN_MAX_INSTANCES  = 1,  ///< max. inotify file descriptors per process
    IN_MAX_WATCHES    = 2   ///< max. watches per file descriptor
  } ANCapability;

#ifdef INOTIFY_THREAD_SAFE

#include <pthread.h>

#define IN_LOCK_DECL mutable pthread_rwlock_t __m_lock;

#define IN_LOCK_INIT \
  { \
    pthread_rwlockattr_t attr; \
    int res = 0; \
    if ((res = pthread_rwlockattr_init(&attr)) != 0) \
      throw InotifyException(IN_EXC_MSG("cannot initialize lock attributes"), res, this); \
    if ((res = pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NP)) != 0) \
      throw InotifyException(IN_EXC_MSG("cannot set lock kind"), res, this); \
    if ((res = pthread_rwlock_init(&__m_lock, &attr)) != 0) \
      throw InotifyException(IN_EXC_MSG("cannot initialize lock"), res, this); \
    pthread_rwlockattr_destroy(&attr); \
  }
 
#define IN_LOCK_DONE pthread_rwlock_destroy(&__m_lock);

#define IN_READ_BEGIN \
  { \
    int res = pthread_rwlock_rdlock(&__m_lock); \
    if (res != 0) \
      throw InotifyException(IN_EXC_MSG("locking for reading failed"), res, (void*) this); \
  }
  
#define IN_READ_END \
  { \
    int res = pthread_rwlock_unlock(&__m_lock); \
    if (res != 0) \
      throw InotifyException(IN_EXC_MSG("unlocking failed"), res, (void*) this); \
  }
  
#define IN_READ_END_NOTHROW pthread_rwlock_unlock(&__m_lock);
  
#define IN_WRITE_BEGIN \
  { \
    int res = pthread_rwlock_wrlock(&__m_lock); \
    if (res != 0) \
      throw InotifyException(IN_EXC_MSG("locking for writing failed"), res, (void*) this); \
  }
  
#define IN_WRITE_END IN_READ_END
#define IN_WRITE_END_NOTHROW IN_READ_END_NOTHROW

#else // INOTIFY_THREAD_SAFE

#define IN_LOCK_DECL
#define IN_LOCK_INIT
#define IN_LOCK_DONE
#define IN_READ_BEGIN
#define IN_READ_END
#define IN_READ_END_NOTHROW
#define IN_WRITE_BEGIN
#define IN_WRITE_END
#define IN_WRITE_END_NOTHROW

#endif // INOTIFY_THREAD_SAFE


// Map by file descriptor
typedef std::map<unsigned int, InotifyWatch*> WatchFDMap;
// Map by path 
typedef std::map<std::string, InotifyWatch*> WatchPathMap;


