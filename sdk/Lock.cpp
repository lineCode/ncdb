#include "StdAfx.h"
#include "Lock.h"

CLock::CLock(void)
{
    InitializeCriticalSection(&m_cs);
}

CLock::~CLock(void)
{
    DeleteCriticalSection(&m_cs);
}


CLock* CLock::Lock()
{
    EnterCriticalSection(&m_cs);
    return this;
}

void CLock::UnLock()
{
    LeaveCriticalSection(&m_cs);
}


void FunUnLock( CLock* pLock )
{
    pLock->UnLock();
}
