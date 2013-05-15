

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


void happyEnd(){
  cout << "Good Bye." << endl;
}

void sigint_handler(int signum)
{
  try{
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
  }catch(ANotifyException e){
    cout << e.GetMessage() << endl;
  }  
}

void* eventListener(void* ano){
  sleep(5);

  try{
    ANotify* anotify = (ANotify*) ano;
    anotify->waitForEvents();
  }catch(ANotifyException e){
    cout << e.GetMessage() << endl;
  }
  
  pthread_exit(0);
}

void* indexer(void* ano){
  try{
    //ANotify* anotify = (ANotify*) ano; // ==== an
    ANotifyEvent aevt(&sharedEvent, defaultMask);
    sleep(2);
    /*string dir = "/Users/JC/Desktop/git";
      string file = "/Users/JC/Desktop/git/file";*/
    /*string dir = DIR_PATH;
      string file = FILE_PATH;*/
    string dir = "/home/cuisse/Dossier1";
    string file = "/home/cuisse/Dossier2/vide";
    ANotifyWatch* anw = new ANotifyWatch(dir,  &aevt, true, true); anw->setAsDir();
    ANotifyWatch* aw1 = new ANotifyWatch(file,  &aevt, false, true);
    anw->setMonitor(sharedANotify);
    aw1->setMonitor(sharedANotify);
    sharedANotify->add(anw);
    sharedANotify->add(aw1);
    sleep(1000);
  }catch(ANotifyException e){
    cout << e.GetMessage() << endl;
  }
  
  pthread_exit(0);
}

int main(int argc, const char * argv[])
{
  try{

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

    //ANotifyEvent* pEvt = new ANotifyEvent();
    ANotifyEvent pEvt;
    for (; ; ) {
      std::string str;
      //if(sharedANotify->getEvent(pEvt)){
      if(sharedANotify->getEvent(&pEvt)){
	//cout << "Event type: " << pEvt->getMask() << endl;
	pEvt.dumpTypes(str);
	cout << str << " fd : " <<  pEvt.getDescriptor() << endl;
      }
      sleep(1);
    }
  }catch(ANotifyException e){
    cout << e.GetMessage() << endl;
  }
  
  return 0;
}





