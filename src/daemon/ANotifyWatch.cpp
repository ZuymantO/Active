//
//  ANotifyWatch.cpp
//  ANotify
//
//  Created by J. Charles N. MBIADA on 17/03/13.
//  Copyright (c) 2013 Active All rights reserved.
//



#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "ANotifyWatch.h"

void ANotifyWatch::setEnabled(bool ibEnabled) throw (ANotifyException)
{

  if (ibEnabled == m_wEnabled) {
    return;
  }
  
  if (m_pANotify != NULL) {
    if (ibEnabled) {
      m_pANotify->add(this);
    }
    else {
      m_pANotify->remove(this);
      m_wDescr = -1;
    }
  }
  
  m_wEnabled = ibEnabled;
}

void ANotifyWatch::__disable()
{
  /* 
     TODO: decommenter tout ? (utilisation de IN_WRITE_BEGIN et END en
     dehors de ANotify)
  */

  /*IN_WRITE_BEGIN
  
  if (!m_wEnabled) {
    IN_WRITE_END_NOTHROW
    throw ANotifyException(IN_EXC_MSG("event cannot occur on disabled watch"), EINVAL, this);
  }
  
  if (m_pANotify != NULL) {
    (m_pANotify->getWatchesFDMap()).erase(m_wDescr);
    m_wDescr = -1;
  }
  
  m_wEnabled = false;
  
  IN_WRITE_END*/

}



