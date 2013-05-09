#include "XMLParser.h"

using namespace std;

Date* XMLParser::GetDate(TiXmlNode *nodeDepth2) {
  int day, year;
  string month;
  const char* tagName;
  TiXmlElement *tagDepth3;
  TiXmlNode *nodeDepth3;

  tagDepth3 = nodeDepth2->FirstChildElement();
  while (tagDepth3) {

    nodeDepth3 = tagDepth3->Clone();
    tagName = nodeDepth3->Value();

    if (strcmp(tagName, "DAY") == 0) {
      day = u.StringToInt(tagDepth3->GetText());
    }
    else if (strcmp(tagName, "MONTH") == 0) {
      month = tagDepth3->GetText();
    }
    else if (strcmp(tagName, "YEAR") == 0) {
      year = u.StringToInt(tagDepth3->GetText());
    }
    tagDepth3 = tagDepth3->NextSiblingElement();
  }
  Date *d = new Date(day, month, year);
  return d;
}

Search XMLParser::InterpretSearch(string xmlStream) {

  Search *research;
  string word = "", pathDir = "", perm = "", ext = "";
  Date *begin = new Date(), *end = new Date(), *tmp;
  bool content = false;

  TiXmlDocument doc;
  // current principal tag
  TiXmlElement *tagDepth1;
  // tags in current file tag
  TiXmlElement *tagDepth2;
  // node of depth 1
  TiXmlNode *nodeDepth1;
  // node of depth2
  TiXmlNode *nodeDepth2;

  const char* tagName;

  // on parse le flux xml donne en parametre sous forme de const char*
  doc.Parse(xmlStream.c_str(), 0, TIXML_ENCODING_UTF8);
    
  TiXmlHandle hdl(&doc);
  // on recupere la balise SEARCH
  hdl = hdl.FirstChildElement("SEARCH").FirstChildElement();
  // et on regarde ses elements
  tagDepth1 = hdl.Element();

  if(!tagDepth1) {
    return Search(NULL, false, NULL, NULL, NULL, NULL, NULL);
  }

  while (tagDepth1) {
    nodeDepth1 = tagDepth1->Clone();
    tagName = nodeDepth1->Value();

    if (strcmp(tagName, "WORD") == 0) {
      word = tagDepth1->GetText();
    }
    else if (strcmp(tagName, "CONTENT") == 0) {
      if (strcmp(tagDepth1->GetText(),"true") == 0) {
	content = true;
      }
    }
    else if (strcmp(tagName, "PATHDIR") == 0) {
      pathDir = tagDepth1->GetText();
    }
    else if (strcmp(tagName, "PERM") == 0) {
      perm = tagDepth1->GetText();
    }
    else if (strcmp(tagName, "EXTENSION") == 0) {
      ext = tagDepth1->GetText();
    }
    else if (strcmp(tagName, "TIMESLOT") == 0) {

      tagDepth2 = nodeDepth1->FirstChildElement();
      while (tagDepth2) {

	nodeDepth2 = tagDepth2->Clone();
	tagName = nodeDepth2->Value();

	if (strcmp(tagName, "BEGIN") == 0) {
	  tmp = GetDate(nodeDepth2);
	  begin = new Date((*tmp).getDay(), (*tmp).getMonth(), (*tmp).getYear());
	}
	else if (strcmp(tagName, "END") == 0) {
	  tmp = GetDate(nodeDepth2);
	  end = new Date((*tmp).getDay(), (*tmp).getMonth(), (*tmp).getYear());
	}

	tagDepth2 = tagDepth2->NextSiblingElement();
      }
    }

    tagDepth1 = tagDepth1->NextSiblingElement();
  }

  // Ne respecte pas vraiment la DTD, mais me parait plus logique
  if (!(*begin).IsValidDate()) {
    begin = new Date();
  }
  if (!(*end).IsValidDate()) {
    end = new Date();
  }

  research = new Search(word, content, pathDir, perm, ext, begin, end);
  return *research;
}

list<Result> XMLParser::InterpretResult(string xmlStream) {
  Result *tmpResult;
  string n, pa, pe, lm, pr, tSize;
  unsigned int s;
  istringstream iss;
  list<Result> results;

  TiXmlDocument doc;
  // current tag file
  TiXmlElement *tagDepth1;
  // tags in current file tag
  TiXmlElement *tagDepth2;
  // node of current tag file
  TiXmlNode *nodeDepth1;
  // node of the read tag in the second while loop
  TiXmlNode *nodeDepth2;

  const char* tagName;

  // on parse le flux xml donne en parametre sous forme de const char*
  doc.Parse(xmlStream.c_str(), 0, TIXML_ENCODING_UTF8);
    
  TiXmlHandle hdl(&doc);
  // on recupere la premiere balise FILE dans la premier balise RESULT
  hdl = hdl.FirstChildElement("RESULT").FirstChildElement("FILE");
  // et on la met dans un TiXmlElement
  tagDepth1 = hdl.Element();

  if(!tagDepth1){
    cout << "le noeud a atteindre n'existe pas" << endl;
    //mettre un return null ou liste/vector vide
    return results;
  }

  // parcours des balises <FILE></FILE>, 1 iteration par balise
  while (tagDepth1) {
    // on recupere le noeud correspondant a la balise FILE pour parcourir son
    // contenu
    nodeDepth1 = tagDepth1->Clone();
    // on recuperer la premiere balise dans le FILE courant
    tagDepth2 = nodeDepth1->FirstChildElement();

    n = "";
    pa = "";
    pe = "";
    lm = "";
    pr = "";
    tSize = "";

    // parcours du contenu de la balise <FILE></FILE> de l'iteration courrante
    while (tagDepth2) {
    
      // on recupere le noeud correspondant a la balise qui est actuellement lue
      nodeDepth2 = tagDepth2->Clone();
      
      // name of the tag
      tagName = nodeDepth2->Value();
      
      if (strcmp(tagName, "NAME") == 0) {
        n = tagDepth2->GetText();
      }
      else if (strcmp(tagName, "PATH") == 0) {
        pa = tagDepth2->GetText();
      }
      else if (strcmp(tagName, "PERM") == 0) {
        pe = tagDepth2->GetText();
      }
      else if (strcmp(tagName, "SIZE") == 0) {
	tSize = tagDepth2->GetText();
	istringstream iss(tSize);
	iss >> s;
      }
      else if (strcmp(tagName, "LASTMODIF") == 0) {
        lm = tagDepth2->GetText();
      }
      else if (strcmp(tagName, "PROPRIO") == 0) {
        pr = tagDepth2->GetText();
      }
      
      tagDepth2 = tagDepth2->NextSiblingElement();
      
    }
    if (n != "" && pa != "" && pe != "") {
      tmpResult = new Result(n, pa, pe, s, lm, pr);
      results.push_back(*tmpResult);
    }
    
    tagDepth1 = tagDepth1->NextSiblingElement("FILE");
  }

  return results;
}

int main() {
  ostringstream oss;
  XMLParser p;

  // ----------------------------------------------------
  // Test de Search
  
  oss << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << endl;
  oss << "<SEARCH><WORD>MOT</WORD><CONTENT>true</CONTENT><PATHDIR>/home/matthieu</PATHDIR><PERM>-rw-rw-rw-</PERM><EXTENSION>txt</EXTENSION>";
  oss << "<TIMESLOT><BEGIN><DAY>17</DAY><MONTH>mars</MONTH><YEAR>2013</YEAR></BEGIN><END><DAY>17</DAY><MONTH>mai</MONTH><YEAR>2013</YEAR></END></TIMESLOT></SEARCH>";

  Search research = p.InterpretSearch(oss.str().c_str());

  cout << "word : " << (research).getWord() << endl;
  cout << "content : " << (research).getContent() << endl;
  cout << "pathdir : " << (research).getPathDir() << endl;
  cout << "perm : " << (research).getPerm() << endl;
  cout << "extension : " << (research).getExtension() << endl;
  cout << "date debut : " << (research).getBeginDay() << "/" << (research).getBeginMonth() << "/" << (research).getBeginYear() << endl;
  cout << "date fin : " << (research).getEndDay() << "/" << (research).getEndMonth() << "/" << (research).getEndYear() << endl;
  
  // ----------------------------------------------------


  // ----------------------------------------------------
  // Test de Result
  /*
  oss << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << endl;
  oss << "<RESULT>";

  oss << "<FILE>";
  oss << "<NAME>toto.txt</NAME><PATH>/home/thomas/</PATH><PERM>-rw-rw-r--</PERM><SIZE>756</SIZE><LASTMODIF>mai 8 18:05</LASTMODIF>";
  oss << "</FILE>";
  
  oss << "<FILE>";
  oss << "<NAME>titi.txt</NAME><PATH>/home/matthieu/</PATH><PERM>-rw-rw-rw-</PERM><SIZE>3092</SIZE><LASTMODIF>mai 8 18:05</LASTMODIF><PROPRIO>matthieu</PROPRIO>";
  oss << "</FILE>";

  oss << "<FILE>";
  oss << "<NAME>titi.txt</NAME><PATH>/home/matthieu/</PATH><PERM>-rw-rw-rw-</PERM><SIZE>756</SIZE><PROPRIO>matthieu</PROPRIO>";
  oss << "</FILE>";
  
  oss << "</RESULT>";
  
  list<Result> rs = p.InterpretResult(oss.str().c_str());
  
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
  */
  // ----------------------------------------------------

  return 0;
}

