#include "Result.h"
#include <tinyxml.h>

Result::Result(string n, string pa, string pe, unsigned int s, string lm, string pr) {
  this->init(n, pa, pe, s, lm, pr);
}

Result::Result(string n, string pa, string pe, unsigned int s) {
  this->init(n, pa, pe, s, NULL, NULL);
}

Result::~Result() {}

void Result::init(string n, string pa, string pe, unsigned int s, string lm, string pr) {
  this->name = n;
  this->path = pa;
  this-> perm = pe;
  this->size = s;
  this->lastModif = lm;
  this->proprio = pr;
}

string Result::getName() const {
  return this->name;
}

string Result::getPath() const {
  return this->path;
}

string Result::getPerm() const {
  return this->perm;
}

unsigned int Result::getSize() const {
  return this->size;
}

string Result::getLastModif() const {
  return this->lastModif;
}

string Result::getProprio() const {
  return this->proprio;
}

/*
void Result::setName(string n) {
  this->name = n;
}

void Result::setPath(string p) {
  this->path = p;
}

void Result::setPerm(string p) {
  this->perm = p;
}

void Result::setSize(unsigned int s) {
  this->size = s;
}

void Result::setLastModif(string lm) {
  this->lastModif = lm;
}

void Result::setProprio(string p) {
  this->proprio = p;
}
*/

list<Result> SearchResults(string xmlStream) {

  Result *tmpResult;
  string n, pa, pe, lm, pr, tSize;
  unsigned int s;
  istringstream iss;
  list<Result> results;

  TiXmlDocument doc;
  // current tag file
  TiXmlElement *cTagFile;
  // tags in current file tag
  TiXmlElement *cTagInTagFile;
  // node of current tag file
  TiXmlNode *nodeCTagFile;
  // node of the read tag in the second while loop
  TiXmlNode *currentNode;

  const char* tagName;

  // on parse le flux xml donne en parametre sous forme de const char*
  doc.Parse(xmlStream.c_str(), 0, TIXML_ENCODING_UTF8);
    
  TiXmlHandle hdl(&doc);
  // on recupere la premiere balise FILE dans la premier balise RESULT
  hdl = hdl.FirstChildElement("RESULT").FirstChildElement("FILE");
  // et on la met dans un TiXmlElement
  cTagFile = hdl.Element();

  if(!cTagFile){
    cout << "le noeud a atteindre n'existe pas" << endl;
    //mettre un return null ou liste/vector vide
    return results;
  }

  // parcours des balises <FILE></FILE>, 1 iteration par balise
  while (cTagFile) {
    // on recupere le noeud correspondant a la balise FILE pour parcourir son
    // contenu
    nodeCTagFile = cTagFile->Clone();
    // on recuperer la premiere balise dans le FILE courant
    cTagInTagFile = nodeCTagFile->FirstChildElement();

    n = "";
    pa = "";
    pe = "";
    lm = "";
    pr = "";
    tSize = "";

    // parcours du contenu de la balise <FILE></FILE> de l'iteration courrante
    while (cTagInTagFile) {
    
      // on recupere le noeud correspondant a la balise qui est actuellement lue
      currentNode = cTagInTagFile->Clone();
      
      // name of the tag
      tagName = currentNode->Value();
      
      if (strcmp(tagName, "NAME") == 0) {
        n = cTagInTagFile->GetText();
      }
      else if (strcmp(tagName, "PATH") == 0) {
        pa = cTagInTagFile->GetText();
      }
      else if (strcmp(tagName, "PERM") == 0) {
        pe = cTagInTagFile->GetText();
      }
      else if (strcmp(tagName, "SIZE") == 0) {
	tSize = cTagInTagFile->GetText();
	istringstream iss(tSize);
	iss >> s;
      }
      else if (strcmp(tagName, "LASTMODIF") == 0) {
        lm = cTagInTagFile->GetText();
      }
      else if (strcmp(tagName, "PROPRIO") == 0) {
        pr = cTagInTagFile->GetText();
      }
      
      cTagInTagFile = cTagInTagFile->NextSiblingElement();
      
    }
    if (n != "" && pa != "" && pe != "") {
      tmpResult = new Result(n, pa, pe, s, lm, pr);
      results.push_back(*tmpResult);
    }
    
    cTagFile = cTagFile->NextSiblingElement("FILE");
  }

  return results;

}

int main() {
  
  ostringstream oss;

  oss << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << endl;
  oss << "<RESULT>";

  oss << "<FILE>";
  oss << "<NAME>toto.txt</NAME><PATH>/home/thomas/</PATH><PERM>-rw-rw-r--</PERM><SIZE>756</SIZE><LASTMODIF>mai 8 18:05</LASTMODIF>";
  oss << "</FILE>";
  
  oss << "<FILE>";
  oss << "<NAME>titi.txt</NAME><PATH>/home/matthieu/</PATH><PERM>-rw-rw-rw-</PERM><SIZE>756</SIZE><LASTMODIF>mai 8 18:05</LASTMODIF><PROPRIO>matthieu</PROPRIO>";
  oss << "</FILE>";

  oss << "<FILE>";
  oss << "<NAME>titi.txt</NAME><PATH>/home/matthieu/</PATH><PERM>-rw-rw-rw-</PERM><SIZE>756</SIZE><PROPRIO>matthieu</PROPRIO>";
  oss << "</FILE>";
  
  oss << "</RESULT>";
  
  list<Result> rs = SearchResults(oss.str().c_str());
  std::list<Result>::const_iterator lit(rs.begin()), lend(rs.end());
  for (; lit != lend; ++lit) {
    cout << "name : " << lit->getName() << endl;
    cout << "path : " << lit->getPath() << endl;
    cout << "perm : " << lit->getPerm() << endl;
    cout << "size : " << lit->getSize() << endl;
    cout << "lastmodif : ";
    if (lit->getLastModif() != "") {
      cout << lit->getLastModif();
    } else {
      cout << "NULL";
    }
    cout << endl;
    cout << "proprio : ";
    if (lit->getProprio() != "") {
      cout << lit->getProprio();
    } else {
      cout << "NULL";
    }
    cout << endl;
    cout << endl;
  }

  return 0;
}
