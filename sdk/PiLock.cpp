#include "StdAfx.h"
#include "PiLock.h"

CPiLock::CPiLock(void)
{
    InitializeCriticalSection(&m_cs);
}

CPiLock::~CPiLock(void)
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
