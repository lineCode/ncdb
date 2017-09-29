#pragma once
#include <Windows.h>
#include "PiTypeDef.h"
Pi_NameSpace_Begin

class CPiLock
{
public:
    CPiLock();
    ~CPiLock();

public:
    CPiLock*  Lock();
    void    UnLock();
    
private:
    CRITICAL_SECTION	m_cs;
};


/************************************************************************/
/*  fun:资源管理对象里释放资源
/************************************************************************/
void FunUnLock(CPiLock*   pLock);

Pi_NameSpace_End