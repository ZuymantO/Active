// Gere les communication xml

#ifndef __XmlCommunication__
#define __XmlCommunication__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "../daemon/ANotifyEvent.h"
#include "../bi/AnyFile.h"
#include "../mr/mr.h"

const std::string EN_TETE = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
int id = 0;

class XmlCommunication {
 public :
  /*
   * Envoie d'informations du moteur d'indexation a la base d'indexation
   * (donc d'une modification sur l'arborescence surveillee)
   */
  std::string MIToBI(ANotifyEvent ane);
  /*
   * Envoie d'informations de la base d'indexation au moteur de recherche
   * (donc, normalement le resultat de la recherche)
   */
  std::string BIToMR(vector<AnyFile>* AQResult, int searchID);
  /*
   * Envoie d'informations du moteur de recherche vers la base d'indexation
   * (donc, normalement une recherche)
   */
  std::string MRToBI(MR mr, int searchID);
};

#endif
