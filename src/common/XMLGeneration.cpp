#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include "XMLGeneration.h"
#include "AnyFile.h"
#include "AQuery.h"

using namespace std;
using namespace acommon;

string XMLGeneration::MIToBI(ANotifyEvent ane) {
  ostringstream oss;
  string fileName;
  struct stat fileInfo;
  oss << EN_TETE << "<INDEXATION>";
  
  if (ane.isType(INOTIFY_MODIFY)) {
    fileName = ane.getName();
    if (stat(fileName.c_str(), &fileInfo) == 0) {
      oss << "<MODIFICATIONS id=" << id << "><FICHIERMOFIFIE>";
      oss << "<PATH>" << fileName << "</PATH>";
      oss << "<DATEMODIFICATION>" << ctime(&fileInfo.st_mtime) << "</DATEMODIFICATION>";
      oss << "<TAILLE>" << fileInfo.st_size << "</TAILLE>";
      oss << "<PROPRIETAIRE>" << fileInfo.st_uid << "</PROPRIETAIRE>";
      oss << "<GROUPE>" << fileInfo.st_gid << "</GROUPE>";
      oss << "<PERMISSIONS>" << fileInfo.st_mode << "</PERMISSIONS>";
      /*
       * TODO : ajouter les mots d'indexages par appel de fonction
       *        qui doit renvoyer un string
       */
      oss << "<INDEXAGE>" << "mots_a_indexer(fonction)" <<"</INDEXAGE>";
      oss << "<NEWPATH>" << fileName << "</NEWPATH>";
      oss << "</FICHIERMODIFIE></MODIFICATIONS>";
      id++;
    }
    else {
      // Le fichier n'existe pas
      return "";
    }
  }
  else if (ane.isType(ANOTIFY_DELETE)) {
    oss << "<SUPPRESSIONS id=" << id << "><FICHIERSUPPRIME>";
    oss << "<FICHIERSUPPRIME>" << ane.getName() << "</FICHIERSUPPRIME>";
    oss << "</FICHIERSUPPRIME></SUPPRESSIONS>";
    id++;
  }
  else if (ane.isType(INOTIFY_CREATE)) {
    fileName = ane.getName();
    if (stat(fileName.c_str(), &fileInfo) == 0) {
      oss << "<CREATIONS id=" << id << "><FICHIERCREE>";
      oss << "<PATH>" << fileName << "</PATH>";
      oss << "<format>" << Utils::GetExtension(fileName) << "</format>";
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
      oss << "</FICHIERCREE></CREATIONS>";
      id++;
    }
    else {
      // cas non pris en compte
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

// comme explique dans le code des ANotify la convention i/o (input/output) p/r(pointer/ref) nom_variable 
// permet de vite analyser la le fonctionnement de la fonction. ipquery voulait dire input pointer query ici aq devrait etre nomme
// iraq par exemple...
string XMLGeneration::BIToMR(AQuery& ipquery) {
  if (&ipquery == 0 || !ipquery.hasNewResult()) {
    return "";
  }

  ostringstream oss;
  
  vector<AnyFile>* AQResult = ipquery.results();
  vector<AnyFile>::iterator tmpIt = AQResult->begin();
  oss << "<RESULT id=" << ipquery.GetId() << ">";
  if(ipquery.hasNewResult()) {
    while (tmpIt != AQResult->end()) {
      AnyFile f = *tmpIt;
      oss << "<FILE>";
      oss << "<NAME>" << f.GetName() << "</NAME>";
      oss << "<PATH>" << f.GetPath() << "</PATH>";
      oss << "<PERM>" << f.GetMime() << "</PERM>";
      oss << "<SIZE>" << f.GetDiskSize() << "</SIZE>";
      if (f.GetLastModif() != "") {
	oss << "<LASTMODIF>" << f.GetLastModif() << "</LASTMODIF>";
      }
      oss << "<PROPRIO>" << f.GetUserId() << "</PROPRIO>";
      oss << "</FILE>";
      tmpIt++;
    }
  }
  oss << "</RESULT>";
  return oss.str();
}

string XMLGeneration::MRToBI(Search search, int searchID) {
  ostringstream oss;
  oss << "<SEARCH id=" << searchID << ">";
  oss << "<WORD>" << search.getWord() << "</WORD>";
  oss << "<CONTENT>" << search.getContent() << "</CONTENT>";
  if (search.getPathDir() != "") {
    oss << "<PATHDIR>" << search.getPathDir() << "</PATHDIR>";
  }
  if (search.getPerm() != "") {
    oss << "<PERM>" << search.getPerm() << "</PERM>";
  }
  if (search.getExtension() != "") {
    oss << "<EXTENSION>" << search.getExtension() << "</EXTENSION>";
  }
  if (search.timeSlot()) {
    oss << "<TIMESLOT><BEGIN>";
    oss << "<DAY>" << search.getBeginDay() << "</DAY>";
    oss << "<MONTH>" << search.getBeginMonth() << "</MONTH>";
    oss << "<YEAR>" << search.getBeginYear() << "</YEAR>";
    oss << "</BEGIN><END>";
    oss << "<DAY>" << search.getEndDay() << "</DAY>";
    oss << "<MONTH>" << search.getEndMonth() << "</MONTH>";
    oss << "<YEAR>" << search.getEndYear() << "</YEAR>";
    oss << "</END></TIMESLOT>";
  }
  oss << "</SEARCH>";
  return oss.str();
}
