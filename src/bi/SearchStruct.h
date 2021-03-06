#ifndef __SEARCH_STRUCT_H__
#define __SEARCH_STRUCT_H__

class SearchStruct {
 private:
  /* Chaine a rechercher */
  std::string text;

  /* Repertoire ou effectuer la recherche */
  std::string srcDir;

  /* Les permissions sur le fichier */
  int permissions;
  
  /* Date de debut de l'intervalle */
  std::string beginDate;
  /* Date de fin de l'intervalle */
  std::string endDate;

  /* Booleen indiquant le type de recherche (contenu=true ou nom=false) */
  bool content;

  /* Modifie le texte a rechercher */
  void setText(std::string cnt);

  /* Modifie la date de debut de l'intervalle */
  void setBeginDate(int day, int month, int year);
  /* Modifie la date de fin de l'intervalle */
  void setEndDate(int day, int month, int year);

  SearchStruct();

 public:
  /* Transforme un code xml en une structure de recherche SearchStruct */
  static SearchStruct xmlToSearchStruct(std::string xml);

  /* Indique si la recherche est une recherche par contenu ou par nom */
  bool isContent();

  /* Renvoie le texte a rechercher */
  std::string getText();

  /* Renvoie la date de debut d'intervalle */
  std::string getBeginDate();
  /* Renvoie la date de fin d'intervalle */
  std::string getEndDate();

  /* Transforme la structure en une requete sqlite3 */
  std::string toQuery();

  ~SearchStruct();
};

#endif
