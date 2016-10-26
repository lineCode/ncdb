#include "StdAfx.h"
#include "System\PiSignal.h"
#include "ResCommon.h"


CPiSignal::CPiSignal(void)
{
	m_bWaitRet = true;
}


CPiSignal::~CPiSignal(void)
{
	UnInit();
}

bool CPiSignal::Init( UNINT nInitial /*= 0*/, UNINT nMax /*= 0*/)
{
	m_hSemaphore  = CreateSemaphore(NULL, nInitial, nMax, NULL);
	if ( !m_hSemaphore)
	{
		return false;
	}
	return true;
}

bool CPiSignal::Add( UNINT nCount /*= 1*/ )
{
	return ::ReleaseSemaphore(m_hSemaphore, nCount, NULL) == TRUE;
}

bool CPiSignal::WaitSign()
{
	DWORD dwRet = ::WaitForSingleObject(m_hSemaphore, INFINITE);
	if (WAIT_FAILED == dwRet)
	{
		return false;
	}
	return m_bWaitRet;

}

bool CPiSignal::UnInit()
{
	ClearHandle(m_hSemaphore);
	return true;
}

bool CPiSignal::SetSign( bool bRet /*= true*/ )
{
	m_bWaitRet = bRet;
	Add();
	return bRet;
}
