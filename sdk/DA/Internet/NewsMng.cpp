#include "StdAfx.h"
#include "NewsMng.h"

CNewsMng::CNewsMng(void)
{
    m_nCircle       = 0;
}

CNewsMng::~CNewsMng(void)
{
}

int CNewsMng::GetNextCycle()
{
    return m_nCircle;
}

bool CNewsMng::SetCircle( int nCircle )
{
    m_nCircle = nCircle;
    return true;
}
