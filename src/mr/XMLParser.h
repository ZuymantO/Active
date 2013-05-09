#include <string>
#include <sstream>
#include <tinyxml.h>

#include "Search.h"

using namespace std;

class XMLParser {
 private :
  Utils u;

 public :
  Search InterpretRequest(string xmlStream);
  Date* GetDate(TiXmlNode *nodeDepth2);
};
