#pragma once
#include "monitorprocess.h"
class CMDHPAssist;
class CPAMonitorPrc :
    public CMonitorProcess
{
public:
    CPAMonitorPrc(void);
    ~CPAMonitorPrc(void);
public:
    bool SetPAHandle(CMDHPAssist* pHPA);
    virtual void InitTask();
    bool UnInit();
};

