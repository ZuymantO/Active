//
//  ANotifyWatch.h
//  ANotify
//
//  Created by J. Charles N. MBIADA on 17/03/13.
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

#ifndef __ANotify__ANotifyWatch__
#define __ANotify__ANotifyWatch__

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "ANotify.h"
#include "ANotifyEvent.h"


class ANotify;
//class ANotifyEvent;

class ANotifyWatch
{
  
private:
  std::string m_path;   // watched file path string
  ANotifyEvent* m_pAEvent;     // Anotify event object
  ANotify* m_pANotify;  // ANotify object (parent of watched file)
  FD m_wDescr;         // watch descriptor (duplicated in the Anotify Event Object)
  bool m_wEnabled;      // activate or desactivated event on that file
  bool m_isDir;      // is directory permit some behavior when is a directoriy. behavior like recurssivity (not yet implemented)
  std::vector<std::string> m_files ; // all the file of the directory. vector file for detect added files on UNIX Based system
  std::vector<ANotifyWatch*> m_watchedFiles; // watched files for recursivity mode
  IN_LOCK_DECL
  
public:
  ANotifyWatch(const std::string& iPath, ANotifyEvent* ipAEvt, bool iisDir = false, bool iwEnabled = true) throw (ANotifyException)
  : m_path(iPath), m_pAEvent(ipAEvt), m_wDescr((FD) -1), m_wEnabled(iwEnabled), m_pANotify(NULL), m_isDir(iisDir)  {
    IN_LOCK_INIT
    initWatch();
  }
  ANotifyWatch(const std::string& iPath, ANotifyEvent* ipAEvt, ANotify* ipANotify = NULL, bool iisDir = false, bool iwEnabled = true)  throw (ANotifyException)
  : m_path(iPath), m_pAEvent(ipAEvt), m_wDescr((FD) -1), m_wEnabled(iwEnabled), m_pANotify(ipANotify), m_isDir(iisDir) {
    IN_LOCK_INIT
    initWatch();
  }  
  ~ANotifyWatch(){
    IN_LOCK_DONE
  }
  inline FD getDescriptor() const{
    return m_wDescr;
  }
  inline const std::string& getPath() const{
    return m_path;
  }

  ANMask getMask() const{
    if(m_pAEvent == NULL)
      return ANOTIFY_NONE;
    else
    return m_pAEvent->getMask() ;
  }
  
  void initWatch() throw (ANotifyException) {
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    
#else
    if (m_wEnabled) {
      m_wDescr = open(m_path.c_str() , (isDir() ? O_DIRECTORY : O_RDWR));
      if (m_wDescr == -1) {
        throw ANotifyException(IN_EXC_MSG("Unable to open file to watch"), errno, NULL);
      }
    }
#endif // __ENVIREONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__

  
  }
  
    // relacher la structure apres desactivation
  void releaseWatch()  throw (ANotifyException){
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    
#else
    if (m_wDescr > 0) {
      if (close(m_wDescr) == -1 ) {
        throw ANotifyException(IN_EXC_MSG("Unable to close file descriptor of watched file"), errno, NULL);
      }
    }
    
#endif // __ENVIREONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    
  }
  
  void setMask(ANMask iuMask) throw (ANotifyException);
  inline ANotify* getANotify(){
    return m_pANotify;
  }
  void setEnabled(bool ifEnabled) throw (ANotifyException);
  inline void enable() throw (ANotifyException) {
    setEnabled(true);
  }
  inline void disable() throw (ANotifyException) {
    IN_WRITE_BEGIN
    
    if (!m_wEnabled) {
      IN_WRITE_END_NOTHROW
      throw ANotifyException(IN_EXC_MSG("event cannot occur on disabled watch"), EINVAL, this);
    }
    setEnabled(false);

    /* TODO: IN_WRITE_END manquant ? */
    IN_WRITE_END
  }
  inline ANotifyEvent* getAEvent() const{
    return m_pAEvent;
  }

  inline bool isEnabled() const{
    return m_wEnabled;
  }

  inline bool isDir(){
    return m_isDir;
  }
  void setAsDir(){
    m_isDir = true;
  }
  void setAsFile(){
    m_isDir = false;
  }
  void setMonitor(ANotify* ipANot){
    m_pANotify = ipANot;
  }
  void setMonitor(ANotify& irANot){
    setMonitor(&irANot);
  }
  void setDescriptor(FD iFD){
    m_wDescr = iFD;
  }
  void setPath(std::string *str){
    m_path = std::string(str->c_str());
  }
  void setPath(std::string &str){
    setPath(&str);
  }

  inline bool isRecursive() const{
    return false;
  }
  
  void __disable();
};


#endif /* defined(__ANotify__ANotifyWatch__) */
