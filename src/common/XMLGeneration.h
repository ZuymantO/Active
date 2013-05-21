// Gere les communication XML

#ifndef __XML__Generation__
#define __XML__Generation__

#include <string>
#include <vector>

#include "../daemon/ANotifyMask.h"
#include "../daemon/ANotifyEvent.h"
#include "AnyFile.h"
#include "AQuery.h"

namespace acommon {

const std::string EN_TETE = "<?XML version=\"1.0\" encoding=\"UTF-8\"?>";
int id = 0;

class XMLGeneration {
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

  std::string BIToMR(AQuery& iraquery, int searchID);
  /*
   * Envoie d'informations du moteur de recherche vers la base d'indexation
   * (donc, normalement une recherche)
   */
  /*
  std::string MRToBI(MR mr, int searchID);
  */
};

}
#endif
