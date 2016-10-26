#include "StdAfx.h"
#include "SystemUseMonitor.h"
#include "MonitorSystemBase.h"
#include "MonitorCPU.h"
#include "MonitorMemory.h"
#include "define_gnl.h"
#define FOR_EACH_MONITOR(it) FOR_EACH_ITER(LST_MONITOR_SYS, m_lstMonitor, it)

CSystemUseMonitor::CSystemUseMonitor(void)
{
    m_nPeriodMS         = 3000;
}


CSystemUseMonitor::~CSystemUseMonitor(void)
{
}


bool CSystemUseMonitor::Init()
{
    AddMonitor(new CMonitorCPU);
    AddMonitor(new MonitorMemory);
    
    CThreadLight::Init();
    CThreadLight::SetDelayMax(m_nPeriodMS);
    //AddMonitor(new CMonitorCPU);
    
    return true;

}

bool CSystemUseMonitor::Start()
{
    //创建线程
    CThreadLight::Run();
    return true;
}

bool CSystemUseMonitor::AddNotifyWnd( WndID wID)
{
    FOR_EACH_MONITOR(it)
    {
        (*it)->AddNotifyWnd(wID);
    }
    return true;
}



bool CSystemUseMonitor::AddMonitor( CMonitorSystemBase* pBase )
{
    bool bRet = pBase->Init();
    m_lstMonitor.push_back(pBase);
    return bRet;

}

DWORD CSystemUseMonitor::DoWork()
{
    while(1)
    {
        FOR_EACH_MONITOR(it)
        {
            CMonitorSystemBase* pBase = (*it);
            bool bRet = pBase->IsDataValid();

            pBase->GetUseStatus();
            
            if (bRet)
            {
                pBase->Notify();
            }
            else
            {
                TRACE(_T("data invalid!\n"));
            }
        }

        CThreadLight::WaitTime();
        if (CThreadLight::IsWillStopThread())
        {
            break;
        }
    }
    
    return 1;
}

bool CSystemUseMonitor::Stop()
{
    CThreadLight::ExitThread();
    CThreadLight::Reset();
    return true;

}

