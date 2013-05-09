#include <string>
#include <sstream>
#include <list>
#include <tinyxml.h>

#include "Search.h"
#include "Result.h"

using namespace std;

class XMLParser {
 private :
  Utils u;

 public :
  Search InterpretSearch(string xmlStream);
  list<Result> InterpretResult(string xmlStream);
  // permet d'extraire une date a partir d'un noeud
  Date* GetDate(TiXmlNode *nodeDepth2);
};
