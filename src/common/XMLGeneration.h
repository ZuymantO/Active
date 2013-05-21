// Gere les communication XML

#ifndef __XML__Generation__
#define __XML__Generation__

#include <string>
#include <vector>

#include "../daemon/ANotifyMask.h"
#include "../daemon/ANotifyEvent.h"
#include "AnyFile.h"
#include "AQuery.h"
#include "Utils.h"
#include "Search.h"

namespace acommon {
  //const std::string EN_TETE = "<?XML version=\"1.0\" encoding=\"UTF-8\"?>";
  //int id = 0;

class XMLGeneration {
 private:
  /*static std::string EN_TETE = "<?XML version=\"1.0\" encoding=\"UTF-8\"?>";
    static int id = 0;*/
  static const std::string EN_TETE;
  static int id;

 public :
  /*
   * Envoie d'informations du moteur d'indexation a la base d'indexation
   * (donc d'une modification sur l'arborescence surveillee)
   */
  static std::string MIToBI(ANotifyEvent ane);
  /*
   * Envoie d'informations de la base d'indexation au moteur de recherche
   * (donc, normalement le resultat de la recherche)
   */

  static std::string BIToMR(AQuery& iraquery);
  /*
   * Envoie d'informations du moteur de recherche vers la base d'indexation
   * (donc, normalement une recherche)
   */
  static std::string MRToBI(Search search, int searchID);
};

}
#endif
