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

Search XMLParser::InterpretRequest(string xmlStream) {

  Search *research;
  string word, pathDir, perm, ext;
  Date *begin, *end, *tmp;
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

  research = new Search(word, content, pathDir, perm, ext, begin, end);
  cout << "word : " << (*research).getWord() << endl;
  cout << "content : " << (*research).getContent() << endl;
  cout << "pathdir : " << (*research).getPathDir() << endl;
  cout << "perm : " << (*research).getPerm() << endl;
  cout << "extension : " << (*research).getExtension() << endl;
  cout << "date debut : " << (*research).getBeginDay() << "/" << (*research).getBeginMonth() << "/" << (*research).getBeginYear() << endl;
  cout << "date fin : " << (*research).getEndDay() << "/" << (*research).getEndMonth() << "/" << (*research).getEndYear() << endl;

  return *research;
}

int main() {
  ostringstream oss;

  oss << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << endl;
  oss << "<SEARCH><WORD>MOT</WORD><CONTENT>true</CONTENT><PATHDIR>/home/matthieu</PATHDIR><PERM>-rw-rw-rw-</PERM><EXTENSION>txt</EXTENSION>";
  oss << "<TIMESLOT><BEGIN><DAY>17</DAY><MONTH>mars</MONTH><YEAR>2013</YEAR></BEGIN><END><DAY>17</DAY><MONTH>mai</MONTH><YEAR>2013</YEAR></END></TIMESLOT></SEARCH>";

  XMLParser p;
  p.InterpretRequest(oss.str().c_str());
  return 0;
}

