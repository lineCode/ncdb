#include "StdAfx.h"
#include "PAMonitorPrc.h"
#include "MDHandle.h"

CPAMonitorPrc::CPAMonitorPrc(void)
{
}


CPAMonitorPrc::~CPAMonitorPrc(void)
{
}

void CPAMonitorPrc::InitTask()
{
    AddMonitor(new CMDHandleSai);
    AddMonitor(new CMDHandlePA);
    AddMonitor(new CMDHandlePS);
    //AddMonitor(m_pHDA);
    
}

bool CPAMonitorPrc::UnInit()
{
    //释放添加的所有监测对象的内存,delete
    return true;
}
