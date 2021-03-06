//
//  ANotify.cpp
//  ANotify
//
//  Created by J. Charles N. MBIADA on 17/03/13.
//  Copyright (c) 2013 Active. All rights reserved.
//  Utils for file monitoring system. Don't care what os is launching (linux/unix only)
// TODO 1. METTRE TOUT LES COMS EN ANGLAIS APRES LECTURE PAR TOUT LE MONDE
// TODO 2. ACTIVER LA LECTURE ET INSERTION DANS LE FILE DES EVENTS PAR UN LOCK SUR (SUR UNIX surtout)
/*********************************************************************
 * Fichier singleton permettant de monitoriser les fichiers system
 * On ne force pas toute fois qu'il n'y ait qu'une instance de ANotify ('a d'edicder plus tard)
 * Un Active Notify a une liste des fichiers monotoris'es
 * Une liste d'evenement captur'e et permet donc de mettre ces elements en relation.
 *      /// Voir le fichier exemple.c pour un cas d'utilisation rapide
 * Utilise ANotifyEvent, ANotifyMask, ANotifyUtils, ANotifyWatch et ANotifyException
 **********************************************************************/

#include <iostream>
#include <sstream>
#include <string.h>
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#include <sys/inotify.h>
#endif // __ENVIREONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#include "ANotifyException.h"
#include "ANotifyEvent.h"
#include "ANotifyWatch.h"
#include "ANotifyUtils.h"
#include "ANotifyMask.h"
#include "ANotify.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

ANotify::ANotify() throw (ANotifyException)
{
  IN_LOCK_INIT
  
    m_fileDescr = ANOTIFY_INIT();
  if (m_fileDescr == -1) {
    IN_LOCK_DONE
      throw ANotifyException(IN_EXC_MSG("ANotify init failed"), errno, NULL);
  }
}

ANotify::~ANotify()
{
  AClose();
  IN_LOCK_DONE
    }

void ANotify::AClose()
{
  IN_WRITE_BEGIN
  
    if (m_fileDescr != -1) {
      removeAll();
      if(close(m_fileDescr) < 0){
	IN_WRITE_END_NOTHROW
	  throw ANotifyException(IN_EXC_MSG("Error on inotify descriptor close"), EBUSY, this);
      }
      m_fileDescr = -1;
    }
  
  IN_WRITE_END
    }

void ANotify::Close(){  
  IN_WRITE_BEGIN  
    if (m_fileDescr != -1) {
      removeAll();
    }  
  IN_WRITE_END
}

void ANotify::add(ANotifyWatch* pWatch) throw (ANotifyException)
{
  //IN_WRITE_BEGIN

    // invalid descriptor - this case shouldn't occur - go away
    if (m_fileDescr == -1) {
      //IN_WRITE_END_NOTHROW
	throw ANotifyException(IN_EXC_MSG("invalid file descriptor"), EBUSY, this);
    }
  
  // this path already watched - go away
  if (findWatch(pWatch->getPath()) != NULL) {
    //IN_WRITE_END_NOTHROW
      throw ANotifyException(IN_EXC_MSG("path already watched"), EBUSY, this);
  }
  
  // for enabled watch
  if (pWatch->isEnabled()) {
    
    // try to add watch to kernel
    FD wd = -1;//
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    wd = inotify_add_watch(m_fileDescr, pWatch->getPath().c_str(), pWatch->getMask());
#else
    EV_SET((pWatch->getAEvent()->getEvent()), pWatch->getDescriptor() , EVFILT_VNODE, EV_ADD | EV_ENABLE | EV_CLEAR, pWatch->getMask(), 0, 0);;
    kevent(m_fileDescr, pWatch->getAEvent()->getEvent(), 1, NULL, 0, NULL);
    wd = pWatch->getDescriptor();
#endif

    
    // adding failed - go away
    if (wd == -1) {
      //IN_WRITE_END_NOTHROW
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
	throw ANotifyException(IN_EXC_MSG("adding watch failed"), errno, this);
#else
      throw ANotifyException(IN_EXC_MSG("adding watch failed"), errno, this);
#endif
    }
    
    // this path already watched (but defined another way)
    ANotifyWatch* pW = findWatch(wd);
    if (pW != NULL) {
      //      
      //        // try to recover old watch because it may be modified - then go away
      //      if (0){//inotify_add_watch(m_fileDescr, pW->getPath().c_str(), pW->getMask()) < 0) {
      //        IN_WRITE_END_NOTHROW
      //        throw ANotifyException(IN_EXC_MSG("watch collision detected and recovery failed"), errno, this);
      //      }
      //      else {
      //          // recovery failed - go away
      //        IN_WRITE_END_NOTHROW
      //        throw ANotifyException(IN_EXC_MSG("path already watched (but defined another way)"), EBUSY, this);
      //      }
    }
    
    pWatch->setDescriptor(wd);
    IN_WATCH_WRITE_BEGIN
      //m_watches.insert(std::pair<FD, ANotifyWatch*>(pWatch->getDescriptor(), pWatch));
      m_watches.insert(std::pair<FD, ANotifyWatch*>(wd, pWatch));
    IN_WATCH_WRITE_END

      }
  
  IN_WATCH_WRITE_BEGIN
    m_paths.insert(std::pair<std::string, ANotifyWatch*>(pWatch->getPath(), pWatch)); // paths of non enabled and enabled files
  IN_WATCH_WRITE_END
    pWatch->setMonitor(this);
  
  //IN_WRITE_END
    }

bool ANotify::remove(ANotifyWatch* pWatch) throw (ANotifyException)
{
  if(pWatch == NULL){
    return false;
  }

  IN_WATCH_WRITE_BEGIN
    // invalid descriptor - this case shouldn't occur - go away
    if (m_fileDescr == -1) {
      IN_WATCH_WRITE_END_NOTHROW
	/*throw ANotifyException(IN_EXC_MSG("invalid file descriptor"), EBUSY, this);*/
	return false;
    }
  
  // for enabled watch
  if (pWatch->getDescriptor() != -1) {    
    // removing watch failed - go away
    /* TODO: quand est changee la valeur sous Mac ? */
    //FD wd = -1;
    FD wd = 0;
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
    //wd = inotify_rm_watch(m_fileDescr, pWatch->m_wDescr);
    wd = inotify_rm_watch(m_fileDescr, pWatch->getDescriptor());
#else
    EV_SET((pWatch->getAEvent()->getEvent()), pWatch->getDescriptor() , EVFILT_VNODE, EV_DELETE | EV_DISABLE | EV_CLEAR, pWatch->getMask(), 0, 0);;
#endif

    if (wd == -1){
      IN_WATCH_WRITE_END_NOTHROW
	//throw ANotifyException(IN_EXC_MSG("removing watch failed"), errno, this);
	/*throw ANotifyException(IN_EXC_MSG("removing watch failed [" + ss.str() + "]"), errno, this);*/
      /* TODO: decommenter plus haut si besoin */
      return false;
    }
    m_watches.erase(pWatch->getDescriptor());
    pWatch->setDescriptor(-1);
  }
  
  m_paths.erase(pWatch->getPath());
  pWatch->setMonitor(NULL) ;
  
  IN_WATCH_WRITE_END
    return true;
}

bool ANotify::removeOneWatch(std::string& path){
  IN_WATCH_WRITE_BEGIN

  ANotifyWatch* pW;
  bool res;

  pW = findWatchWOLock(path);
  
  if(pW != NULL){
    clearWatch(pW);
    res = true;
  }
  else{
    res = false;
  }

  IN_WATCH_WRITE_END

  return res;
}

bool ANotify::removeRecWatch(std::string& path){
  IN_WATCH_WRITE_BEGIN

  WatchPathMap::iterator it = m_paths.begin();
  std::string filepath;
  int filepathLength;
  std::vector<std::string> vectPaths;
  std::vector<std::string>::iterator vectIt;
  ANotifyWatch* pWatch;
  bool res = true;
  
  while(it != m_paths.end()){
    filepath = it->first;
    filepathLength = filepath.length();
    
    if(strncmp(filepath.c_str(), path.c_str(), path.length()) == 0){
      vectPaths.push_back(filepath);
    }
    
    it++;
  }
  
  vectIt = vectPaths.begin();

  while(vectIt != vectPaths.end()){
    filepath = *vectIt;
    pWatch = findWatchWOLock(filepath);
    
    if(pWatch == NULL){
      res = false;
    }
    else{
      res = res && clearWatch(pWatch);
    }   

    vectIt++;
  }


  IN_WATCH_WRITE_END  
    
    return true;
}

bool ANotify::remove(std::string& path, bool rec){
    ANotifyWatch* pWatch;
    bool res;

    if(!rec){
      res = removeOneWatch(path);
    }
    else{
      res = removeRecWatch(path);
    }

    return res;
}

bool ANotify::clearWatch(ANotifyWatch* pWatch) throw (ANotifyException){
   if(pWatch == NULL){
     return false;
   }
   
   // invalid descriptor - this case shouldn't occur - go away
   if (m_fileDescr == -1) {
    return false;
    }
  
   // for enabled watch
   if (pWatch->getDescriptor() != -1) {    
     // removing watch failed - go away
     /* TODO: quand est changee la valeur sous Mac ? */
     FD wd = -1;
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
     wd = inotify_rm_watch(m_fileDescr, pWatch->getDescriptor());
#else
     EV_SET((pWatch->getAEvent()->getEvent()), pWatch->getDescriptor() , EVFILT_VNODE, EV_DELETE | EV_DISABLE | EV_CLEAR, pWatch->getMask(), 0, 0);;
#endif
     
     if (wd == -1){
      return false;
    }
    m_watches.erase(pWatch->getDescriptor());
    pWatch->setDescriptor(-1);
  }
  
  m_paths.erase(pWatch->getPath());
  pWatch->setMonitor(NULL);
  return true;
}

bool ANotify::removeAll() throw (ANotifyException)
{
  /* TODO: enlever les locks si besoin est */
  //IN_WATCH_READ_BEGIN
  IN_WATCH_WRITE_BEGIN

  bool b = true;
  WatchPathMap::iterator it = m_paths.begin();
  while (it != m_paths.end()) {
    ANotifyWatch* pW = (*it).second;
    //if(!b || remove(pW))
    b = b && clearWatch(pW);
    //if(!b || !remove(pW))
      b = false;
    it++;
  }
  
  //IN_WATCH_WRITE_BEGIN

  m_watches.clear();
  m_paths.clear();

  IN_WATCH_WRITE_END
      
  return b;
}

void ANotify::waitForEvents(bool fNoIntr) throw (ANotifyException)
{
  
#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__

  ssize_t len = 0 ;
  do {
    len = read(m_fileDescr, m_buf, ANOTIFY_BUFF_LENGTH);
  } while (fNoIntr && len == -1 && errno == EINTR);
  
  if (len == -1 && !(errno == EWOULDBLOCK || errno == EINTR))
    throw ANotifyException(IN_EXC_MSG("reading events failed"), errno, this);
  
  if (len == -1)
    return;
  
  IN_EVENT_WRITE_BEGIN
  
    ssize_t i = 0;
  while (i < len) {
    ANOTIFY_EVENT* pEvt = (ANOTIFY_EVENT*) &m_buf[i];
    ANotifyWatch* pW = findWatch((FD)pEvt->wd);
    if (pW != NULL) {
      /* TODO: trouver le bon mask en argument */
      //ANotifyEvent evt(pEvt, pW->getMask(), pW);
      //ANotifyEvent* evt = new ANotifyEvent(pEvt, pW->getMask(), pW);
      ANotifyEvent* evt = new ANotifyEvent(pEvt, (ANMask)pEvt->mask, pW->getPath());
      /*if (    ANotifyEvent::isType(pW->getMask(), IN_ANOTIFY_ONESHOT)
	||  ANotifyEvent::isType(evt.getMask(), IN_ANOTIFY_IGNORED))*/
      /*if (    ANotifyMask::isType(pW->getMask(), ANOTIFY_ONESHOT)
	||  ANotifyMask::isType(evt.getMask(), ANOTIFY_IGNORED))
        pW->__disable();*/
      m_events.push_back(evt);
    }
    i += ANOTIFY_EVENT_SIZE + (ssize_t) pEvt->len;
  }

#else

  int n = -1;
  for (;;) {
    ANOTIFY_EVENT* pEvt = (ANOTIFY_EVENT*) malloc(ANOTIFY_EVENT_SIZE);
    ANotifyWatch* pW;
    n = kevent(m_fileDescr, NULL, 0,  pEvt, 1, NULL); // Lecture d'un event
    if (n > 0) {
      pW = findWatch((FD)pEvt->ident);
      if(pW != NULL){
        //ANotifyEvent* aevt = new ANotifyEvent(pEvt, (ANMask)pEvt->fflags, pW);
	/* TOCHECK: voir si ca marche */
	ANotifyEvent* aevt = new ANotifyEvent(pEvt, (ANMask)pEvt->fflags, pW->getPath());
        m_events.push_back(aevt);
      }
    }
  }
#endif // __ENVIREONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
  
  IN_EVENT_WRITE_END
    }

bool ANotify::getEvent(ANotifyEvent* opEvt) throw (ANotifyException){
  if (peekEvent(opEvt)){
    m_events.pop_front();
    return true;
  }else
    return false;
}

bool ANotify::peekEvent(ANotifyEvent* opEvt) throw (ANotifyException)
{
  if (opEvt == NULL)
    throw ANotifyException(IN_EXC_MSG("Please allow space for event"), EINVAL, this);
  
  IN_EVENT_READ_BEGIN

    bool b = !m_events.empty();
  if (b) {
    *opEvt = *(m_events.front());
  }

  IN_EVENT_READ_END

    return b;
}

ANotifyWatch* ANotify::findWatch(FD iDescriptor)
{
  IN_WATCH_READ_BEGIN

    ANotifyWatch* pW;
  WatchFDMap::iterator it = m_watches.find(iDescriptor);
  if(it == m_watches.end() ) {
    pW = NULL;
  }else {
    pW = (*it).second;
  }
  
  IN_WATCH_READ_END
  
    return pW;
}

ANotifyWatch* ANotify::findWatch(const std::string& irPath)
{
  IN_WATCH_READ_BEGIN
  
    WatchPathMap::iterator it = m_paths.find(irPath);

  ANotifyWatch* pW;
  if(it == m_paths.end()) {
    pW = NULL;
  }else {
    pW = (*it).second;
  }

  IN_WATCH_READ_END
  
    return pW;
}

ANotifyWatch* ANotify::findWatchWOLock(std::string& path){
  WatchPathMap::iterator it = m_paths.find(path);
  ANotifyWatch* pW;
  
  if(it == m_paths.end()) {
    pW = NULL;
  }else {
    pW = (*it).second;
  }
  
  return pW;
}

void ANotify::setNonBlock(bool fNonBlock) throw (ANotifyException)
{
  IN_WRITE_BEGIN
  
    if (m_fileDescr == -1) {
      IN_WRITE_END_NOTHROW
	throw ANotifyException(IN_EXC_MSG("invalid file descriptor"), EBUSY, this);
    }
  
  int res = fcntl(m_fileDescr, F_GETFL);
  if (res == -1) {
    IN_WRITE_END_NOTHROW
      throw ANotifyException(IN_EXC_MSG("cannot get ANotify flags"), errno, this);
  }
  
  if (fNonBlock) {
    res |= O_NONBLOCK;
  }
  else {
    res &= ~O_NONBLOCK;
  }
  
  if (fcntl(m_fileDescr, F_SETFL, res) == -1) {
    IN_WRITE_END_NOTHROW
      throw ANotifyException(IN_EXC_MSG("cannot set ANotify flags"), errno, this);
  }
  
  IN_WRITE_END
    }

void ANotify::setCloseOnExec(bool fClOnEx) throw (ANotifyException)
{
  IN_WRITE_BEGIN
  
    if (m_fileDescr == -1) {
      IN_WRITE_END_NOTHROW
	throw ANotifyException(IN_EXC_MSG("invalid file descriptor"), EBUSY, this);
    }
  
  int res = fcntl(m_fileDescr, F_GETFD);
  if (res == -1) {
    IN_WRITE_END_NOTHROW
      throw ANotifyException(IN_EXC_MSG("cannot get ANotify flags"), errno, this);
  }
  
  if (fClOnEx) {
    res |= FD_CLOEXEC;
  }
  else {
    res &= ~FD_CLOEXEC;
  }
  
  if (fcntl(m_fileDescr, F_SETFD, res) == -1) {
    IN_WRITE_END_NOTHROW
      throw ANotifyException(IN_EXC_MSG("cannot set ANotify flags"), errno, this);
  }
  
  IN_WRITE_END
    }

uint32_t ANotify::getCapability(ANCapability cap) throw (ANotifyException)
{
  FILE* f = fopen(getCapabilityPath(cap).c_str(), "r");
  if (f == NULL)
    throw ANotifyException(IN_EXC_MSG("cannot get capability"), errno, NULL);
  
  unsigned int val = 0;
  if (fscanf(f, "%u", &val) != 1) {
    fclose(f);
    throw ANotifyException(IN_EXC_MSG("cannot get capability"), EIO, NULL);
  }
  
  fclose(f);
  
  return (uint32_t) val;
}

void ANotify::setCapability(ANCapability cap, uint32_t val) throw (ANotifyException)
{
  FILE* f = fopen(getCapabilityPath(cap).c_str(), "w");
  if (f == NULL)
    throw ANotifyException(IN_EXC_MSG("cannot set capability"), errno, NULL);
  
  if (fprintf(f, "%u", (unsigned int) val) <= 0) {
    fclose(f);
    throw ANotifyException(IN_EXC_MSG("cannot set capability"), EIO, NULL);
  }
  
  fclose(f);
}

std::string ANotify::getCapabilityPath(ANCapability cap) throw (ANotifyException)
{
  std::string path(PROCFS_ANOTIFY_BASE);
  
  switch (cap) {
  case MAX_EVENTS:
    path.append("max_queued_events");
    break;
  case MAX_INSTANCES:
    path.append("max_user_instances");
    break;
  case MAX_WATCHES:
    path.append("max_user_watches");
    break;
  default:
    throw ANotifyException(IN_EXC_MSG("unknown capability type"), EINVAL, NULL);
  }
  
  return path;
}

