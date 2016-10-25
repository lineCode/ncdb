#pragma once
#include "monitorsystembase.h"
class CCpuState;

class CMonitorCPU :
    public CMonitorSystemBase
{
public:
    CMonitorCPU(void);
    ~CMonitorCPU(void);
public:
    virtual bool InitChild();
    virtual UseStatus GetUseStatus();
private:
    CCpuState*                  m_pCPU;
    MONITOR_CPU_DATA            m_data;
};

