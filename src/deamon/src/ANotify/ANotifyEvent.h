//
//  ANotifyEvent.h
//  ANotify
//
//  Created by J. Charles N. MBIADA on 12/03/13.
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


#ifndef __ANotify__ANotifyEvent__
#define __ANotify__ANotifyEvent__

#include <iostream>
#include "ANotifyUtils.h"
#include "ANotifyMask.h"
#include "ANotifyException.h"
class ANotifyWatch;

class ANotifyEvent
{
  
private:
    //  ANotifyWatch* m_pANWatch;       // Contient le FD de inotify sous linux kq de kqueu sous unix
  ANOTIFY_EVENT*  m_pEvent;
  FD              m_evtDescr;     /* Identifiant de l'auditeur */
  std::string     m_name;         //* nom fichier source d'event (optionnel et terminé par un caractère nul) */
  ANMASK          m_mask;
  ANotifyWatch* m_pWatch;
  
public:
  ANotifyEvent() : m_evtDescr((FD)0), m_pEvent(NULL), m_name(""), m_pWatch(NULL){ }
  
  ANotifyEvent(ANOTIFY_EVENT* ipEvt, ANMask imask, ANotifyWatch* ipWatch = NULL)
  : m_pEvent(ipEvt),  m_name(""), m_evtDescr((FD)-1), m_mask(imask), m_pWatch(ipWatch){
    m_pEvent = ipEvt;
    if (ipEvt != NULL) {
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    m_evtDescr = ipEvt->wd; // fd of concerned file
    if (ipEvt->name != NULL)
      m_name = (ipEvt->length > 0 ? std::string(ipEvt->name) : "");
#else
      m_evtDescr =  (FD) ipEvt->ident;
#endif // __ENVIREONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    }
  }
  
  ~ANotifyEvent() {}
  
  FD getDescriptor() const {
    return  m_evtDescr;
  }
  
  inline ANOTIFY_EVENT* getEvent() const{
    return m_pEvent ;
  }
  
  inline ANMASK getMask() const{
    return m_mask;
  }
  
  
  inline ANMASK getEventMask() const{
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    return (ANMASK) m_pEvent->mask;
#else
    return (ANMASK) m_pEvent->fflags;
#endif
  }
  
  inline  void setMask(ANMask imask){
    m_mask = imask;
  }
  
  inline ANEVENTID getIdent() const{
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    return m_pEvent->ident;
#else
    return (ANEVENTID)0;
#endif
  }
  
  inline ANFILTER getFilter() const{
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    return m_pEvent->filter;
#else
    return (ANFILTER)0;
#endif
  }
  
  inline ANMask getFlags() const{
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    return (ANMask) 0;
#else
    return (ANMASK) m_pEvent->flags;
#endif
  }
  
  inline ANMask getFFlags() const{
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    return (ANMask)0;
#else
    return (ANMASK) m_pEvent->fflags;
#endif
  }
  
  inline ANDATA getData() const{
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    return (ANDATA) 0;
#else
    return (ANDATA) m_pEvent->data;
#endif
  }
  
  inline void* getUData() const{
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    return NULL;
#else
    return m_pEvent->udata;
#endif
  }

  inline bool isType(ANMask iuType) const{
    return ANotifyMask::isType(getMask(), iuType);
  }

  inline ANMask getCookie() const{
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    return m_pEvent->cookie;
#else
    return (ANMask)0;
#endif
  }
  
  inline int getLength() const{
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    return (int) m_name.length();
#else
    return (ANMask)0;
#endif
  }
  
  inline const std::string& getName() const {
    return m_name;
  }
  inline void getName(std::string& orName) const {
    orName = "";
    orName.append(getName());
  }


  void dumpTypes(std::string& orStr) const {
    if (m_pEvent == NULL) {
      orStr = "";
      return ;
    }
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    ANotifyMask::dumpTypes(m_pEvent->mask, &orStr);
#else
    ANotifyMask::dumpTypes(m_pEvent->fflags, &orStr);
#endif
  }
  
};

#endif /* defined(__ANotify__ANotifyEvent__) */
