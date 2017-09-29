#include "stdafx.h"
#include "RandNum.h"


CRandNum::CRandNum( int nBegin, int nEnd )
	:m_nBegin(nBegin)
	,m_nEnd(nEnd)
{

}

CRandNum::CRandNum( int nEnd /*= 1*/)
{
	m_nBegin = 1;
	m_nEnd = nEnd;
}

CRandNum::CRandNum( int nBegin, int nEnd, int nPercent )
	:m_nBegin(nBegin)
	,m_nEnd(nEnd)
	,m_nPercent(nPercent)
{

}


CRandNum::~CRandNum(void)
{
}

int CRandNum::GetNum()
{
	return GetNumRange(m_nBegin, m_nEnd);
}

UINT CRandNum::GetNumRange( int nBegin, int nEnd )
{
	UINT nPos = 0;
	if (nBegin == nEnd)
	{
		nPos = nBegin;
	}
	else
	{
		nPos = rand()%( nEnd + 1 - nBegin) + nBegin;
	}

	return nPos;
}

bool CRandNum::IsRandHit()
{
	return GetNumRange(m_nBegin, m_nEnd) <= m_nPercent;

}
