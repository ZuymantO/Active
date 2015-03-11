//
//  SQLite3DBException.h
//  BI
//
//  Created by J. Charles N. MBIADA on 22/04/13.
//  Copyright (c) 2013 Sylorion. All rights reserved.
//

#ifndef __BI__SQLite3DBException__
#define __BI__SQLite3DBException__

#include <iostream>

namespace asqlite {

using namespace std;
class SQLite3DBException {
  
private:
protected:
  std::string m_msg;
  int m_err;
  mutable void* m_pSrc;
  
public:
  
  SQLite3DBException(const std::string& irMsg = "", int iErr = 0, void* ipSrc = NULL) : m_msg(irMsg), m_err(iErr) {
    m_pSrc = ipSrc;
  }

  //~SQLite3DBException() throw() {};  // util si on etend d'exception 
  inline const std::string& getMessage() const{
    return m_msg;
  }
  
  inline int getErrorNumber() const{
    return m_err;
  }
  
  
  inline void* getSource() const{
    return m_pSrc;
  }

};

}
#endif /* defined(__BI__SQLite3DBException__) */
