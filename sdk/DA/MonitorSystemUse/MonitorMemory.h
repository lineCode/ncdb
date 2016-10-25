#pragma once
#include "monitorsystembase.h"
class CHardwareQuery;

class MonitorMemory :
    public CMonitorSystemBase
{
public:
    MonitorMemory(void);
    ~MonitorMemory(void);
public:
    virtual bool InitChild();
    virtual UseStatus GetUseStatus();
private:
    CHardwareQuery*         m_pHardWare;
    MONITOR_MEMORY_DATA     m_data;
};

