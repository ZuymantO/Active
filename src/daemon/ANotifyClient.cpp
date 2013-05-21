#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sstream>

#include "ANotifyDaemon.h"

using namespace std;

void printHelp(){
  cout << "--------------------------------------------------" << endl;
  cout << "Available commands:" << endl;
  cout << "--------------------------------------------------" << endl;
  cout << "help\t\t\t(prints help)" << endl;
  cout << "exit\t\t\t(ends connection)" << endl;
  cout << "list\t\t\t(lists watched files)" << endl;
  cout << "start [PATH]\t\t(starts daemon watch on path 'PATH' if given)" << endl;
  cout << "restart [PATH]\t\t(restarts daemon watch on path 'PATH' if given)" << endl;
  cout << "stop\t\t\t(stops daemon)" << endl;
  cout << "kill\t\t\t(kills daemon)" << endl;
  cout << "remove [-r] PATH\t([resursively] removes watch on path 'PATH')" << endl;
  cout << "--------------------------------------------------" << endl;
}

int main(int argc, char** argv){
  int sock, //descripteur socket
    port = -1, //port socket
    propsFd = -1; //descripteur fichier
  bool connected = true, //etat de la connexion
    getRet = true, //verifier le retour d'une commande ou non
    rec = false; //recursivite pour la suppression
  string instruction, //instruction tapee en ligne de commande
    path; //chemin fourni en argument
  struct sockaddr_in addr; //infos de connexion pour la socket
  char msg; //caractere correspondant a l'instruction courante
  int nb, length, i, pathLength;
  char buffer[300]; //chaine contenant le chemin d'un fichier

  propsFd = ANotifyDaemon::openPropsFile();

  if(propsFd == -1){
    /* TODO: Lever une exception */
    exit(EXIT_FAILURE);
  }

  port = ANotifyDaemon::getDaemonPort(propsFd);

  if(port == -1){
    ANotifyDaemon::closePropsFile(propsFd);
    exit(EXIT_FAILURE);
  }

  ANotifyDaemon::closePropsFile(propsFd);
  
  /* TODO: <Connexion de la socket au port> */
  
  sock = socket(AF_INET, SOCK_STREAM, 0);

  struct hostent* host = gethostbyname("localhost");
    
  if(host == NULL){
    std::cout << "Hote inexistant: localhost" << std::endl;
    ANotifyDaemon::closePropsFile(propsFd);
    close(sock);
    exit(EXIT_FAILURE);
  }
  
  memcpy(&(addr.sin_addr.s_addr), host->h_addr_list[0], host->h_length);
  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  
  if(connect(sock, (struct sockaddr*)(&addr), sizeof(addr)) == -1){
    std::cout << "Connexion echouee" << std::endl;
    ANotifyDaemon::closePropsFile(propsFd);
    close(sock);
    exit(EXIT_FAILURE);
  }

  /* </Connexion> */
  
  cout << "Type 'help' for usage" << endl;

  while(connected){
    cout << "> ";
    getline(cin, instruction);

    if(instruction == "exit"){
      connected = false;
      break;
    }
    else if(instruction == "help"){
      printHelp();
      continue;
    }
    else if(instruction == "kill"){
      msg = 'K';

      length = send(sock, &msg, 1, 0);

      if(length <= 0){
	connected = false;
	break;
      }

      connected = false;
    }
    else if(instruction == "stop"){
      msg = 'E';

      length = send(sock, &msg, 1, 0);

      if(length <= 0){
	connected = false;
	break;
      }
    }
    else if(instruction == "list"){
      msg = 'L';
      
      length = send(sock, &msg, 1, 0);

      if(length <= 0){
	cout << "Send failed" << endl;
	connected = false;
	break;
      }

      length = recv(sock, &nb, sizeof(int), 0);
      
      if(length <= 0){
	cout << "Recv failed" << endl;
	connected = false;
	break;
      }

      if(nb > 0){
	cout << "(" << nb << " watche(s) found)" << endl;
      }
      else{
	cout << "(No watch found)" << endl;
      }

      /* Reception des chemins des fichiers surveilles */
      for(i=0; i<nb; i++){
	length = recv(sock, &pathLength, sizeof(int), 0);

	if(length <= 0 || pathLength <= 0){
	  connected = false;
	  break;
	}

	length = recv(sock, buffer, pathLength, 0);

	if(length < pathLength){
	  connected = false;
	  break;
	}

	buffer[pathLength] = '\0';
	path = buffer;

	cout << path << endl;
      }
    }
    //Instructions avec arguments
    else{
      vector<string> cmds;
      string arg;
      stringstream ss(instruction);

      while(ss >> arg){
	cmds.push_back(arg);
      }
            
      if(!cmds.empty()){
	if(cmds[0] == "start"){
	  /* TODO */
	  msg = 'S';

	  length = send(sock, &msg, 1, 0);

	  if(cmds.size() == 2){
	    /* > start PATH*/
	    msg = 'Y';
	    length = send(sock, &msg, 1, 0);

	    if(length <= 0){
	      /* TODO */
	      connected = false;
	      break;
	    }
	  }
	  else if(cmds.size() == 1){
	    /* > start */
	    msg = 'N';
	    length = send(sock, &msg, 1, 0);

	    if(length <= 0){
	      /* TODO */
	      connected = false;
	      break;
	    }
	  }
	  else{
	    cout << "Too much arguments" << endl;
	    continue;
	  }
	}
	else if(cmds[0] == "restart"){
	  /* TODO */
	  msg = 'R';
	  length = send(sock, &msg, 1, 0);

	  if(cmds.size() == 2){
	    /* > start PATH*/
	    msg = 'Y';
	    length = send(sock, &msg, 1, 0);

	    if(length <= 0){
	      /* TODO */
	      connected = false;
	      break;
	    }
	  }
	  else if(cmds.size() == 1){
	    /* > start */
	    msg = 'N';
	    length = send(sock, &msg, 1, 0);

	    if(length <= 0){
	      /* TODO */
	      connected = false;
	      break;
	    }
	  }
	  else{
	    cout << "Too much arguments" << endl;
	    continue;
	  }	  
	}
	else if(cmds[0] == "remove"){
	  /* TODO */
	  if(cmds.size() >= 2){
	    msg = 'R';
	    length = send(sock, &msg, 1, 0);

	    if(length <= 0){
	      connected = false;
	      break;
	    }

	    int pathIndex = 1;

	    if(cmds[1] == "-r"){
	      rec = true;
	      msg = 'Y';
	      pathIndex = 2;
	    }
	    else{
	      rec = false;
	      msg = 'N';
	    }

	    length = send(sock, &msg, 1, 0);

	    if(length <= 0){
	      connected = false;
	      break;
	    }
	    
	    if(cmds.size() == pathIndex + 1){
	      path = cmds[pathIndex];
	      pathLength = path.length();

	      length = send(sock, &pathLength, sizeof(int), 0);

	      if(length <= 0){
		connected = false;
		break;
	      }

	      length = send(sock, path.c_str(), pathLength, 0);

	      if(length <= 0){
		connected = false;
		break;
	      }
	    }
	    else{
	      cout << "Too much arguments" << endl;
	    }
	  }
	  else{
	    cout << "error: Invalid arguments" << endl;
	  }
	}
	else{
	  cout << "error: Unknown command" << endl;
	  //getRet = false;
	  continue;
	}	
      }
    }
      
    if(!connected){
      break;
    }
    
    length = recv(sock, &msg, 1, 0);

    if(length <= 0){
      cout << "answer recv failed" << endl;
    }
    else{
      if(msg == 'Y'){
	cout << "[operation succeeded]" << endl;
      }
      else{
	cout << "[operation failed]" << endl;
      }
    }
  }

  close(sock);

  return 0;
}
