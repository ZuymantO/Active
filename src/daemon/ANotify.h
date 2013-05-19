//
//  ANotify.h
//  ANotify
//
//  Created by J. Charles N. MBIADA on 17/03/13.
//  Copyright (c) 2013 Active All rights reserved.
//
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

#ifndef __ANotify__ANotify__
#define __ANotify__ANotify__

#ifndef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
   #include <sys/inotify.h>
#endif // __ENVIREONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__

#include <deque>  // La liste des evenements est une deque
#include <map>    // La liste des fichier watches par FD et paths name sont des map
#include "ANotifyUtils.h" // Ensemble de macro et define pour simplifier la portabilit'e
#include "ANotifyException.h" // Une class permettant d'abstraire les erreurs errno vers une gestion par exception
#include "ANotifyEvent.h" // Gestion des Evenements kevent ou inotfy_event (on s'y abstrait via Active Notify Event
#include "ANotifyWatch.h" // Un objet surveill'e est encapsul'e dans un Watch
#include "ANotifyMask.h"  // Ensemble de masque support'e pour les evements et signaux system (on reduit la puissance de kevent)

  // Capacités limite du systeme de notification (pour configurer le system
typedef enum{
  MAX_EVENTS     = 0,  ///< max. events in the kernel queue
  MAX_INSTANCES  = 1,  ///< max. inotify file descriptors per process
  MAX_WATCHES    = 2   ///< max. watches per file descriptor
} ANCapability;

class ANotify;
class ANotifyMask;
class ANotifyException;
class ANotifyWatch;
class ANotifyEvent;

  // Map by file descriptor
typedef std::map<FD, ANotifyWatch*> WatchFDMap;       // Le type pour stocker les fichiers surveill'es par File Descriptor
  // Map by path
typedef std::map<std::string, ANotifyWatch*> WatchPathMap;  // Le type pour stocker les fichiers surveill'es par nom

class ANotify
{
  
private:
  FD m_fileDescr;                             // file descriptor
  WatchFDMap m_watches;               // watches (by descriptors)
  WatchPathMap m_paths;                    // watches (by paths)
  std::string m_buff;                 // Buffer des evenements pour par inotify
  unsigned char m_buf[ANOTIFY_BUFF_LENGTH];  ///< buffer for events
  std::deque<ANotifyEvent*> m_events;    ///< event deque
  IN_LOCK_DECL // On declare un verrou pour les acces aux ressources partages (listes ci-dessus, buffer etc)
  
public:

  ANotify() throw (ANotifyException);
  ~ANotify();
  void AClose();
  void add(ANotifyWatch* ipWatch) throw (ANotifyException);
  inline void add(ANotifyWatch& irWatch) throw (ANotifyException){
    add(&irWatch);
  }
  

  bool remove(ANotifyWatch* ipWatch) throw (ANotifyException);
  inline bool remove(ANotifyWatch& irWatch) throw (ANotifyException){
    return remove(&irWatch);
  }
  bool removeAll() throw (ANotifyException);

  inline size_t getWatchCount() const{
    IN_WATCH_READ_BEGIN
    size_t n = (size_t) m_paths.size();
    IN_WATCH_READ_END
    return n;
  }
  inline size_t getEnabledCount() const{
    IN_WATCH_READ_BEGIN
    size_t n = (size_t) m_watches.size();
    IN_WATCH_READ_END
    return n;
  }

  
  void waitForEvents(bool ifNoIntr = false) throw (ANotifyException);
  inline size_t getEventCount(){
    IN_EVENT_READ_BEGIN
    size_t n = (size_t) m_events.size();
    IN_EVENT_READ_END
    return n;
  }

  bool getEvent(ANotifyEvent* opEvt) throw (ANotifyException);
  bool getEvent(ANotifyEvent& orEvt) throw (ANotifyException){
    return getEvent(&orEvt);
  }

  bool peekEvent(ANotifyEvent* opEvt) throw (ANotifyException);
  bool peekEvent(ANotifyEvent& orEvt) throw (ANotifyException){
    return peekEvent(&orEvt);
  }
  ANotifyWatch* findWatch(int iDescriptor);
  ANotifyWatch* findWatch(const std::string& irPath);
  inline int getDescriptor() const{
    return m_fileDescr;
  }

  WatchFDMap getWatchesFDMap(){
    return m_watches;
  }
  WatchPathMap getWatchesPathMap(){
    return m_paths;
  }
  FD getFileDescriptor(){
    return m_fileDescr;
  }
  std::string getBuffer(){
    return m_buff;
  }
  std::deque<ANotifyEvent*> getEventQeu(){
    return m_events;
  }

  void setNonBlock(bool ifNonBlock) throw (ANotifyException);
  void setCloseOnExec(bool ifClOnEx) throw (ANotifyException);
  static uint32_t getCapability(ANCapability icap) throw (ANotifyException);
  static void setCapability(ANCapability icap, uint32_t ival) throw (ANotifyException);
  inline static uint32_t getMaxEvents() throw (ANotifyException){
    return getCapability(MAX_EVENTS);
  }
  inline static void setMaxEvents(uint32_t ival) throw (ANotifyException){
    setCapability(MAX_EVENTS, ival);
  }
  inline static uint32_t getMaxInstances() throw (ANotifyException){
    return getCapability(MAX_INSTANCES);
  }
  inline static void setMaxInstances(uint32_t ival) throw (ANotifyException){
    setCapability(MAX_INSTANCES, ival);
  }
  inline static uint32_t getMaxWatches() throw (ANotifyException){
    return getCapability(MAX_WATCHES);
  }
  inline static void setMaxWatches(uint32_t ival) throw (ANotifyException){
    setCapability(MAX_WATCHES, ival);
  }
  
  static std::string getCapabilityPath(ANCapability cap) throw (ANotifyException);

};



#endif /* defined(__ANotify__ANotify__) */
