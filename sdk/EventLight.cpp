#include "StdAfx.h"
#include "EventLight.h"
#include <windows.h>
#include <assert.h>
#include "ResCommon.h"
const int NUM_WAIT_TIMEOUT                  = 500;  //
Pi_NameSpace_Begin

CEventLight::CEventLight(void)
{
	m_hEventSync = NULL;
    m_nWaitMax  = 500;
}

CEventLight::~CEventLight(void)
{
    UnInit();
}

bool CEventLight::Init(tcpchar cName /* = NULL*/)
{
	//�߳�ͬ��
	m_hEventSync = ::CreateEvent(NULL, FALSE, FALSE, cName);//2:�Զ������ź�, 3:����ʱû���ź�
    m_dwErr = ::GetLastError();
	return m_hEventSync != NULL;
}

bool CEventLight::UnInit()
{
	if (!m_hEventSync)
	{
		return true;
	}
    ClearHandle(m_hEventSync);
    
	return true;

}

bool CEventLight::Open(const char* cName)
{
	m_hEventSync = ::OpenEventA(EVENT_ALL_ACCESS , NULL, cName);
	return true;
}

bool CEventLight::GetLock( UNINT nWaitMax /*= INFINITE*/ )
{
    bool bRet = false;
	UNINT nWait = nWaitMax;
	
	switch(::WaitForSingleObject(m_hEventSync, nWait))
	{
		case WAIT_TIMEOUT:
			//��ʱ, д��־
            //TRACE(_T("event wait timeout\n"));
            bRet     = false;
			break;
        case WAIT_OBJECT_0:
            bRet = m_bWaitRet;
            break;
		default:
            //TRACE(_T("event ret quick\n"));
            bRet    = false;
			break;
	}
    return bRet;
}

bool CEventLight::SetEventSign(bool bRet /*= true*/)
{
    m_bWaitRet = bRet;
	return TRUE == ::SetEvent(m_hEventSync);
}

bool CEventLight::SetWaitTimeout( UNINT nWaitMaxMS )
{
    m_nWaitMax = nWaitMaxMS;
    return true;

}

bool CEventLight::IsExist( tcpchar cName )
{
    CEventLight ev;
    if(!ev.Init(cName))
    {
        return false;
    }
    return ERROR_ALREADY_EXISTS == ev.m_dwErr;
}

Pi_NameSpace_End