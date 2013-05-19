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

Search* XMLParser::InterpretSearch(string xmlStream) {

  Search *research;
  string id ="", word = "", pathDir = "", perm = "", ext = "";
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
  hdl = hdl.FirstChildElement("SEARCH");
  tagDepth1 = hdl.Element();
  id = tagDepth1->Attribute("id");
  hdl = hdl.FirstChildElement();
  // et on regarde ses elements
  tagDepth1 = hdl.Element();

  if(!tagDepth1) {
    return (new Search(0, NULL, false, NULL, NULL, NULL, NULL, NULL));
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

  research = new Search(id, word, content, pathDir, perm, ext, begin, end);
  return research;
}

Results XMLParser::InterpretResult(string xmlStream) {
  Result *tmpResult;
  string id, n, pa, pe, lm, pr, tSize;
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
  hdl = hdl.FirstChildElement("RESULT");
  tagDepth1 = hdl.Element();
  id = tagDepth1->Attribute("id");
  hdl = hdl.FirstChildElement("FILE");
  // et on la met dans un TiXmlElement
  tagDepth1 = hdl.Element();

  if(!tagDepth1){
    cout << "le noeud a atteindre n'existe pas" << endl;
    //mettre un return null ou liste/vector vide
    list<Result> rtmp;
    Results *res = new Results("", rtmp);
    return *res;
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

  Results *res = new Results(id, results);
  return *res;
}

Indexation XMLParser::InterpretIndexation(string xmlStream) {
  Indexation *indexations;
  list<Renommage> tmpR;
  list<Modification> tmpM;
  list<Suppression> tmpS;
  list<Creation> tmpC;

  TiXmlDocument doc;
  // current tag file
  TiXmlElement *tagDepth1;
  // node of current tag file
  TiXmlNode *nodeDepth1;

  const char* tagName;

  doc.Parse(xmlStream.c_str(), 0, TIXML_ENCODING_UTF8);
    
  TiXmlHandle hdl(&doc);
  hdl = hdl.FirstChildElement("INDEXATION").FirstChildElement();
  tagDepth1 = hdl.Element();

  if (!tagDepth1) {
    indexations = new Indexation(tmpR, tmpM, tmpS, tmpC);
    return *indexations;
  }

  while (tagDepth1) {
    nodeDepth1 = tagDepth1->Clone();
    tagName = nodeDepth1->Value();
    if (strcmp(tagName, "RENOMMAGES") == 0) {
      tmpR = GetRenommages(nodeDepth1);
    }
    else if (strcmp(tagName, "MODIFICATIONS") == 0) {
      tmpM = GetModifications(nodeDepth1);
    }
    else if (strcmp(tagName, "SUPPRESSIONS") == 0) {
      tmpS = GetSuppressions(nodeDepth1);
    }
    else if (strcmp(tagName, "CREATIONS") == 0) {
      tmpC = GetCreations(nodeDepth1);
    }

    tagDepth1 = tagDepth1->NextSiblingElement();
  }

  indexations = new Indexation(tmpR, tmpM, tmpS, tmpC);
  return *indexations;
}

list<Renommage> XMLParser::GetRenommages(TiXmlNode *nodeDepth1) {
  list<Renommage> renoms;
  Renommage *tmpr;
  string oldp, newp;
  const char *tagName;
  TiXmlElement *tagDepth2, *tagDepth3;
  TiXmlNode *nodeDepth2, *nodeDepth3;

  tagDepth2 = nodeDepth1->FirstChildElement("FICHIERRENOMME");
  while (tagDepth2) {

    nodeDepth2 = tagDepth2->Clone();
    tagDepth3 = nodeDepth2->FirstChildElement();
    while (tagDepth3) {
      nodeDepth3 = tagDepth3->Clone();
      tagName = nodeDepth3->Value();

      if (strcmp(tagName, "PATH") == 0) {
	oldp = tagDepth3->GetText();
      }
      else if (strcmp(tagName, "NEWPATH") == 0) {
	newp = tagDepth3->GetText();
      }

      tagDepth3 = tagDepth3->NextSiblingElement();
    }
    if (newp != "" && oldp != "") {
      tmpr = new Renommage(oldp, newp);
      renoms.push_front(*tmpr);
    }
    oldp = "";
    newp = "";

    tagDepth2 = tagDepth2->NextSiblingElement("FICHIERRENOMME");
  }
  return renoms;
}

list<Modification> XMLParser::GetModifications(TiXmlNode *nodeDepth1) {

  list<Modification> modifs;
  Modification *tmpm;
  string path, date, prop, groupe, perm, newpath, w;
  unsigned int taille;
  int freq;
  Indexage *ind;
  list<Indexage> index;
  const char* tagName;
  TiXmlElement *tagDepth2, *tagDepth3, *tagDepth4;
  TiXmlNode *nodeDepth2, *nodeDepth3, *nodeDepth4;

  tagDepth2 = nodeDepth1->FirstChildElement("FICHIERMODIFIE");
  while(tagDepth2) {
    nodeDepth2 = tagDepth2->Clone();
    tagDepth3 = nodeDepth2->FirstChildElement();
    while (tagDepth3) {
      nodeDepth3 = tagDepth3->Clone();
      tagName = nodeDepth3->Value();

      if (strcmp(tagName, "PATH") == 0) {
	path = tagDepth3->GetText();
      }
      else if (strcmp(tagName, "DATEMODIFICATION") == 0) {
	date = tagDepth3->GetText();
      }
      else if (strcmp(tagName, "TAILLE") == 0) {
	taille = u.StringToInt(tagDepth3->GetText());
      }
      else if (strcmp(tagName, "PROPRIETAIRE") == 0) {
	prop = tagDepth3->GetText();
      }
      else if (strcmp(tagName, "GROUPE") == 0) {
	groupe = tagDepth3->GetText();
      }
      else if (strcmp(tagName, "PERMISSIONS") == 0) {
	perm = tagDepth3->GetText();
      }
      else if (strcmp(tagName, "INDEXAGE") == 0) {
	tagDepth4 = nodeDepth3->FirstChildElement("MOT");
	while (tagDepth4) {
	  nodeDepth4 = tagDepth4->Clone();
	  tagName = nodeDepth4->Value();
	  w = tagDepth4->GetText();
	  tagDepth4->QueryIntAttribute("frequence", &freq);
	  ind = new Indexage(w, freq);
	  index.push_front(*ind);
	  tagDepth4 = tagDepth4->NextSiblingElement("MOT");
	  w = "";
	  freq = 0;
	}
      }
      else if (strcmp(tagName, "NEWPATH") == 0) {
	newpath = tagDepth3->GetText();
      }
      tagDepth3 = tagDepth3->NextSiblingElement();
    }
    if (path != "" && date != "" && prop != "" && groupe != "" && perm != "" && !index.empty()) {
      tmpm = new Modification(path, date, taille, prop, groupe, perm, index, newpath);
      modifs.push_front(*tmpm);
    }
    path ="";
    date = "";
    prop = "";
    perm = "";
    newpath = "";
    index.clear();

    tagDepth2 = tagDepth2->NextSiblingElement("FICHIERMODIFIE");
  }

  return modifs;
}

list<Suppression> XMLParser::GetSuppressions(TiXmlNode *nodeDepth1) {
  list<Suppression> supps;
  Suppression *tmps;
  string path;
  const char *tagName;
  TiXmlElement *tagDepth2, *tagDepth3;
  TiXmlNode *nodeDepth2, *nodeDepth3;

  tagDepth2 = nodeDepth1->FirstChildElement("FICHIERSUPPRIME");
  while (tagDepth2) {

    nodeDepth2 = tagDepth2->Clone();
    tagDepth3 = nodeDepth2->FirstChildElement();
    while (tagDepth3) {
      nodeDepth3 = tagDepth3->Clone();
      tagName = nodeDepth3->Value();

      if (strcmp(tagName, "PATH") == 0) {
	path = tagDepth3->GetText();
	break;
      }
      tagDepth3 = tagDepth3->NextSiblingElement();
    }
    if (path != "") {
      tmps = new Suppression(path);
      supps.push_front(*tmps);
    }
    path = "";

    tagDepth2 = tagDepth2->NextSiblingElement("FICHIERSUPPRIME");
  }

  return supps;
}

list<Creation> XMLParser::GetCreations(TiXmlNode *nodeDepth1) {
  list<Creation> creas;
  Creation *tmpc;
  string path, date, prop, groupe, perm, format, w;
  unsigned int taille;
  int freq;
  Indexage *ind;
  list<Indexage> index;
  const char* tagName;
  TiXmlElement *tagDepth2, *tagDepth3, *tagDepth4;
  TiXmlNode *nodeDepth2, *nodeDepth3, *nodeDepth4;

  tagDepth2 = nodeDepth1->FirstChildElement("FICHIERCREE");
  while(tagDepth2) {
    nodeDepth2 = tagDepth2->Clone();
    tagDepth3 = nodeDepth2->FirstChildElement();
    while (tagDepth3) {
      nodeDepth3 = tagDepth3->Clone();
      tagName = nodeDepth3->Value();

      if (strcmp(tagName, "PATH") == 0) {
	path = tagDepth3->GetText();
      }
      else if (strcmp(tagName, "DATECREATION") == 0) {
	date = tagDepth3->GetText();
      }
      else if (strcmp(tagName, "TAILLE") == 0) {
	taille = u.StringToInt(tagDepth3->GetText());
      }
      else if (strcmp(tagName, "PROPRIETAIRE") == 0) {
	prop = tagDepth3->GetText();
      }
      else if (strcmp(tagName, "GROUPE") == 0) {
	groupe = tagDepth3->GetText();
      }
      else if (strcmp(tagName, "PERMISSIONS") == 0) {
	perm = tagDepth3->GetText();
      }
      else if (strcmp(tagName, "INDEXAGE") == 0) {
	tagDepth4 = nodeDepth3->FirstChildElement("MOT");
	while (tagDepth4) {
	  nodeDepth4 = tagDepth4->Clone();
	  tagName = nodeDepth4->Value();
	  w = tagDepth4->GetText();
	  tagDepth4->QueryIntAttribute("frequence", &freq);
	  ind = new Indexage(w, freq);
	  index.push_front(*ind);
	  tagDepth4 = tagDepth4->NextSiblingElement("MOT");
	  w = "";
	  freq = 0;
	}
      }
      else if (strcmp(tagName, "format") == 0) {
	format = tagDepth3->GetText();
      }
      tagDepth3 = tagDepth3->NextSiblingElement();
    }
    if (path != "" && date != "" && prop != "" && groupe != "" && perm != ""  && format != "" && !index.empty()) {
      tmpc = new Creation(path, date, taille, prop, groupe, perm, index, format);
      creas.push_front(*tmpc);
    }
    path ="";
    date = "";
    prop = "";
    perm = "";
    format = "";
    index.clear();

    tagDepth2 = tagDepth2->NextSiblingElement("FICHIERCREE");
  }

  return creas;
}
