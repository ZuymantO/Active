#ifndef __BASE_EXCEPTION_H__
#define __BASE_EXCEPTION_H__

#include <iostream>

class BaseException {  
 private:
  
 protected:
  std::string m_msg;
  int m_err;
  mutable void* m_pSrc;
  
 public:  
 BaseException(const std::string& irMsg = "", int iErr = 0, void* ipSrc = NULL) : m_msg(irMsg), m_err(iErr) {
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

#endif
