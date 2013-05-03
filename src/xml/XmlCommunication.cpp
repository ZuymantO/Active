#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include "XmlCommunication.h"

using namespace std;

string XmlCommunication::MIToBI(ANotifyEvent ane) {
  ostringstream oss;
  string fileName;
  struct stat fileInfo;
  oss << EN_TETE << "<INDEXATION>";
  if (ane.isRennomage()) {
    oss << "<RENOMMAGES id=" << id << ">";
    oss << "<FICHIERRENOMME><PATH>";
    /*
     * TODO : ajouter le nom du oldpath
     * oss << oldpath;
     */
    oss << "</PATH><NEWPATH>";
    /* 
     * TODO : ajouter le nom du newpath
     * oss << newpath;
     */
    oss << "</NEWPATH></FICHIERRENOMME>";
    oss << "</RENOMMAGES>";
    id++;
  }
  else if (ane.isModification()) {
    fileName = ane.getName();
    if (stat(fileName, &fileInfo) == 0) {
      oss << "<MODIFICATIONS id=" << id << ">";
      oss << "<PATH>" << ane.getName() << "</PATH>";
      oss << "<DATEMODIFICATION" << ctime(&fileInfo.st_mtime) << "</DATEMODIFICATION>";
      oss << "<TAILLE>" << fileInfo.st_size << "</TAILLE>";
      oss << "<PROPRIETAIRE>" << fileInfo.st_uid << "</PROPRIETAIRE>";
      oss << "<GROUPE>" << fileInfo.st_gid << "</GROUPE>";
      oss << "<PERMISSIONS>" << fileInfo.st_mode << "</PERMISSIONS>";
      /*
       * TODO : ajouter les mots d'indexages par appel de fonction
       *        qui doit renvoyer un string
       */
      oss << "<INDEXAGE>" << "mots_a_indexer(fonction)" <<"</INDEXAGE>";
      oss << "<NEWPATH>" << ane.getName() << "</NEWPATH>";
      oss << "</MODIFICATIONS>";
      id++;
    }
    else {
      // Le fichier n'existe pas
      return "";
    }
  }
  else if (ane.isSuppression()) {
    oss << "<SUPPRESSIONS id=" << id << ">";
    oss << "<FICHIERSUPPRIME>" << ane.getName() << "</FICHIERSUPPRIME>";
    oss << "</SUPPRESSIONS>";
    id++;
  }
  else if (ane.isCreation()) {
    fileName = ane.getName();
    if (stat(fileName, &fileInfo) == 0) {
      oss << "<CREATIONS id=" << id << ">";
      oss << "<PATH>" << fileName << "</PATH>";
      /*
       * TODO : recuperation du format :
       *        - soit sur ane par ane.getFormat()
       *        - soit sur le nom du fichier
       */
      oss << "<format>" << ane.getFormat() << "</format>";
      /*
       * Liberte : la date de creation d'un fichier est identique a sa date de
       * derniere modification au moment de sa creation
       */
      oss << "<DATECREATION>" << ctime(&fileInfo.st_mtime) << "</DATECREATION>";
      oss << "<TAILLE>" << fileInfo.st_size << "</TAILLE>";
      oss << "<PROPRIETAIRE>" << fileInfo.st_uid << "</PROPRIETAIRE>";
      oss << "<GROUPE>" << fileInfo.st_gid << "</GROUPE>";
      oss << "<PERMISSIONS>" << fileInfo.st_mode << "</PERMISSIONS>";
      /*
       * TODO : ajouter les mots d'indexages par appel de fonction
       *        qui doit renvoyer un string
       */
      oss << "<INDEXAGE>" << "mots_a_indexer(fonction)" << "</INDEXAGE>";
      oss << "</CREATIONS>";
      id++;
    }
    else {
      // Le fichier n'existe pas
      return "";
    }
  }
  else {
    // ces cas ne sont pas pris en compte par la dtd
    return "";
  }
  oss << "</INDEXATION>";
  return oss.str();
}

string XmlCommunication::BIToMR(ANotifyEvent ane) {
  ostringstream oss;
  oss << "1";
  return oss.str();
}

string XmlCommunication::MRToBI(ANotifyEvent ane) {
  return "";
}

int main() {
  ostringstream oss;
  oss << "coucou";
  oss << endl;
  oss << "test";
  oss << endl;
  cout << oss.str();
  return 0;
}
