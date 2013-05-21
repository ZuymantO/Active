//
//  SQLite3DB.h
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#ifndef __BI__SQLite3DB__
#define __BI__SQLite3DB__
#include <iostream>
#include "sqlite3.h"
#include "SQLite3DBException.h"
  
namespace asqlite {

using namespace std;

class SQLQuery;
class SQLite3DB {
private:
  sqlite3 *dataBase;
  string dbPath;
  string strErr;
  SQLQuery*  actualQuery;
  
public:
  SQLite3DB(){
    initSQLite3DB();
  };
  
  SQLite3DB(const string &irdbName){
    initSQLite3DB();
    dbPath = irdbName;
  };
  
//     
//  void query(){
//    
//  };
  
  sqlite3* getSQLDB(){
    return dataBase;
  }
  
  bool open(const string &irdbPathName) throw (SQLite3DBException){
    if (dbPath.length() == 0 && irdbPathName.length() == 0) {
      throw new SQLite3DBException(strErr.append(" Use open with a data base path valid name"));
    }
    bool b = false;
    if(irdbPathName.length() > 0)
      dbPath = irdbPathName;
    int raiseCondition  = sqlite3_open(dbPath.c_str(), &dataBase);
    b = raiseCondition == SQLITE_OK;
    if(!b){
      strErr = "Unable to open data base ";
      strErr.append(dbPath.append(" "));
      strErr.append(sqlite3_errmsg(dataBase));
      throw new SQLite3DBException(strErr);
    }
    return b;
  };
  
  bool open() throw (SQLite3DBException){
    return open("");
  }
  void initSQLite3DB(){
    dataBase = NULL;
    dbPath = "";
    strErr = "NO Data Base allocated: First, use SQLite3DB::open()";
    actualQuery = NULL;
  }

  void close(){
    sqlite3_close_v2(dataBase);
  }
  
  ~ SQLite3DB(){
    if(dataBase != NULL){
      sqlite3_close(dataBase);
    }
  };
  
};

}
#endif /* defined(__BI__SQLite3DB__) */
