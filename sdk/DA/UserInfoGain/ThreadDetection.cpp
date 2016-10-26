#include "StdAfx.h"
#include "ThreadDetection.h"
#include "PAMonitorPrc.h"
#include "EventLight.h"
#include "ResCommon.h"

CThreadDetection::CThreadDetection( CallBackFun pFun /*= NULL*/, void* pAdd /*= NULL*/ )
    :CThreadLight(pFun, pAdd)
{
    m_nDetectPeriodMS       = 4 * 1000;
    m_bWillStop             = false;
}


CThreadDetection::~CThreadDetection(void)
{
    this->Stop();
}

bool CThreadDetection::Init()
{
    CThreadLight::Init();
    Create_Auto_Ptr(m_pLockDetect, CEventLight);

    CThreadLight::SetDelayMax(m_nDetectPeriodMS);
    return true;
    
}


DWORD CThreadDetection::DoWork()
{
    Create_Auto_Ptr(m_pMonitor, CPAMonitorPrc);
    m_pMonitor->Init();
    bool bEnd = false;
    while(1)
    {
        m_pMonitor->DetectAllProcess();

        CThreadLight::WaitTime();

        if(IsWillStopThread())
        {
            break;
        }
    }
    m_pMonitor->UnInit();
    return 1;
}


bool CThreadDetection::Start()
{
    this->Run();
    return true;
}

bool CThreadDetection::Stop()
{
    CThreadLight::ExitThread();
    return true;

}

bool CThreadDetection::SetPeriod( int nSecond )
{
    m_nDetectPeriodMS = nSecond;
    return true;

}
