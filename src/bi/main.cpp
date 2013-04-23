//
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
#include <sqlite3.h>
#include "SQLite3DB.h"
#include "SQLQuery.h"
#include "AQuery.h"
#define REF_DATA_BASE_PATH "active.db"  // par defaut on cree la db sur place
int main(int argc, const char * argv[])
{
  
  std::string rq[10] ;
  rq[0] = "CREATE TABLE AnyFile \
    (id_file INT PRIMARY KEY, id_parent INT REFERENCES AnyFile (id_file), path VARCHAR(256) NOT NULL, \
    name VARCHAR(64) NOT NULL, mime_type VARCHAR(16) NOT NULL, extension VARCHAR(8) NOT NULL, mask_mode INT NOT NULL, \
    nb_hard_link TINYINT, user_id UNSIGNED INT, group_id UNSIGNED INT, disk_size INT, created_date DATETIME NOT NULL, \
    last_access_time DATETIME, last_modif_time DATETIME, last_stat_time DATETIME, hash_gen VARCHAR(64) UNIQUE NOT NULL, \
    insert_date DATETIME NOT NULL, user_flags VARCHAR(128));";
  
  rq[1] = "CREATE TABLE Audio (id_audio INT REFERENCES AnyFile (id_file),\
                                    artist VARCHAR(256),\
                                    title VARCHAR(256),\
                                    album_title VARCHAR(256),\
                                    year TINYINT(8),\
                                    duration TIME,\
                                    genre VARCHAR(64)\
                                    );";
  rq[2] = "CREATE TABLE Video (id_video INT REFERENCES AnyFile (id_file),\
                                   title VARCHAR(256),\
                                   duration INT,\
                                   year INT\
                                   );";
  rq[3] = "CREATE TABLE Image (id_image INT REFERENCES AnyFile (id_file),\
                                   width INT,\
                                   height INT\
                                   color_avg    INT\
                                   );";
  rq[4] = "CREATE TABLE Text (id_text INT REFERENCES AnyFile (id_file),\
                                   nd_word INT,\
                                   nd_line INT\
                                   nb_page INT\
                                   );";
  rq[5] = "CREATE TABLE Directory (id_dir INT REFERENCES AnyFile (id_file),\
                                   nb_file INT\
                                   );";
  rq[6] = "CREATE TABLE Archive (id_arch INT REFERENCES AnyFile (id_file),\
                                   nb_file INT\
                                   compress INT\
                                   need_pswd INT\
                                   );";
  rq[7] = "CREATE TABLE Object (id_object INT REFERENCES AnyFile (id_file),\
                                   title VARCHAR(256),\
                                   subject VARCHAR(64),\
                                   author VARCHAR(64),\
                                   nb_page INT\
                                   ); "; 
  SQLite3DB db;
  db.open(REF_DATA_BASE_PATH);
  SQLQuery q;
  q.setDataBase(&db);
    for (int i = 0; i < 8; i++) {
      q.setQuery(rq[i]);
      try {
        q.prepare();
        q.perform();
      } catch (SQLite3DBException e) {
        cout << e.getMessage() << endl;
    }
  
  }
  q.reset();
  db.close();
  std::cout << "Si seul ce texte est affiche... Nice tout s'est bien deroule la base devrait être cree avec les bases !" << std::endl;
    return 0;
}

