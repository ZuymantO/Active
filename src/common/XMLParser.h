#ifndef _XMLParser__
#define _XMLParser__

#include <string>
#include <sstream>
#include <list>
#include <iostream>
//#include "tinyxml.h"
#include <tinyxml.h>

#include "Search.h"
#include "Result.h"
#include "Indexation.h"

#include "AQuery.h"
#include "AnyFile.h"

using namespace std;
using namespace acommon;

class XMLParser {
 private :
  Utils u;

 public :
  Search* InterpretSearch(string xmlStream);
  AQuery* InterpretResult(string xmlStream);
  Indexation InterpretIndexation(string xmlStream);
  // permet d'extraire une date a partir d'un noeud
  Date* GetDate(TiXmlNode *nodeDepth2);
  list<Renommage> GetRenommages(TiXmlNode *nodeDepth1);
  list<Modification> GetModifications(TiXmlNode *nodeDepth1);
  list<Suppression> GetSuppressions(TiXmlNode *nodeDepth1);
  list<Creation> GetCreations(TiXmlNode *nodeDepth1);
};

#endif
