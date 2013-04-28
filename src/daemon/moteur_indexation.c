#include "moteur_indexation.h"

/* thread 1 */
void* notre_notify() {
}

/* thread 2 */
void* envoie_donnees() {
}

void signal_handler(int sig) {
  switch (sig) {
  case SIGHUP:
    syslog(LOG_NOTICE, "hangup signal catched");
    break;
  case SIGTERM:
    syslog(LOG_NOTICE, "terminate signal catched");
    exit(EXIT_SUCCESS);
    break;
  }
}

void daemonize(char* prog) {
  pid_t pid, sid;
  int i, val_thr;
  pthread_t pth1, pth2;

  pid = fork();
  
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  /* pour avoir acces aux fichiers generes par le demon */
  umask(0);

  /* logs */
  openlog(prog, LOG_NOWAIT | LOG_PID, LOG_USER);
  syslog(LOG_NOTICE, "Successfully started \"moteur d'indexation\"\n");

  /* creation d'un nouvel SID pour le processus fils */
  sid = setsid();
  if (sid < 0) {
    syslog(LOG_ERR, "Could not create process group\n");
    exit(EXIT_FAILURE);
  }

  /* changement du repertoire de travail */
  if (chdir(WORKING_DIR) < 0) {
    syslog(LOG_ERR, "Could not change working directory to /\n");
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  signal(SIGTERM, signal_handler);

  /* on lance la surveillance de l'arborescence voulue */
  val_thr = pthread_create(&pth1, NULL, notre_notify(), NULL);
  if (val_thr != 0) {
    /* erreur de creation */
  }
  val_thr = pthread_create(&pth2, NULL, envoie_donnees(), NULL);
  if (val_thr != 0) {
    /* erreur creation */
  }

  while (1) {
    syslog(LOG_INFO, "my heart is beating...\n");
    sleep(15);
  }

  closelog();

}


int main(int argc, char** argv) {
  daemonize(argv[0]);
  exit(EXIT_SUCCESS);
}
