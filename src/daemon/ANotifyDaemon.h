#ifndef __ANOTIFY_DAEMON_H__
#define __ANOTIFY_DAEMON_H__

#include <pthread.h>
#include <netinet/in.h>

#include "ANotify.h"
#include "ANotifyException.h"

class ANotifyDaemon {

 private:
  ANotify* notify;
  int sock;
  struct sockaddr_in* addr;
  int propsFd;
  bool running;
  pthread_mutex_t runningAccess;
  std::string watchPath;
  
  /* Retire la surveillance d'un fichier */
  bool removeOneWatch(std::string& path);
  /* 
     Retire recursivement la surveillance sur le fichier 
     et les fichiers fils (si repertoire)
  */
  bool recRemoveWatch(std::string& path);

 public:
  /* Chemin vers le fichier des proprietes du daemon */
  static const std::string propsPath;
  /* Chemin vers le fichier de log du daemon */
  static const std::string logPath;
  

  /* Constructeur */
  ANotifyDaemon() throw (ANotifyException);
  /* Destructeur */
  ~ANotifyDaemon();

  /* Demarre la surveillance sur 'watchPath' */
  bool start();
  /* Demarre la surveillance sur le chemin en argument */
  bool start(std::string& path);  
  /* Demarre la surveillance sur 'watchPath' */
  bool restart();
  /* Redemarre la surveillance sur le chemin en argument */
  bool restart(std::string& path);
  /* Arrete le daemon */
  bool stop();
  /* Tue le daemon */
  bool kill();

  /* Renvoie un descripteur vers le fichier des proprietes du daemon */
  int openPropsFile();
  /* Ferme le fichier des proprietes du daemon */
  int closePropsFile();
  /* Supprime le contenu du fichier de proprietes */
  int deletePropsFile();

  /* Indique l'etat du daemon */
  bool isRunning();
  /* Changer l'etat du daemon */
  void setRunning(bool run);

  /* Fonction de traitement des evenements provenant du ANotify */
  void run();
  /* Fonction d'attente des evenements du ANotify */
  void waitForEvents();
  /* Attend la connexion de clients */
  void waitForClients(struct sockaddr_in* addr);
  /* Fonction de communication avec le client */
  void communicate(int client_socket);

  /* 
     Indique si le processus courant est celui du daemon en execution
     (renvoie true s'il n'y a aucun daemon en cours)
  */
  bool isActiveDaemon(pid_t daemon_pid);
  
  /* 
     Retire la surveillance du fichier a l'emplacement path
     et aussi celle de ses fils, recursivement, si rec est true
  */
  bool removeWatch(std::string& path, bool rec);
};

#endif
