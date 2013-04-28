#ifndef __ANOTIFY_DAEMON_H__
#define __ANOTIFY_DAEMON_H__

#include "ANotify.h"

class ANotifyDaemon {

 private:
  ANotify* notify;
  int sock;

 public:
  ANotifyDaemon();
  ~ANotifyDaemon();

  void start();
  void restart();
  void stop();

  /* Retirer la surveillance d'un fichier */
  bool removeWatch(std::string& path);
  /* 
     Retirer recursivement la surveillance sur le fichier 
     et les fichiers fils (si repertoire)
   */
  bool recRemoveWatch(std::string& path);
};

#endif
