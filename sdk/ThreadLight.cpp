#include "StdAfx.h"
#include "ThreadLight.h"
#include "define_gnl.h"
#include "ResCommon.h"
#include "EventLight.h"
//#include "RAIILock.h"
#include "Lock.h"
#include "functional.h"
#include "AutoType.h"
#include "System/PiExceptionHandle.h"
#include <process.h>
#include "PiString.h"
//#include "PiRandom.h"

FunLog			CThreadLight::m_funLog = NULL;
Pi_NameSpace_Using

/*
CThreadLight::CThreadLight( CallBackFun pCB / *= NULL* /, void* pObject / *= NULL* /)
	:m_CallBackFun(pCB)
	,m_pObject(pObject)
{
    InitVariable();

}*/

CThreadLight::CThreadLight( ProcThreadFun pFun /*= NULL*/, void* pAdd /*= NULL*/ )
:m_procThreadFun(pFun)
,m_paramR(pAdd)
{
	InitVariable();

}


CThreadLight::~CThreadLight(void)
{
	DestroyThread();
}

bool CThreadLight::Init()
{
	Create_Auto_Ptr(m_pLockDetect, CEventLight);
    Create_Auto_Ptr(m_pLockThreadInitDone, CEventLight);
    Create_Auto_Ptr(m_pLockThread, CLock);
    if(!m_pLockDetect->Init()
		|| ! m_pLockThreadInitDone->Init())
    {
        return false;
    }
    m_bInit = true;
    return true;
}

bool CThreadLight::Run()
{
    if(!DoRunBegin())
    {
        return false;
    }
    //CRAIILock raii(m_pLockThread->Lock());
	m_pLockThread->Lock();
    if (m_hThread)
    {//一次只有一个下载线程
		m_pLockThread->UnLock();
        return true;    
    }
    
	ThreadParam* pThreadParam = new ThreadParam;
	pThreadParam->pThread = this;
	pThreadParam->pFun = m_procThreadFun;
	pThreadParam->paramR = m_paramR;
	pThreadParam->strThreadName = m_strThreadName;
	m_hThread = (HANDLE)_beginthreadex(NULL, NULL, SProc, pThreadParam, NULL, NULL);
	m_pLockThread->UnLock();
	return true;
}

unsigned __stdcall CThreadLight::SProc( void* pAdd )
{
	//OutLog(_T("sproc begin"));
	OutInfo(_T("lightthread begin"));
	ThreadParam* pThreadParam = reinterpret_cast<ThreadParam*>(pAdd);

	DWORD dwRet = pThreadParam->pThread->DoThread(pThreadParam);


	delete pThreadParam;
	OutInfo(_T("lightthread end"));
	return dwRet;
}

bool CThreadLight::Notify(NotifyObj* pNt)
{
	if (m_CallBackFun)
	{
		m_CallBackFun(m_pObject, pNt);
	}

	return true;
}

void CThreadLight::SetWaitThreadTimeout( UNLONG nMilliSec )
{
    m_dwWaitThread  = nMilliSec;    
}

void* CThreadLight::GetObject()
{
    return m_pObject;
}

bool CThreadLight::IsWillStopThread()
{
    return m_bStopThread;
}

bool CThreadLight::StopThread()
{
    m_bStopThread = true;
    //TRACE(_T("CThreadLight set event sign!\n"));
    m_pLockDetect->SetEventSign();
    return true;

}

bool CThreadLight::ExitThread()
{
    if (! m_bInit)
    {
        return true;
    }

    HANDLE hTemp = GetThreadHandle();
    if (! hTemp)
    {
        return true;
    }

    CAutoType at;
    StopThread();
    tstring strMsg(m_strThreadName);
    if(hTemp)
    {
        at.SetData(GetCurrentThreadId());
		OutInfo(_T("cur thread id"), at.ToHex());
        //LOG(INFO) << "CThreadLight::ExitThread call ThreadID:"), at.ToHex(;
        DWORD dwWait = ::WaitForSingleObject(hTemp, m_dwWaitThread);
        //CRAIILock raii(m_pLockThread->Lock());
		m_pLockThread->Lock();
        if(WAIT_TIMEOUT == dwWait)
        {
            strMsg += _T(" CThreadLight force exit");
			int n = 0;
			at.SetData(m_dwThreadID);
			OutInfo(_T("tmn thread id"), at.ToHex());

            if(!TerminateThread(hTemp, -1))
				n = GetLastError();
            //ClearHandle(hTemp);
			OutInfo(_T("ter thread"));
			ClearThread();
			OutInfo(_T("clear thread"));

			OutLog(strMsg.c_str());
			m_pLockThread->UnLock();

            return true;
        }
		ClearThread();
        //ClearHandle(hTemp);
        strMsg += _T(" CThreadLight exit");
        OutInfo(strMsg.c_str());
		m_pLockThread->UnLock();
        return true;
    }
    strMsg += _T(" CThreadLight exit without thread");
    OutInfo(strMsg.c_str());
    return true;
}

bool CThreadLight::WaitTime()
{
    return m_pLockDetect->GetLock();
}

void CThreadLight::SetDelayMax( int nPeriodMS )
{
    m_pLockDetect->SetWaitTimeout(nPeriodMS);    
}

bool CThreadLight::ClearThread()
{
    //LOG(INFO) << "m_pLockThread begin ";
    //CRAIILock raii(m_pLockThread->Lock());
	m_pLockThread->Lock();
    //LOG(INFO) << "m_pLockThread end ";
    ClearHandle(m_hThread);
	m_pLockThread->UnLock();

    return true;
}

bool CThreadLight::IsThreadRunning()
{
    //CRAIILock raii(m_pLockThread->Lock());
	m_pLockThread->Lock();
	bool bRet = m_hThread > 0;
	m_pLockThread->UnLock();

    return bRet;
}

bool CThreadLight::InitVariable()
{
    m_dwWaitThread = 200;
    m_hThread       = NULL;
    m_bStopThread   = false;
    m_bAutoDestroy  = false;
    m_strThreadName = _T("thread_unName");
    m_dwThreadID    = 0;
    m_bInit         = false;
	m_bDieKillThread	= true;
    return true;
}

bool CThreadLight::Reset()
{
    m_bStopThread = false;
    //TRACE(_T("CThreadLight set event sign!\n"));
    //m_pLockDetect->();
    return true;
}

bool CThreadLight::UnInit()
{
	if (m_bDieKillThread)
	{
		return ExitThread();
	}
	return true;
}

bool CThreadLight::SetAutoDestroy()
{
    m_bAutoDestroy = true;
    return true;
}

bool CThreadLight::Clear()
{
    CThreadLight::ClearThread();
    this->ClearRes();
    return true;
}

bool CThreadLight::ClearRes()
{
    if (m_bAutoDestroy)
    {
        delete this;
    }
    return true;
}

void CThreadLight::SetThreadName( tcpchar szName )
{
    m_strThreadName = szName;
}

bool CThreadLight::InitThread()
{
    m_dwThreadID = GetCurrentThreadId();
	m_pLockThreadInitDone->SetEventSign();
    return true;
}

bool CThreadLight::DoRunBegin()
{
    return true;
}

HANDLE CThreadLight::GetThreadHandle()
{
    m_pLockThread->Lock();
    HANDLE hTemp = m_hThread;
    m_pLockThread->UnLock();
    return hTemp;
}

unsigned CThreadLight::ThreadRun()
{
	//InitThread();
	DWORD dwRet = 0;
	//判断是哪种线程模式
	if (m_procThreadFun)
	{
		dwRet = m_procThreadFun(m_paramR);
	}
	else
	{
		dwRet = DoWork();
	}

	CPiString strMsg;
	strMsg.Format(_T("%s thread (ID:%d) will exit safely"), m_strThreadName.c_str(), m_dwThreadID);
	OutInfo(strMsg.c_str());
	//Clear();
	return dwRet;
}

bool CThreadLight::OutLog( tcpchar strMsg )
{
	if (m_funLog)
	{
		OutInfo(strMsg);
		m_funLog(strMsg);
	}
	else
	{
		OutInfo(strMsg);
	}
	return true;
}

DWORD CThreadLight::DoWork()
{
	/*CPiRandom rd;
	rd.Init();
	int nSleep = rd.GetRandomNum(3000) + 100;
	OutInfo(_T("thread work start"));
	Sleep(nSleep);*/
	return 1;
}

bool CThreadLight::WaitThreadExit()
{
	HANDLE hTemp = GetThreadHandle();
	
	::WaitForSingleObject(hTemp, INFINITE);
	return true;
}

bool CThreadLight::DestroyThread()
{

	return UnInit();
}

bool CThreadLight::WaitInitDone()
{
	return m_pLockThreadInitDone->GetLock(INFINITE);
}

bool CThreadLight::DoRunEnd( DWORD dwRet )
{
	return true;
}

DWORD CThreadLight::DoThread( ThreadParam* pThreadParam )
{
	CThreadLight* pThread = pThreadParam->pThread;
	pThread->InitThread();


	DWORD dwRet = 0;
#if !defined _DEBUG
	SEH_BEGIN
#endif
		//判断是哪种线程模式
		/*if (pThreadParam->pFun)
		{
		dwRet = pThreadParam->pFun(pThreadParam->paramR);
		}
		else
		{*/
		dwRet = pThreadParam->pThread->ThreadRun();
	//}

#if !defined _DEBUG
	SEH_EXCEPT_BEGIN(EXCEPTION_EXECUTE_HANDLER)
		DWORD dwEc = GetExceptionCode();
	TCHAR szBuf[200] = {0};
	::_stprintf_s(szBuf, _countof(szBuf), _T("error, %s catch except:0x%08X")
		, pThread->m_strThreadName.c_str(), dwEc);
	CThreadLight::OutLog(szBuf);
	SEH_EXCEPT_END
#endif
	pThread->DoRunEnd(dwRet);
	pThread->Clear();
	return dwRet;
}

