/******************************************************************
*
*  CyberLink for C++
*
*  Copyright (C) Satoshi Konno 2002-2003
*
*  File: Disposer.cpp
*
*  Revision;
*
*  01/06/04
*    - first revision
*
******************************************************************/

#include <cybergarage/upnp/ControlPoint.h>
#include <cybergarage/upnp/device/Disposer.h>

using namespace CyberLink;

////////////////////////////////////////////////
//  Thread
////////////////////////////////////////////////
  
void Disposer::run()  {
  ControlPoint *ctrlp = getControlPoint();
  long monitorInterval = ctrlp->getExpiredDeviceMonitoringInterval() * 1000;
  
  while (isRunnable() == true) {
      if(m_timeUtil.Wait(monitorInterval)){
        ctrlp->removeExpiredDevices();
      }
    //ctrlp->print();
  }
}

bool Disposer::stop(){
    m_timeUtil.Stop();
    return Thread::stop();
}