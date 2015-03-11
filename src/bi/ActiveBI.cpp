
//  main.cpp
//  BI
//
//  Created by J. Charles N. MBIADA on 10/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//
//
//
//
// linker -lsqlite3

#include <iostream>
#include <map>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include "sqlite3.h"
#include "SQLite3DB.h"
#include "common.h"
#include "SQLQuery.h"

#define REF_DATA_BASE_PATH "active.db"  // par defaut on cree la db sur place

using namespace std;
using namespace asqlite;
using namespace acommon;

void printHelp(){
  cout << "================== Print HELP ==================" << endl;
};
bool checkOrCreateDB(SQLite3DB& irdb, const string& irname); // Check l'existence de la bdd et la cree sinon avec les tables
std::map<string, string>* paramsAssigner(const char* ippparams[], int nbparams); // creer la table des valeurs de parametre (nom, valeur)
void tests(SQLite3DB& db);

int main(int argc, const char * argv[])
{
  // =============================== GESTION des arguments ==================================
  map<string, string> r; // result
  for(int i = 0; i < argc; i++){
    if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0){
      printHelp();
      exit(0) ; // EXIT_SUCCESS
    }
    if(strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--port_mr") == 0){
      i++;
      r.insert(std::pair<string, string>(string("MR_PORT"), string(argv[i])));
    }
    if(strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--port_br") == 0){
      i++;
      r.insert(std::pair<string, string>(string("BR_PORT"), string(argv[i])));
    }
  }
  
  //================================ FIN GESTION DES ARGUMENTS ================================
  
   
  SQLite3DB db ; // Creation de la structure de la bd
  checkOrCreateDB(db, REF_DATA_BASE_PATH); // Verification d'existance d'une bd par defaut
  //*******************************
  try{
    db.open(REF_DATA_BASE_PATH);	// Ouverture de la base de donnee
  }catch (SQLite3DBException e) {
    cout << "Error : " << e.getMessage() << endl;
    exit(EXIT_FAILURE);
  }
  /*********************************
   * Debut du programme ou des tests
   * 
   ********************************/
  tests(db);
  //********************************
  db.close();	// Fermeture de la base de donnee
  std::cout << " Fermeture de la base d'indexation !" << std::endl;
  return EXIT_SUCCESS;
    
}

bool checkOrCreateDB(asqlite::SQLite3DB& irdb, const string& irname){

  bool exists = false;
  if(ifstream(irname.c_str()))		/*Le fichier existe*/
    exists = true;
  
  if(!exists){
    string rq[10] ;
    irdb.open(irname);
    rq[0] = "CREATE TABLE AnyFile \
    (id_file INT PRIMARY KEY, id_parent INT REFERENCES AnyFile (id_file), path VARCHAR(256) NOT NULL, \
    name VARCHAR(64) NOT NULL, mime_type VARCHAR(16) NOT NULL, extension VARCHAR(8) NOT NULL, mask_mode INT NOT NULL, \
    nb_hard_link TINYINT, user_id UNSIGNED INT, group_id UNSIGNED INT, disk_size INT, nb_file UNSIGNED INT DEFAULT NULL, created_date DATETIME NOT NULL, \
    last_access_time DATETIME, last_modif_time DATETIME, last_stat_time DATETIME, hash_gen VARCHAR(64) UNIQUE NOT NULL, \
    insert_date DATETIME NOT NULL, user_flags VARCHAR(128));";
  
    rq[1] = "CREATE TABLE Audio (id_audio INT REFERENCES AnyFile (id_file),\
                                    artists VARCHAR(256),\
                                    title VARCHAR(256),\
                                    album_title VARCHAR(256),\
                                    year TINYINT(8),\
                                    duration TIME,\
                                    genre VARCHAR(64)\
                                    );";
				    
    rq[2] = "CREATE TABLE Video (id_video INT REFERENCES AnyFile (id_file),\
                                   title VARCHAR(256),\
                                   realisators VARCHAR(256),\
                                   genre VARCHAR(32),\
                                   actors VARCHAR(256),\
                                   format TINYINT,\
                                   width TINYINT,\
                                   height TINYINT,\
                                   duration INT,\
                                   stereo_sound BOOLEAN,\
                                   year TINYINT,\
                                   fps TINYINT\
                                   );";
				   
    // Type BOOLEAN a verifie //
    rq[3] = "CREATE TABLE Image (id_image INT REFERENCES AnyFile (id_file),\
                                   width INT,\
                                   height INT\
                                   color_avg    INT\
                                   );";
				   
    rq[4] = "CREATE TABLE Text (id_text INT REFERENCES AnyFile (id_file),\
                                   nd_word INT,\
                                   nd_line INT\
                                   nb_page INT\
                                   keyword varchar(512)\
                                   );";
				   
    //  rq[5] = "CREATE TABLE Directory (id_dir INT REFERENCES AnyFile (id_file),\
    //                                   nb_file INT\
    //                                   );";
    // On vire ce type anyfile est de base dir avec le champ nb_file
    // Inutile de faire une table avec une colonne
    rq[5] = "CREATE TABLE Archive (id_arch INT REFERENCES AnyFile (id_file),\
                                   nb_file INT\
                                   compress INT\
                                   need_pswd INT\
                                   );";
				   
    rq[6] = "CREATE TABLE Object (id_object INT REFERENCES AnyFile (id_file),\
                                   title VARCHAR(256),\
                                   subject VARCHAR(64),\
                                   authors VARCHAR(64),\
                                   keyword varchar(512),\
                                   nb_page INT\
                                   ); "; 
  
  
    asqlite::SQLQuery q;
    q.setDataBase(&irdb);
    for (int i = 0; i < 7; i++) {
      q.setQuery(rq[i]);
      try {
        q.prepare();
        q.perform();
      } catch (SQLite3DBException e) {
        cout << "Error occur on db creation : " << e.getMessage() << endl;
	return false;
      }
      cout << "Base de donnee cree avec les tables adequates" <<endl;
    }
    q.reset();
    irdb.close();
  }else{ // La base existe
    cout << "La base de donnee existe deja." <<endl;
  } // end else
  return true;
}

void tests(SQLite3DB& db){
  string fq("SELECT * FROM AnyFile;"); 
  AQuery actualQuery(SELECT, ANY); 
  asqlite::SQLQuery fquery(actualQuery.queryType(), actualQuery.queryObject());
  fquery.setDataBase(&db);
  fquery.setQuery(fq);
  fquery.alignWith(actualQuery);
  //cout << "AQuery  Type " << actualQuery.queryType() << endl;
 
  try {
    fquery.prepare();
    cout << "Requete prepare " << endl;
    fquery.perform();
  } catch (SQLite3DBException e) {
    cout << "Error occur on db creation : " << e.getMessage() << endl;
    exit(EXIT_FAILURE);
  }
  cout << " Requete effectue " << endl;
  if(fquery.hasNewResult()){ // La base de donnee a des resultats ?
    vector<map<string, string> >* r =  fquery.results();
    if(r->size() == 0) { // Aucun resultat apparemment
      cout << " Indique des resultats mais aucun resultats transmis " << endl;
      return ;
    }
      
    vector<map<string, string> >::iterator it = r->begin();
    while(it != r->end()){
      map<string, string> ar = *it; // actual row
      if(ar.size() == 0) { // Aucune donnee ? bizarre
	cout << " Aucune donnee sur cette ligne " << endl;
      }
      it++;
    }
  }else{
    cout << " La requete de selection \"" << fq << "\" n'a fournie aucune donnee " << endl;
  }
}

