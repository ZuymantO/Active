//
//  ANotifyUtils.h
//  ANotify
//
//  Created by J. Charles N. MBIADA on 12/03/13.
//  Copyright (c) 2013 Active All rights reserved.
//

#include <iostream>
#include <sys/types.h>
  /// procfs inotify base path, pour la configuration chemin 'a choisir encore et bien d'efinir
#define PROCFS_ANOTIFY_BASE "/proc/sys/fs/inotify/" // mettre path pour unix



/*#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#include <sys/inotify.h>
  /// procfs inotify base path 
#define PROCFS_ANOTIFY_BASE "/proc/sys/fs/inotify/"
#endif // __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#include <sys/event.h>*/
/** <MOD> **/
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
   #include <sys/inotify.h>
   #include <sys/eventfd.h>
   /// procfs inotify base path 
   #define PROCFS_ANOTIFY_BASE "/proc/sys/fs/inotify/"
#else
   #include <sys/event.h>
#endif // __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__

#define IN_EXC_MSG(msg) (std::string(__PRETTY_FUNCTION__) + ": " + msg)
/** </MOD> **/

/* TODO: commenter cette ligne */
#define INOTIFY_THREAD_SAFE

#ifdef INOTIFY_THREAD_SAFE
   #include <pthread.h>

   #define IN_LOCK_DECL mutable pthread_rwlock_t __m_lock;
   #define IN_LOCK_INIT \
   { \
   pthread_rwlockattr_t attr; \
   int res = 0; \
   if ((res = pthread_rwlockattr_init(&attr)) != 0) \
   throw ANotifyException(IN_EXC_MSG("cannot initialize lock attributes"), res, this); \
   if ((res = pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NP)) != 0) \
   throw ANotifyException(IN_EXC_MSG("cannot set lock kind"), res, this); \
   if ((res = pthread_rwlock_init(&__m_lock, &attr)) != 0) \
   throw ANotifyException(IN_EXC_MSG("cannot initialize lock"), res, this); \
   pthread_rwlockattr_destroy(&attr); \
   }

   #define IN_LOCK_DONE pthread_rwlock_destroy(&__m_lock);

   #define IN_READ_BEGIN \
   { \
   int res = pthread_rwlock_rdlock(&__m_lock); \
   if (res != 0) \
   throw ANotifyException(IN_EXC_MSG("locking for reading failed"), res, (void*) this); \
   }

   #define IN_READ_END \
   { \
   int res = pthread_rwlock_unlock(&__m_lock); \
   if (res != 0) \
   throw ANotifyException(IN_EXC_MSG("unlocking failed"), res, (void*) this); \
   }

   #define IN_READ_END_NOTHROW pthread_rwlock_unlock(&__m_lock);

   #define IN_WRITE_BEGIN \
   { \
   int res = pthread_rwlock_wrlock(&__m_lock); \
   if (res != 0) \
   throw ANotifyException(IN_EXC_MSG("locking for writing failed"), res, (void*) this); \
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


/* <MOD> */
/*#define ANOTIFY_INIT() (kqueue())
#define ANOTIFY_EVENT struct kevent*/

#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
  #define ANOTIFY_INIT() inotify_init()
  #define ANOTIFY_EVENT struct inotify_event
#else
  #define ANOTIFY_INIT() (kqueue())
  #define ANOTIFY_EVENT struct kevent
#endif
/* </MOD> */


#define ANMASK uint16_t // mask
#define FD int32_t  // file descriptor

#define ANEVENTID unsigned long
#define ANFILTER uint16_t
#define ANDATA long
#define ANOPAQUEDATE void*

#ifdef __APPLE__
   #ifdef TARGET_OS_MAC
      #define ANOTIFY_INIT() kqueue() //kqueue etc.
      #define ANOTIFY_EVENT struct kevent
      #define ANMASK uint16_t // mask
   #endif
#elif defined _WIN32 || defined _WIN64 // For fun, on touche pas a windows...
   #define ANOTIFY_INIT()
   #define ANOTIFY_EVENT
#elif defined LINUX // struct inotify_event,
   #define ANOTIFY_INIT() inotify_init()
   #define ANOTIFY_EVENT struct inotify_event
   #define ANMASK uint32_t // mask
#endif


#define ANOTIFY_EVENT_SIZE (sizeof(ANOTIFY_EVENT))
#define ANOTIFY_BUFF_LENGTH (1024 * (ANOTIFY_EVENT_SIZE + 16))
#define FD int32_t  // file descriptor


