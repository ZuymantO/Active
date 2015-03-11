//
//  ANotifyException.h
//  ANotify
//
//  Created by J. Charles N. MBIADA on 10/03/13.
//  Copyright (c) 2013 Active All rights reserved.
//
// TODO 1.
/*********************************************************************
 *
 *
 *
 *
 *
 *
 **********************************************************************/

#ifndef __ANotify__ANotifyException__
#define __ANotify__ANotifyException__

#include <iostream>
class ANotifyException
{
  
private:
protected:
  std::string m_msg;
  int m_err;
  mutable void* m_pSrc;
  
public:
  
  ANotifyException(const std::string& irMsg = "", int iErr = 0, void* ipSrc = NULL) : m_msg(irMsg), m_err(iErr) {
    m_pSrc = ipSrc;
  }
  
  inline const std::string& GetMessage() const{
  return m_msg;
  }
  
  inline int GetErrorNumber() const{
  return m_err;
  }

  
  inline void* GetSource() const{
  return m_pSrc;
  }
  
};


#endif /* defined(__ANotify__ANotifyException__) */
