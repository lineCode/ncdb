#pragma once
#include <Windows.h>
class CLock
{
public:
    CLock(void);
    ~CLock(void);

public:
    CLock*  Lock();
    void    UnLock();
    
private:
    CRITICAL_SECTION	m_cs;
};


/************************************************************************/
/*  fun:资源管理对象里释放资源
/************************************************************************/
void FunUnLock(CLock*   pLock);