#pragma once
#include "MonitorSystemDef.h"
#include "ThreadLight.h"
#include <list>
using namespace std;

class CMonitorSystemBase;


typedef list<CMonitorSystemBase*>   LST_MONITOR_SYS;

class CSystemUseMonitor
    :public CThreadLight
{
public:
    CSystemUseMonitor(void);
    ~CSystemUseMonitor(void);
public:
    bool Init();
    bool AddNotifyWnd(WndID);
    bool Start();
    bool Stop();
    //bool WaitFor( int nDetectPeriod );
    virtual DWORD DoWork();
    /**************************************** ∑Ω∑® ****************************************/
private:
    bool CreateThread();
    bool AddMonitor(CMonitorSystemBase* pBase);
    /****************************************  Ù–‘ ****************************************/
private:
    LST_MONITOR_SYS                 m_lstMonitor;
    int                             m_nPeriodMS;

};

