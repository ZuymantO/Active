#ifndef __ANOTIFY_DAEMON_H__
#define __ANOTIFY_DAEMON_H__

#include "ANotify.h"

class ANotifyDaemon {

 private:
  ANotify* notify;
  int sock;

 public:
  static const std::string propsPath = "daemon.prop";

  ANotifyDaemon();
  ~ANotifyDaemon();

  /* Demarre le daemon */
  void start();
  /* Redemarre le daemon */
  void restart();
  /* Arrete le daemon */
  void stop();

  /* Renvoie un descripteur vers le fichier des proprietes du daemon */
  int openPropsFile();
  /* */
  /* Supprime le contenu du fichier de proprietes */
  int deletePropsFile();

  /* 
     Indique si le processus courant est celui du daemon en execution
     (renvoie true s'il n'y a aucun daemon en cours)
  */
  bool isActiveDaemon(int fd, pid_t daemon_pid);
  
  /* Retirer la surveillance d'un fichier */
  bool removeWatch(std::string& path);
  /* 
     Retirer recursivement la surveillance sur le fichier 
     et les fichiers fils (si repertoire)
  */
  bool recRemoveWatch(std::string& path);

};

#endif
