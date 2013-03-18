/*=======
 * ANotify.hpp
 *=======
 * Created by J. Charles N. MBIADA <jcharles@zuymanto.com> on 07/02/13.
 * Copyright (c) 2013 Active. All rights reserved.
 *----
 * Utils for file monitoring system. To don't care about targeted OS.
 * 
 */
#ifndef __ACTIVE_NOTIFY_H__
#define __ACTIVE_NOTIFY_H__
#include <string>
#include <deque>
#include <map>
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#include <sys/inotify.h>
#endif // __ENVIREONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#include "ANotifyException.hpp"
#include "ANotifyEvent.hpp"
#include "ANotyWatch.hpp"
#include "utils/ANotifyUtils.hpp"

/**
 * It holds information about the inotify device descriptor
 * and manages the event queue.
 * 
 * If the INOTIFY_THREAD_SAFE is defined this class is thread-safe.
 */
class ANotify
{

 private:
  friend class ANotifyWatch;  
  int m_fd;                             ///< file descriptor
  WatchFDMap m_watches;               ///< watches (by descriptors)
  WatchPathMap m_paths;                    ///< watches (by paths)
  unsigned char m_buf[INOTIFY_BUFLEN];  ///< buffer for events
  std::deque<ANotifyEvent> m_events;    ///< event queue
  IN_LOCK_DECL
  
  static std::string GetCapabilityPath(InotifyCapability_t cap) throw (ANotifyException);

 public:
 
  /**
   * Créer et ouvrir une communication inotify
   * 
   * Si inotify n'existe pas ANotifyException est propagé
   */
  ANotify() throw (ANotifyException);
  
  /// Destructor.
  /**
   * Calls Close() due to clean-up.
   */
  ~ANotify();
  
  /// Removes all watches and closes the inotify device.
  void Close();
    
  /// Adds a new watch.
  /**
   * \param[in] pWatch inotify watch
   * 
   * \throw InotifyException thrown if adding failed
   */
  void Add(ANotifyWatch* pWatch) throw (ANotifyException);
  
  /// Adds a new watch.
  /**
   * \param[in] rWatch inotify watch
   * 
   * \throw InotifyException thrown if adding failed
   */
  inline void Add(ANotifyWatch& rWatch) throw (ANotifyException)
  {
    Add(&rWatch);
  }
  
  /// Removes a watch.
  /**
   * If the given watch is not present it does nothing.
   * 
   * \param[in] pWatch inotify watch
   * 
   * \throw InotifyException thrown if removing failed
   */
  void Remove(ANotifyWatch* pWatch) throw (ANotifyException);
  
  /// Removes a watch.
  /**
   * If the given watch is not present it does nothing.
   * 
   * \param[in] rWatch inotify watch
   * 
   * \throw InotifyException thrown if removing failed
   */
  inline void Remove(ANotifyWatch& rWatch) throw (ANotifyException)
  {
    Remove(&rWatch);
  }
  
  /// Removes all watches.
  void RemoveAll();
  
  /// Returns the count of watches.
  /**
   * This is the total count of all watches (regardless whether
   * enabled or not).
   * 
   * \return count of watches
   * 
   * \sa GetEnabledCount()
   */
  inline size_t GetWatchCount() const
  {
    IN_READ_BEGIN
      size_t n = (size_t) m_paths.size();
    IN_READ_END
      return n;
  }
  
  /// Returns the count of enabled watches.
  /**
   * \return count of enabled watches
   * 
   * \sa GetWatchCount()
   */  
  inline size_t GetEnabledCount() const
  {
    IN_READ_BEGIN
      size_t n = (size_t) m_watches.size();
    IN_READ_END
      return n;
  }
  
  /// Waits for inotify events.
  /**
   * It waits until one or more events occur. When called
   * in nonblocking mode it only retrieves occurred events
   * to the internal queue and exits.
   * 
   * \param[in] fNoIntr if true it re-calls the system call after a handled signal
   * 
   * \throw InotifyException thrown if reading events failed
   * 
   * \sa SetNonBlock()
   */
  void WaitForEvents(bool fNoIntr = false) throw (ANotifyException);
  
  /// Returns the count of received and queued events.
  /**
   * This number is related to the events in the queue inside
   * this object, not to the events pending in the kernel.
   * 
   * \return count of events
   */
  inline size_t GetEventCount()
  {
    IN_READ_BEGIN
      size_t n = (size_t) m_events.size();
    IN_READ_END
      return n;
  }
  
  /// Extracts a queued inotify event.
  /**
   * The extracted event is removed from the queue.
   * If the pointer is NULL it does nothing.
   * 
   * \param[in,out] pEvt event object
   * 
   * \throw InotifyException thrown if the provided pointer is NULL
   */
  bool GetEvent(InotifyEvent* pEvt) throw (ANotifyException);
  
  /// Extracts a queued inotify event.
  /**
   * The extracted event is removed from the queue.
   * 
   * \param[in,out] rEvt event object
   * 
   * \throw InotifyException thrown only in very anomalous cases
   */
  bool GetEvent(ANotifyEvent& rEvt) throw (ANotifyException)
  {
    return GetEvent(&rEvt);
  }
  
  /// Extracts a queued inotify event (without removing).
  /**
   * The extracted event stays in the queue.
   * If the pointer is NULL it does nothing.
   * 
   * \param[in,out] pEvt event object
   * 
   * \throw InotifyException thrown if the provided pointer is NULL
   */
  bool PeekEvent(ANotifyEvent* pEvt) throw (ANotifyException);
  
  /// Extracts a queued inotify event (without removing).
  /**
   * The extracted event stays in the queue.
   * 
   * \param[in,out] rEvt event object
   * 
   * \throw InotifyException thrown only in very anomalous cases
   */
  bool PeekEvent(ANotifyEvent& rEvt) throw (ANotifyException)
  {
    return PeekEvent(&rEvt);
  }
  
  /// Searches for a watch by a watch descriptor.
  /**
   * It tries to find a watch by the given descriptor.
   * 
   * \param[in] iDescriptor watch descriptor
   * \return pointer to a watch; NULL if no such watch exists
   */
  InotifyWatch* FindWatch(int iDescriptor);
  
  /// Searches for a watch by a filesystem path.
  /**
   * It tries to find a watch by the given filesystem path.
   * 
   * \param[in] rPath filesystem path
   * \return pointer to a watch; NULL if no such watch exists
   * 
   * \attention The path must be exactly identical to the one
   *            used for the searched watch. Be careful about
   *            absolute/relative and case-insensitive paths.
   */
  InotifyWatch* FindWatch(const std::string& rPath);
  
  /// Returns the file descriptor.
  /**
   * The descriptor can be used in standard low-level file
   * functions (poll(), select(), fcntl() etc.).
   * 
   * \return valid file descriptor or -1 for inactive object
   * 
   * \sa SetNonBlock()
   */
  inline int GetDescriptor() const
  {
    return m_fd;
  }
  
  /// Enables/disables non-blocking mode.
  /**
   * Use this mode if you want to monitor the descriptor
   * (acquired thru GetDescriptor()) in functions such as
   * poll(), select() etc.
   * 
   * Non-blocking mode is disabled by default.
   * 
   * \param[in] fNonBlock enable/disable non-blocking mode
   * 
   * \throw InotifyException thrown if setting mode failed
   * 
   * \sa GetDescriptor(), SetCloseOnExec()
   */
  void SetNonBlock(bool fNonBlock) throw (ANotifyException);
  
  /// Enables/disables closing on exec.
  /**
   * Enable this if you want to close the descriptor when
   * executing another program. Otherwise, the descriptor
   * will be inherited.
   * 
   * Closing on exec is disabled by default.
   * 
   * \param[in] fClOnEx enable/disable closing on exec
   * 
   * \throw InotifyException thrown if setting failed
   * 
   * \sa GetDescriptor(), SetNonBlock()
   */
  void SetCloseOnExec(bool fClOnEx) throw (ANotifyException);
  
  /// Acquires a particular inotify capability/limit.
  /**
   * \param[in] cap capability/limit identifier
   * \return capability/limit value
   * \throw InotifyException thrown if the given value cannot be acquired
   */
  static uint32_t GetCapability(ANotifyCapability_t cap) throw (ANotifyException);
  
  /// Modifies a particular inotify capability/limit.
  /**
   * \param[in] cap capability/limit identifier
   * \param[in] val new capability/limit value
   * \throw InotifyException thrown if the given value cannot be set
   * \attention Using this function requires root privileges.
   *            Beware of setting extensive values - it may seriously
   *            affect system performance and/or stability.
   */
  static void SetCapability(InotifyCapability_t cap, uint32_t val) throw (ANotifyException);
  
  /// Returns the maximum number of events in the kernel queue.
  /**
   * \return maximum number of events in the kernel queue
   * \throw InotifyException thrown if the given value cannot be acquired
   */
  inline static uint32_t GetMaxEvents() throw (ANotifyException)
  {
    return GetCapability(IN_MAX_EVENTS);
  }
  
  /// Sets the maximum number of events in the kernel queue.
  /**
   * \param[in] val new value
   * \throw InotifyException thrown if the given value cannot be set
   * \attention Using this function requires root privileges.
   *            Beware of setting extensive values - the greater value
   *            is set here the more physical memory may be used for the inotify
   *            infrastructure.
   */
  inline static void SetMaxEvents(uint32_t val) throw (ANotifyException)
  {
    SetCapability(IN_MAX_EVENTS, val);
  }
  
  /// Returns the maximum number of inotify instances per process.
  /**
   * It means the maximum number of open inotify file descriptors
   * per running process.
   * 
   * \return maximum number of inotify instances
   * \throw InotifyException thrown if the given value cannot be acquired
   */
  inline static uint32_t GetMaxInstances() throw (ANotifyException)
  {
    return GetCapability(IN_MAX_INSTANCES);
  }
  
  /// Sets the maximum number of inotify instances per process.
  /**
   * \param[in] val new value
   * \throw InotifyException thrown if the given value cannot be set
   * \attention Using this function requires root privileges.
   *            Beware of setting extensive values - the greater value
   *            is set here the more physical memory may be used for the inotify
   *            infrastructure.
   */
  inline static void SetMaxInstances(uint32_t val) throw (ANotifyException)
  {
    SetCapability(IN_MAX_INSTANCES, val);
  }
  
  /// Returns the maximum number of inotify watches per instance.
  /**
   * It means the maximum number of inotify watches per inotify
   * file descriptor.
   * 
   * \return maximum number of inotify watches
   * \throw InotifyException thrown if the given value cannot be acquired
   */
  inline static uint32_t GetMaxWatches() throw (ANotifyException)
  {
    return GetCapability(IN_MAX_WATCHES);
  }
  
  /// Sets the maximum number of inotify watches per instance.
  /**
   * \param[in] val new value
   * \throw InotifyException thrown if the given value cannot be set
   * \attention Using this function requires root privileges.
   *            Beware of setting extensive values - the greater value
   *            is set here the more physical memory may be used for the inotify
   *            infrastructure.
   */
  inline static void SetMaxWatches(uint32_t val) throw (ANotifyException)
  {
    SetCapability(IN_MAX_WATCHES, val);
  }


};






#endif // __ACTIVE_NOTIFY_H__


