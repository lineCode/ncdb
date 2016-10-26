#pragma once
#include "ListMng.h"
#include "PARestfulDef.h"

class CNewsMng :public CListMng<tagDATA_NEWS, LST_NEWS>
{
public:
    CNewsMng(void);
    ~CNewsMng(void);
    int GetNextCycle();
    bool SetCircle( int nCircle );
public:
private:
    int               m_nCircle;
};

