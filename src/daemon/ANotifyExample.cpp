

  //
  //  main.cpp
  //  ANotify
  //
  //  Created by J. Charles N. MBIADA on 27/03/13.
  //  Copyright (c) 2013 Sylorion. All rights reserved.
  //

#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "ANotify.h"
#include "ANotifyWatch.h"
#include "ANotifyUtils.h"
#include "ANotifyMask.h"

using namespace std;

ANotify* sharedANotify = NULL;
  // On surveille les renommage, écriture, suppression ou maj des attributs d'un fichier
ANMask defaultMask = ANOTIFY_RENAME | ANOTIFY_WRITE | ANOTIFY_DELETE | ANOTIFY_ATTRIBUT;
ANOTIFY_EVENT sharedEvent;

/*#ifdef DIR_PATH_VALUE
#define DIR_PATH DIR_PATH_VALUE
#else
#define DIR_PATH ""
#endif

#ifdef FILE_PATH_VALUE
#define FILE_PATH FILE_PATH_VALUE
#else
#define FILE_PATH ""
#endif*/

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
  /*string dir = "/Users/JC/Desktop/git";
  string file = "/Users/JC/Desktop/git/file";*/
  /*string dir = DIR_PATH;
  string file = FILE_PATH;*/
  string dir = "/home/cuisse/Dossier 1";
  string file = "/home/cuisse/Dossier 2/vide";
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
    //if(sharedANotify->getEvent(*pEvt)){
    if(sharedANotify->getEvent(pEvt)){
      pEvt->dumpTypes(str);
      cout << str << " fd : " <<  pEvt->getDescriptor() << endl;
    }
    sleep(1);
  }
  
  return 0;
}





