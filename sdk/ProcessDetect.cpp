#include "StdAfx.h"
#include "ProcessDetect.h"
#include "EventLight.h"
#include "piprocess.h"
#include "ResCommon.h"
CProcessDetect::CProcessDetect( bool bDetectLunchy /*= false*/)
{
    m_bDetectLunchy = bDetectLunchy;
    m_DetectCircle = 500;
}


CProcessDetect::~CProcessDetect(void)
{
}

DWORD CProcessDetect::DoWork()
{
    bool bExist = false;
    while (1)
    {
        bExist = CPiProcess::IsExistModule(m_strProcess.c_str());
        if(IsNeedNotify(bExist))
        {
            m_pEvent->SetEventSign();
            break;
        }
        Sleep(m_DetectCircle);
    }
    
    return true;
}

bool CProcessDetect::WaitProcess()
{
    m_pEvent->GetLock();
    return true;
}

bool CProcessDetect::IsDetectProcessLaunch()
{
    return m_bDetectLunchy;
}

bool CProcessDetect::IsNeedNotify( bool bExist )
{
    if (bExist == (bExist & IsDetectProcessLaunch())) //存在 && 监测存在, 不存在 && 监测不存在
    {
        return true;
    }
    return false;
}

bool CProcessDetect::Init()
{
    CThreadLight::Init();

    Create_Auto_Ptr(m_pEvent, CEventLight);
    m_pEvent->Init();
    m_pEvent->SetWaitTimeout(INFINITE);
    Run();
    return true;
}

bool CProcessDetect::SetDetectProcess( tcpchar szProcess )
{
    m_strProcess = szProcess;
    return true;
}

bool CProcessDetect::SetDetectCircle( UNINT nMillSecond )
{
    m_DetectCircle = nMillSecond;
    return true;
}
