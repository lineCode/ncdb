#include "StdAfx.h"
#include <Windows.h>
#include <winnt.h>
#include "PiSemaphore.h"

CPiSemaphore::CPiSemaphore(void)
:m_hWnd(NULL)
,m_nMsgID(0)
{
}

CPiSemaphore::~CPiSemaphore(void)
{
}

bool CPiSemaphore::Init()
{
	return m_event.Init();
}

void CPiSemaphore::SetWaitCount( UINT nCount )
{
	m_lock.Lock();
	//m_event.Init();
	m_nCount = nCount;
	m_lock.UnLock();

}

void CPiSemaphore::SetCB( TypeFuncWait pFun, void* pAdd )
{
	m_pFun = pFun;
	m_pAdd = pAdd;
}

void CPiSemaphore::Wait()
{
	//m_event.GetLock();
	/*if (m_pFun)
	{
		//TODO:线程池中执行
		(m_pFun)(m_pAdd);
	}*/
}

void CPiSemaphore::AddWaitDone( UNINT nCount /*= 1*/ )
{
	m_lock.Lock();
	m_nCount -= nCount;
	long n = m_nCount;
	m_lock.UnLock();

	if(n == 0)
	{
		//TODO:线程池中执行

		ExecuteCB();

	}
}

bool CPiSemaphore::CallCB()
{
	(m_pFun)(m_pAdd);
	return true;
}

void CPiSemaphore::SetMsgNotify( HWND hW, int nMsg )
{
	m_hWnd = hW;
	m_nMsgID = nMsg;
}

bool CPiSemaphore::ExecuteCB()
{
	if (m_hWnd)
	{
		::PostMessage(m_hWnd, m_nMsgID, NULL, NULL);
	}
	else
	{
		CallCB();
	}
	return true;
}
