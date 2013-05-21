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

bool sendCommand(int socket_fd, char cmd, bool b, string path = ""){
  if(socket_fd == -1){
    return false;
  }

  int length, pathLength;
  char msg;

  msg = cmd;
  length = send(socket_fd, &msg, 1, 0);
  
  if(length <= 0){
    return false;
  }

  if(b){
    msg = 'Y';
    length = send(socket_fd, &msg, 1, 0);

    if(length <= 0){
      return false;
    }   
  }
  else{
    msg = 'N';
    length = send(socket_fd, &msg, 1, 0);

    if(length <= 0){
      return false;
    }
  }

  if(cmd != 'D' && !b){
    return true;
  }

  pathLength = path.length();
  length = send(socket_fd, &pathLength, sizeof(int), 0);
  
  if(length <= 0){
    return false;
  }
  
  length = send(socket_fd, path.c_str(), pathLength, 0);
  
  if(length <= 0
     || length < pathLength){
    return false;
  }
  
  return true;
}

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
    rec = false, //recursivite pour la suppression
    yes = false,
    correct = true;
  string instruction, //instruction tapee en ligne de commande
    path; //chemin fourni en argument
  struct sockaddr_in addr; //infos de connexion pour la socket
  char msg; //caractere correspondant a l'instruction courante
  int nb, length, i, pathLength;
  char buffer[300]; //chaine contenant le chemin d'un fichier
  

  propsFd = ANotifyDaemon::openPropsFile();

  if(propsFd == -1){
    cout << "There is no daemon running" << endl;
    exit(EXIT_FAILURE);
  }

  port = ANotifyDaemon::getDaemonPort(propsFd);

  if(port == -1){
    cout << "Unable to get daemon port" << endl;
    ANotifyDaemon::closePropsFile(propsFd);
    exit(EXIT_FAILURE);
  }

  ANotifyDaemon::closePropsFile(propsFd);
  
  
  sock = socket(AF_INET, SOCK_STREAM, 0);

  struct hostent* host = gethostbyname("localhost");
    
  if(host == NULL){
    std::cout << "No such host: localhost" << std::endl;
    ANotifyDaemon::closePropsFile(propsFd);
    close(sock);
    exit(EXIT_FAILURE);
  }
  
  memcpy(&(addr.sin_addr.s_addr), host->h_addr_list[0], host->h_length);
  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  
  if(connect(sock, (struct sockaddr*)(&addr), sizeof(addr)) == -1){
    std::cout << "Connection failed" << std::endl;
    ANotifyDaemon::closePropsFile(propsFd);
    close(sock);
    exit(EXIT_FAILURE);
  }
  
  cout << "Type 'help' for usage" << endl;

  while(connected){
    cout << "> ";
    getline(cin, instruction);

    correct = true;
    yes = false;

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
	cout << "failed to send list instruction" << endl;
	connected = false;
	break;
      }

      length = recv(sock, &nb, sizeof(int), 0);
      
      if(length <= 0){	
	cout << "failed to receive watch number" << endl;
	connected = false;
	break;
      }

      if(nb > 0){
	cout << "(" << nb << " watche(s) found)" << endl;	

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
      else{
	cout << "(No watch found)" << endl;
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
	  if(cmds.size() == 2){
	    /* > start PATH*/
	    yes = true;	    
	    path = cmds[1];
	  }
	  else if(cmds.size() == 1){
	    /* > start */
	    yes = false;
	  }
	  else{
	    cout << "Too much arguments" << endl;
	    correct = false;
	    continue;
	  }

	  if(correct){
	    connected = sendCommand(sock, 'S', yes, path);
	  }
	}
	else if(cmds[0] == "restart"){
	  if(cmds.size() == 2){
	    /* > start PATH*/
	    yes = true;
	    path = cmds[1];
	  }
	  else if(cmds.size() == 1){
	    /* > start */
	    yes = false;
	  }
	  else{
	    cout << "Too much arguments" << endl;
	    correct = false;
	    continue;
	  }	  

	  if(correct){
	    connected = sendCommand(sock, 'R', yes, path);
	  }
	}
	else if(cmds[0] == "remove"){
	  if(cmds.size() >= 2){
	    int pathIndex = 1;
	    
	    if(cmds[1] == "-r"){
	      yes = true;
	      pathIndex = 2;
	    }
	    else{
	      yes = false;
	    }
	    
	    if(cmds.size() == pathIndex + 1){
	      path = cmds[pathIndex];
	    }
	    else{
	      cout << "Too much arguments" << endl;
	      correct = false;
	      continue;
	    }

	    if(correct){
	      connected = sendCommand(sock, 'D', yes, path);
	    }
	  }
	  else{
	    cout << "error: Invalid arguments" << endl;
	    correct = false;
	    continue;
	  }
	}
	else{
	  cout << "error: Unknown command" << endl;
	  correct = false;
	  continue;
	}	
      }
    }
      
    if(!correct){
      continue;
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
