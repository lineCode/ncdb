#include "StdAfx.h"
#include "MonitorMemory.h"
#include "HardwareQuery.h"

MonitorMemory::MonitorMemory(void)
{
    ::memset(&m_data, 0, sizeof(MONITOR_MEMORY_DATA));
}


MonitorMemory::~MonitorMemory(void)
{
}

bool MonitorMemory::InitChild()
{
    m_pHardWare = new CHardwareQuery;
    m_pHardWare->Init();
    SetType(mdt_memory);
    SetDataValid(true); //内存每次获取的数据都是真实的
    return true;
}

UseStatus MonitorMemory::GetUseStatus()
{
    UseStatus us = 0;
    //m_UseStatus = m_pHardWare->GetMemoryUsePercent();
    m_data.nBytesTotal = m_pHardWare->GetMemoryPhysicalTotal();
    m_data.nBytesFree   = m_pHardWare->GetMemoryPhysicalAvailable();
    m_pMonitorResultData = &m_data;
    return us;
}
