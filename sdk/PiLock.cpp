#include "StdAfx.h"
#include "PiLock.h"
Pi_NameSpace_Begin
CPiLock::CPiLock()
{
    InitializeCriticalSection(&m_cs);
}

CPiLock::~CPiLock()
{
    DeleteCriticalSection(&m_cs);
}


CPiLock* CPiLock::Lock()
{
    EnterCriticalSection(&m_cs);
    return this;
}

void CPiLock::UnLock()
{
    LeaveCriticalSection(&m_cs);
}


void FunUnLock( CPiLock* pLock )
{
    pLock->UnLock();
}

Pi_NameSpace_End