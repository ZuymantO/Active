#ifndef __ANOTIFY_DAEMON_H__
#define __ANOTIFY_DAEMON_H__

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

#include "ANotify.h"
#include "ANotifyException.h"

class ANotifyDaemon {

 private:
  ANotify* notify;
  int sock;
  bool running;
  pthread_mutex_t runningAccess;
  
  /* Retire la surveillance d'un fichier */
  bool removeOneWatch(std::string& path);
  /* 
     Retire recursivement la surveillance sur le fichier 
     et les fichiers fils (si repertoire)
  */
  bool recRemoveWatch(std::string& path);

 public:
  static const std::string propsPath;  

  ANotifyDaemon() throw (ANotifyException);
  ~ANotifyDaemon();

  /* Demarre le daemon */
  bool start();
  /* Redemarre le daemon */
  bool restart();
  /* Arrete le daemon */
  bool stop();
  /* Tue le daemon */
  bool kill();

  /* Renvoie un descripteur vers le fichier des proprietes du daemon */
  int openPropsFile();
  /* Ferme le fichier des proprietes du daemon */
  int closePropsFile(int fd);
  /* Supprime le contenu du fichier de proprietes */
  int deletePropsFile();

  /* Indique l'etat du daemon */
  bool isRunning();
  /* Changer l'etat du daemon */
  void setRunning(bool run);

  /* Fonction de traitement des evenements provenant du ANotify */
  void run();
  /* Attend la connexion de clients */
  void waitForClients(struct sockaddr_in* addr);
  /* Fonction de communication avec le client */
  void communicate(int client_socket);

  /* 
     Indique si le processus courant est celui du daemon en execution
     (renvoie true s'il n'y a aucun daemon en cours)
  */
  bool isActiveDaemon(int fd, pid_t daemon_pid);
  
  /* 
     Retire la surveillance du fichier a l'emplacement path
     et aussi celle de ses fils, recursivement, si rec est true
  */
  bool removeWatch(std::string& path, bool rec);
};

#endif
