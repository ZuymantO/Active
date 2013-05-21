//
//  SQLQuery.h
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Active. All rights reserved.
//

#ifndef __BI__SQLQuery__
#define __BI__SQLQuery__

#include <iostream>
#include "sqlite3.h"
#include "common.h"
#include "SQLite3DBException.h"
#include "SQLite3DB.h"
#include <map>
#include <vector>
#include "../common/AQuery.h"
namespace asqlite {

using namespace std;
using namespace acommon;

class SQLQuery {  
private:
  vector<map<string, string> >*  SQLResult;
  bool hasResult;
  SQLite3DB* dataBase;
  string query;
  sqlite3_stmt* stmt;
  acommon::AQueryType aqt;
  acommon::AQueryObject aobj;
  const char*   rTail;
  
  void initSQLQuery(){
    hasResult = false;
    SQLResult = NULL;
    query = "";
    stmt  = NULL;
    rTail = NULL;
    aqt = acommon::UNKNOWT;
    aobj = acommon::ANY;
  };

  
  void fetch() throw (SQLite3DBException){
    SQLResult = new vector<map<string, string> >();
    int cols = sqlite3_column_count(stmt);
    int result = 0;
    do{
      result = sqlite3_step(stmt);
      if(result == SQLITE_ROW) {
        map<string, string> tmp;
        for(int col = 0; col < cols; col++){
          string name = sqlite3_column_name(stmt, col);
          string value = (char*)sqlite3_column_text(stmt, col);
          tmp[name] = value;
        }
        SQLResult->push_back(tmp);
      }else
        break;
    }while (true);
    sqlite3_finalize(stmt);
    hasResult = true;
  };

  acommon::AnyFile createFile(acommon::AQueryObject iAQO);
public:
  acommon::AQuery* getAQuery() throw (SQLite3DBException);
  
  SQLQuery(){
    initSQLQuery();
  }
  
  SQLQuery(SQLite3DB* ipdb){
    initSQLQuery();
    dataBase = ipdb;
  };

  SQLQuery(acommon::AQueryType iaqtype, acommon::AQueryObject iaqobj){
    initSQLQuery();
    aqt = iaqtype;
    aobj = iaqobj;
  }
  
  ~SQLQuery(){};

  void alignWith(AQuery& iraq){
    aqt = iraq.queryType();
    aobj = iraq.queryObject();
  }
  
  string getQuery(){
    return query;
  };
  void setQuery(const string &irstr){
    query = irstr;
  }
  sqlite3_stmt* getStatement(){
    return stmt;
  };
  void setDataBase(SQLite3DB* ipdb){
    dataBase = ipdb;
  }
  
  
  void prepare() throw (SQLite3DBException){
    if (dataBase != NULL && dataBase->getSQLDB() == NULL) {
      string str = "Please create a data base first ";
      str.append(sqlite3_errmsg(dataBase->getSQLDB()));
      str.append("\n****************\n").append(query.append("\n\n"));
      throw SQLite3DBException(str);
    }
    reset();

    if(sqlite3_prepare_v2(dataBase->getSQLDB(), query.c_str() , -1, &stmt, &rTail) != SQLITE_OK){
      string str = "Unable to prepare the query ";
      str.append(sqlite3_errmsg(dataBase->getSQLDB()));
      str.append("\n--------\n").append(query.append("\n\n"));
      throw SQLite3DBException(str);
    }
    if(stmt == NULL){
      string str = " Unknow error happen in prepare ";
      str.append(sqlite3_errmsg(dataBase->getSQLDB()));
      str.append("\n+++++++++++\n").append(query.append("\n\n"));
      throw SQLite3DBException(str);
    }
  }

  void setStatement(sqlite3_stmt* ipstmt){
    stmt = ipstmt;
  };
  
  void reset(){
    if (stmt==NULL) {
      return;
    }
    SQLResult = NULL;
    hasResult = false;
    if (aqt == acommon::SELECT) {
      sqlite3_reset(stmt);
    }
    
  }
  
  void perform() throw (SQLite3DBException){
    if (stmt == NULL) {
      string str = "Please prepare the statment before perform call ";
      str.append(sqlite3_errmsg(dataBase->getSQLDB()));
      str.append("\n========\n").append(query.append("\n\n"));
      throw SQLite3DBException(str);;
    }
    if(aqt != acommon::SELECT){
    if(sqlite3_step(stmt) != SQLITE_DONE)
      fprintf(stderr, "Impossible d'effectuer la requete %s : %s \n", query.c_str(), sqlite3_errmsg(dataBase->getSQLDB()));
    if(sqlite3_finalize(stmt) != SQLITE_OK)
      fprintf(stderr, "Impossible de finaliser la requete %s : %s \n", query.c_str(), sqlite3_errmsg(dataBase->getSQLDB()));
    }else{
      fetch();
    }
  }
  
    // Permet de retrouver les resultats d'une requetes deja prepare
  vector<map<string, string> >* result() {
    return SQLResult;
  };
  
};
}
#endif /* defined(__BI__SQLQuery__) */
