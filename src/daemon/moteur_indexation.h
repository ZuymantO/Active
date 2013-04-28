#ifndef _MOTEUR_INDEXATION_
#define _MOTEUR_INDEXATION_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <pthread.h>

#define WORKING_DIR "/"

char *arborescence_a_surveiller;

pthread_mutex_t lock;

/* thread qui lance le notify */
void* notre_notify();

/* thread qui gere l'envoie de donnees */
void* envoie_donnees();

/* fonction qui gere les signaux envoyes au moteur d'indexaion */
void signal_handler(int sig);

/* 
 * fonction qui lance le daemon
 * prend en argument le nom du programme pour le log
 */
void deamonize(char* prog);

int main(int argc, char** argv);

#endif
