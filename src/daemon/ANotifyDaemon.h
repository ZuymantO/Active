#ifndef __ANOTIFY_DAEMON_H__
#define __ANOTIFY_DAEMON_H__

#include <pthread.h>
#include <netinet/in.h>

#include "ANotify.h"
#include "ANotifyException.h"

class ANotifyDaemon {

 private:
  ANotify* notify;
  ANOTIFY_EVENT sharedEvent;
  ANotifyEvent* notifyEvent;
  ANMask mask;

  int sock;
  int xml_sock;
  struct sockaddr_in addr;
  struct sockaddr_in xml_addr;

  int propsFd;
  int logFd;

  bool running;
  bool alive;
  mutable pthread_mutex_t runningAccess;
  mutable pthread_mutex_t logLock;
  mutable pthread_mutex_t aliveAccess;
  mutable pthread_mutex_t xmlSockLock;

  /* Racine du chemin en cours de surveillance */
  std::string watchPath;
  
  /* Retire la surveillance d'un fichier */
  bool removeOneWatch(std::string& path);
  /* 
     Retire recursivement la surveillance sur le fichier 'path'
     et les fichiers fils (si repertoire)
  */
  bool recRemoveWatch(std::string& path);

 public:
  /* Chemin vers le fichier des proprietes du daemon */
  static const std::string propsPath;
  /* Chemin vers le fichier de log du daemon */
  static const std::string logPath;  

  /* ------- CONSTRUCTEUR ------- */

  ANotifyDaemon() throw (ANotifyException);

  /* ------- DESTRUCTEUR ------- */

  ~ANotifyDaemon();

  /* ------- FONCTIONS D'INITIALISATION ------- */

  void initDaemon() throw (ANotifyException);

  /* ------- FONCTIONS DE CONTROLE ------- */

  /* Recupere le retour d'une fonction de thread renvoyant un booleen */
  bool retrieveBoolResult(void* (f)(void*), void* arg);

  /* Demarre la surveillance sur 'watchPath' */
  bool start();
  /* Demarre la surveillance sur le chemin en argument */
  bool start(std::string& path);
  static void* startT(void* daemon_path_pair);   // ---> bool

  /* Redemarre la surveillance sur 'watchPath' */
  bool restart();
  /* Redemarre la surveillance sur le chemin en argument */
  bool restart(std::string& path);

  /* Arrete le daemon */
  bool stop();

  /* Tue le daemon */
  bool kill();

  /* Liste les fichiers surveilles */
  bool list(int client_socket);

  /* ------- FONCTIONS D'INFORMATIONS ------- */

  /* Renvoie un descripteur vers le fichier des proprietes du daemon */
  static int openPropsFile();
  /* Ferme le fichier des proprietes du daemon */
  static int closePropsFile(int fd);
  /* Supprime le fichier de proprietes */
  static int deletePropsFile();

  /* Renvoie un descripteur vers le fichier de log */
  int openLogFile();
  /* Ferme le fichier de log */
  int closeLogFile();
  /* Ecrit le message 'msg' dans le fichier de log */
  void printLog(std::string& msg);
  /* Supprime le fichier de log */
  int deleteLogFile();
  

  /* 
     Indique si le processus courant est celui du daemon en execution
     (renvoie true s'il n'y a aucun daemon en cours)
  */
  bool isActiveDaemon(pid_t daemon_pid);
  /* Renvoie le port d'ecoute du daemon, -1 en cas d'echec */
  static int getDaemonPort(int fd);

  /* ------- FONCTIONS D'ETAT ------- */

  /* Indique l'etat du daemon */
  bool isRunning();
  /* Change l'etat du daemon */
  void setRunning(bool run);

  /* */
  bool isAlive();
  /* */
  void setAlive(bool alv);

  /* ------- FONCTIONS ------- */

  /* Fonction de traitement des evenements provenant du ANotify */
  static void* run(void* dae);
  /* Tente de surveiller (recursivement) 'path' (si c'est un repertoire) */
  bool addWatch(std::string& path);
  /* Fonction d'attente des evenements du ANotify */
  static void* waitForEvents(void* dae);

  void waitForClients();
  /* Attend la connexion de clients */
  static void* waitForClients(void* dae);
  /* Fonction de communication avec le client */
  static void* communicate(void* daemon_socketfd_pair);

  /* Envoie un message au format XML */
  static void* sendXmlContent(void* daemon_xml_pair);
  

  /* 
     Retire la surveillance du fichier a l'emplacement path
     et aussi celle de ses fils, recursivement, si rec est true
  */
  bool removeWatch(std::string& path, bool rec);
};

#endif
