
  //
  //  main.cpp
  //  ANotify
  //
  //  Created by J. Charles N. MBIADA on 27/03/13.
  //  Copyright (c) 2013 Active. All rights reserved.
  //
/*
Tentative de spec formelle ; je n'ai pas répercute les modifs apres 
modification du code source pour des questions techniques
---
// Exception pour Linux/Unix 

Domain:
ANotifyException

Constantes :
m_msg:String, m_err: Int, m_pSrc: void*

Constructor:

Cons_ANotifyException:  -> ANotifyException
Cons_ANotifyException: String -> ANotifyException
Cons_ANotifyException: String x Int  -> ANotifyException
Cons_ANotifyException: String x Int x void*  -> ANotifyException

Acces methods:

getMessage: ANotifyException -> String
getError: ANotifyException -> Int
getSource: ANotifyException -> Void*

Other methods:


----
// Watch selon le system Linux/Unix 

Domain:
=========
ANotifyWatch
link: ANotify -> friend
Constantes:
m_path:String, m_uMast: UInt, m_wDescr: Int, m_pANotify: ANotify*, m_wEnable: Boolean

Constructor:
--------------
Cons_ANotifyWatch: String x  UInt -> ANotifyWatch
Cons_ANotifyWatch: String x  UInt x Boolean -> ANotifyWatch

Acces Methods:
--------------
inline getDescriptor 	: ANotifyWatch -> Int
inline getPath 		: ANotifyWatch -> String
inline getMask		: ANotifyWatch -> UInt
inline getANotify	: ANotifyWatch -> ANotify*
inline isEnable		: ANotifyWatch -> Boolean
inline isRecurssive	: ANotifyWatch -> Boolean

Setter Methods:
--------------
inline setMask		: ANotifyWatch -> Boolean
inline setANotify	: ANotifyWatch -> Boolean
inline setEnable		: ANotifyWatch -> Boolean
inline setRecurssive	: ANotifyWatch -> Boolean

Other methods:
--------------

#########

Domain:
=========
ANotifyEvent
link: ANotify -> friend
Constantes:
m_path:String, m_uMast: UInt, m_wDescr: Int, m_pANotify: ANotify*, m_wEnable: Boolean

Constructor:
--------------
Cons_ANotifyWatch: String x  UInt -> ANotifyWatch
Cons_ANotifyWatch: String x  UInt x Boolean -> ANotifyWatch

Acces Methods:
--------------
inline getDescriptor 	: ANotifyWatch -> Int
inline getPath 		: ANotifyWatch -> String
inline getMask		: ANotifyWatch -> UInt
inline getANotify	: ANotifyWatch -> ANotify*
inline isEnable		: ANotifyWatch -> Boolean
inline isRecurssive	: ANotifyWatch -> Boolean

Setter Methods:
--------------
inline setMask		: ANotifyWatch -> Boolean
inline setANotify	: ANotifyWatch -> Boolean
inline setEnable		: ANotifyWatch -> Boolean
inline setRecurssive	: ANotifyWatch -> Boolean

Other methods:
--------------

#############

Domain:
======
ANotify
link: ANotifyWatch -> friend

Constantes:
--------------
m_path:String, m_paths: WatchPathMap, m_fileDescr: Int, m_watches: WatchFDMap, m_buff:UChar array 

Constructor:
Cons_ANotifyWatch:  -> ANotify

Acces Methods:
--------------
inline getWatchCount 	: ANotify -> Int
inline getWatchEnableCount 	: ANotify -> Int
PeekEvent:	ANotify x ANotifyEvent* -> Boolean
GetEvent	: ANotify x InotifyEvent -> Boolean
FindWatch:	ANotify x Int -> InotifyWatch* 

Setter Methods:
--------------
inline setCapability		: ANotify x CAP -> Boolean
inline setNonBlock		: ANotify -> Boolean


Other methods:
close				: ANotify -> void
addFile			: ANotify x ANotifyWatch*  -> void
removeFile		: ANotify x ANotifyWatch*  -> void
removeAllFile		: ANotify  -> void

---

*/

#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "ANotify.h"
#include "ANotifyUtils.h"
#include "ANotifyMask.h"

using namespace std;

ANotify* sharedANotify = NULL;
  // On surveille les renommage, écriture, suppression ou maj des attributs d'un fichier
ANMask defaultMask = ANOTIFY_RENAME | ANOTIFY_WRITE | ANOTIFY_DELETE | ANOTIFY_ATTRIBUT;
ANOTIFY_EVENT sharedEvent;

void happyEnd(){
  cout << "Good Bye." << endl;
}

void sigint_handler(int signum)
{
  if (sharedANotify == NULL) {
    happyEnd();
    exit(EXIT_SUCCESS);
  }
  /* Nous ne surveillons plus ce fichier/répertoire */
  if (sharedANotify->removeAll()) {
    perror("Error during closed files watches");
    exit(EXIT_FAILURE);
  }
  /* Fermeture du descripteur de fichier obtenu lors de l'initialisation d'inotify */
  sharedANotify->AClose();
  happyEnd();
  exit(EXIT_SUCCESS);
}

void* eventListener(void* ano){
  sleep(5);
  ANotify* anotify = (ANotify*) ano;
  anotify->waitForEvents();
  pthread_exit(0);
}

void* indexer(void* ano){
  
    //ANotify* anotify = (ANotify*) ano; // ==== an
  ANotifyEvent aevt(&sharedEvent, defaultMask);
  sleep(2);
  string dir = "/Users/JC/Desktop/git";
  string file = "/Users/JC/Desktop/git/file";
  ANotifyWatch* anw = new ANotifyWatch(dir,  &aevt, true, true); anw->setAsDir();
  ANotifyWatch* aw1 = new ANotifyWatch(file,  &aevt, false, true);
  anw->setMonitor(sharedANotify);
  aw1->setMonitor(sharedANotify);
  sharedANotify->add(anw);
  sharedANotify->add(aw1);
  sleep(1000);
  pthread_exit(0);
}

int main(int argc, const char * argv[])
{
  /* Capture de SIGINT (Ctrl + C) */
  signal(SIGINT, sigint_handler);
  pthread_t threads[2] ;
  sharedANotify = new ANotify();
  
  int rcEvt = pthread_create(&threads[0], NULL, eventListener, (void *) sharedANotify);
  int rcIndex = pthread_create(&threads[1], NULL, indexer, (void *) sharedANotify);
  if (rcEvt == -1) {
    cout << "Erreur de lancement du thread d'event" << endl;
  }
  if (rcIndex == -1) {
    cout << "Erreur de lancement du thread d'Indexation" << endl;
  }
  ANotifyEvent* pEvt = new ANotifyEvent();
  for (; ; ) {
    std::string str;
    if(sharedANotify->getEvent(*pEvt)){
      pEvt->dumpTypes(str);
      cout << str << " fd : " <<  pEvt->getDescriptor() << endl;
    }
    sleep(1);
  }
  
  return 0;
}





#include <sys/event.h>


