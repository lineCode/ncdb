#include "StdAfx.h"
#include "MonitorProcess.h"
#include "MonitorDataHandleBase.h"
#include "define_gnl.h"
#include "PiRandom.h"
#include "AutoType.h"
#include "functional.h"
#include "ResCommon.h"
#include "PADebug.h"
CMonitorProcess::CMonitorProcess(void)
{
    m_pCurrentDetectHandle          = NULL;
}


CMonitorProcess::~CMonitorProcess(void)
{
}

bool CMonitorProcess::Init()
{
    InitTask();

    return true;
    
}


bool CMonitorProcess::AddMonitor( CMonitorDataHandleBase* pHandle )
{
    bool bRet = pHandle->Init();    //用逻辑操作符
    m_mdHandle.push_back(pHandle);
    return bRet;
}

bool CMonitorProcess::Notify( time_t tDeleteTime, bool bExist )
{
    //CMonitorDataHandleBase* pMDHandle = NULL/*TODO: 根据任务获取到对应的处理对象*/;

    m_pCurrentDetectHandle->HandleDetectResult(tDeleteTime, bExist);
    return true;
}

bool CMonitorProcess::DetectAllProcess()
{
    bool bExist = false;
    time_t tDeleteTime = ::time(NULL);  //TODO: 获取到检测时的时间
    FOR_EACH_ITER(LST_MD_HANDLE, m_mdHandle, it)
    {
        m_pCurrentDetectHandle = (*it);
        if(!DetectProcess(m_pCurrentDetectHandle->GetMonitorTask().c_str(), bExist))
        {
            //检测进程失败
            return false;
        }
        Notify(tDeleteTime, bExist);
    }
    
    return true;
}

bool CMonitorProcess::DetectProcess( tcpchar szProcessName, bool& bExist )
{
    bExist = false;
    
    //判断指定的进程名是否存在

    bExist = IsProcessExist(szProcessName);

    
//     CPiRandom piRand;
//     piRand.Init();
//     bExist = piRand.GetRandomNum(1) > 0;
    tstring strMsg;
    strMsg.resize(50);
    _stprintf_s(&strMsg.at(0), 50, _T("%s --detect result: %d"), szProcessName, bExist);
    PAOut10(strMsg.c_str());
    //TRACE(strMsg.c_str());
    return true;

}

bool CMonitorProcess::UnInit()
{
    OutputDebugString(_T("CMonitorProcess UnInit\n"));
    FOR_EACH_ITER(LST_MD_HANDLE, m_mdHandle, it)
    {
        ClearMem(*it);
    }
    return true;
}
