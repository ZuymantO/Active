#include "XMLParser.h"

using namespace std;

int main() {
  ostringstream oss;
  XMLParser p;

  // --------------------------------------------------------------------------
  // Test de Indexation
  // --------------------------------------------------------------------------

  cout << " ----------------------------------------------------" << endl;
  cout << "Test de Indexation" << endl;

  oss << "<?xml version='1.0' encoding='UTF-8'?>" << endl;
  
  oss << "<INDEXATION>";

  oss << "<RENOMMAGES>";
  oss << "<FICHIERRENOMME><PATH>TOTO</PATH><NEWPATH>TITI</NEWPATH></FICHIERRENOMME>";
  oss << "<FICHIERRENOMME><PATH>vlad</PATH><NEWPATH>geor</NEWPATH></FICHIERRENOMME>";
  oss << "</RENOMMAGES>";

  oss << "<MODIFICATIONS>";
  oss << "<FICHIERMODIFIE><PATH>TOTO</PATH><DATEMODIFICATION>08-08-13</DATEMODIFICATION><TAILLE>463</TAILLE><PROPRIETAIRE>MAT</PROPRIETAIRE><GROUPE>ROOT</GROUPE><PERMISSIONS>-r--r--r--</PERMISSIONS><INDEXAGE><MOT frequence=\"3\">coucou</MOT><MOT frequence=\"45\">SALUT</MOT></INDEXAGE><NEWPATH>TITI</NEWPATH></FICHIERMODIFIE>";
  oss << "<FICHIERMODIFIE><PATH>TATA</PATH><DATEMODIFICATION>08-08-13</DATEMODIFICATION><TAILLE>463</TAILLE><PROPRIETAIRE>MAT</PROPRIETAIRE><GROUPE>ROOT</GROUPE><PERMISSIONS>-r--r--r--</PERMISSIONS><INDEXAGE><MOT frequence=\"3\">coucou</MOT><MOT frequence=\"45\">SALUT</MOT><MOT frequence=\"45\">bouh</MOT></INDEXAGE></FICHIERMODIFIE>";
  oss << "</MODIFICATIONS>";

  oss << "<SUPPRESSIONS>";
  oss << "<FICHIERSUPPRIME><PATH>TOTO</PATH></FICHIERSUPPRIME>";
  oss << "<FICHIERSUPPRIME><PATH>TITI</PATH></FICHIERSUPPRIME>";
  oss << "</SUPPRESSIONS>";

  oss << "<CREATIONS>";
  oss << "<FICHIERCREE><PATH>TOTO</PATH><DATECREATION>08-08-13</DATECREATION><TAILLE>463</TAILLE><PROPRIETAIRE>MAT</PROPRIETAIRE><GROUPE>ROOT</GROUPE><PERMISSIONS>-r--r--r--</PERMISSIONS><INDEXAGE><MOT frequence=\"3\">coucou</MOT><MOT frequence=\"45\">SALUT</MOT></INDEXAGE><format>TITI</format></FICHIERCREE>";
  oss << "<FICHIERCREE><PATH>TATA</PATH><DATECREATION>08-08-13</DATECREATION><TAILLE>463</TAILLE><PROPRIETAIRE>MAT</PROPRIETAIRE><GROUPE>ROOT</GROUPE><PERMISSIONS>-r--r--r--</PERMISSIONS><INDEXAGE><MOT frequence=\"3\">coucou</MOT><MOT frequence=\"45\">SALUT</MOT><MOT frequence=\"45\">bouh</MOT></INDEXAGE></FICHIERCREE>";
  oss << "</CREATIONS>";

  oss << "</INDEXATION>";

  Indexation ind = p.InterpretIndexation(oss.str().c_str());
  
  list<Renommage> rs = ind.GetRenom();
  std::list<Renommage>::const_iterator lit(rs.begin()), lend(rs.end());
  for (; lit != lend; ++lit) {
    cout << "oldpath : " << lit->GetOldPath() << endl;
    cout << "newpath : " << lit->GetNewPath() << endl;
  }
  cout << "-----------------------------" << endl;
  cout << "-----------------------------" << endl;

  list<Modification> mf = ind.GetModif();
  std::list<Modification>::const_iterator lit2(mf.begin()), lend2(mf.end());
  for (; lit2 != lend2; ++lit2) {
    cout << "path : " << lit2->GetFile()->GetPath() << endl;
    cout << "date : " << lit2->GetFile()->GetDate() << endl;
    cout << "taille : " << lit2->GetFile()->GetTaille() << endl;
    cout << "prop : " << lit2->GetFile()->GetProprietaire() << endl;
    cout << "groupe : " << lit2->GetFile()->GetGroupe() << endl;
    cout << "perm : " << lit2->GetFile()->GetPerm() << endl;
    list<Indexage> li = lit2->GetFile()->GetIndex();
    std::list<Indexage>::const_iterator llit(li.begin()), llend(li.end());
    for (; llit != llend; ++llit) {
      cout << "le mot #" << llit->GetWord() << "# apparait : " << llit->GetOccurrence() << endl;
    }
    cout << "newpath : " << lit2->GetNewpath() << endl;
    cout << "-----------------------------" << endl;
  }

  cout << "-----------------------------" << endl;
  list<Suppression> ss = ind.GetSupp();
  std::list<Suppression>::const_iterator lit3(ss.begin()), lend3(ss.end());
  for (; lit3 != lend3; ++lit3) {
    cout << "path : " << lit3->GetPath() << endl;
  }
  cout << "-----------------------------" << endl;
  cout << "-----------------------------" << endl;

  list<Creation> cr = ind.GetCrea();
  cout << cr.size() << endl;
  std::list<Creation>::const_iterator lit4(cr.begin()), lend4(cr.end());
  for (; lit4 != lend4; ++lit4) {
    cout << "path : " << lit4->GetFile()->GetPath() << endl;
    cout << "date : " << lit4->GetFile()->GetDate() << endl;
    cout << "taille : " << lit4->GetFile()->GetTaille() << endl;
    cout << "prop : " << lit4->GetFile()->GetProprietaire() << endl;
    cout << "groupe : " << lit4->GetFile()->GetGroupe() << endl;
    cout << "perm : " << lit4->GetFile()->GetPerm() << endl;
    list<Indexage> li2 = lit4->GetFile()->GetIndex();
    std::list<Indexage>::const_iterator llit2(li2.begin()), llend2(li2.end());
    for (; llit2 != llend2; ++llit2) {
      cout << "le mot #" << llit2->GetWord() << "# apparait : " << llit2->GetOccurrence() << endl;
    }
    cout << "format : " << lit4->GetFormat() << endl;
    cout << "-----------------------------" << endl;
  }

  cout << " ----------------------------------------------------" << endl;

  // --------------------------------------------------------------------------
  // Test de Result
  // --------------------------------------------------------------------------
  oss.flush();
  oss << "<?xml version='1.0' encoding='UTF-8'?>" << endl;
  oss << "<RESULT id=\"34\">";

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
  
  Results r = p.InterpretResult(oss.str().c_str());
  cout << "id : " << r.GetID() << endl << endl;
  list<Result> rst = r.GetRes();
  std::list<Result>::const_iterator litR(rst.begin()), lendR(rst.end());
  for (; litR != lendR; ++litR) {
    cout << "name : " << litR->getName() << endl;
    cout << "path : " << litR->getPath() << endl;
    cout << "perm : " << litR->getPerm() << endl;
    cout << "size : " << litR->getSize() << endl;
    cout << "lastmodif : ";
    if (litR->getLastModif() != "") {
      cout << litR->getLastModif();
    } else {
      cout << "NULL";
    }
    cout << endl;
    cout << "proprio : ";
    if (litR->getProprio() != "") {
      cout << litR->getProprio();
    } else {
      cout << "NULL";
    }
    cout << endl;
    cout << endl;
  }

  // --------------------------------------------------------------------------
  // Test de Search
  // --------------------------------------------------------------------------
  oss.flush();
  oss << "<?xml version='1.0' encoding='UTF-8'?>" << endl;
  oss << "<SEARCH id=\"34\"><WORD>MOT</WORD><CONTENT>true</CONTENT><PATHDIR>/home/matthieu</PATHDIR><PERM>-rw-rw-rw-</PERM><EXTENSION>txt</EXTENSION>";
  oss << "<TIMESLOT><BEGIN><DAY>17</DAY><MONTH>mars</MONTH><YEAR>2013</YEAR></BEGIN><END><DAY>17</DAY><MONTH>mai</MONTH><YEAR>2013</YEAR></END></TIMESLOT></SEARCH>";

  Search *research = p.InterpretSearch(oss.str().c_str());

  cout << "id : " << (*research).getID() << endl;
  cout << "word : " << (*research).getWord() << endl;
  cout << "content : " << (*research).getContent() << endl;
  cout << "pathdir : " << (*research).getPathDir() << endl;
  cout << "perm : " << (*research).getPerm() << endl;
  cout << "extension : " << (*research).getExtension() << endl;
  cout << "date debut : " << (*research).getBeginDay() << "/" << (*research).getBeginMonth() << "/" << (*research).getBeginYear() << endl;
  cout << "date fin : " << (*research).getEndDay() << "/" << (*research).getEndMonth() << "/" << (*research).getEndYear() << endl;

  return 0;
}
