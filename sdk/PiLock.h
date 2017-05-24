#pragma once
#include <Windows.h>
class CPiLock
{
public:
    CPiLock(void);
    ~CPiLock(void);

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