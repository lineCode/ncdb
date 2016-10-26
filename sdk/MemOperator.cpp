#include "StdAfx.h"
#include "MemOperator.h"

Pi_NameSpace_Begin

CMemOperator::CMemOperator(void* pData)
{
	m_pData = pData;
	m_pCurrentPos = (char*)m_pData;
}

CMemOperator::CMemOperator( const char* pData )
{
	m_pData = pData;
	m_pCurrentPos = (char*)m_pData;
}

CMemOperator::~CMemOperator(void)
{
}

int CMemOperator::GetInt()
{
	if (!ValidPos(sizeof(int)))
	{
		return 0;
	}
	int nValue = *(int*)m_pCurrentPos;
	m_pCurrentPos += sizeof(int);
	return nValue;
}

void* CMemOperator::GetData( int nSize )
{
	if (!ValidPos(nSize))
	{
		return 0;
	}

	void* nValue = m_pCurrentPos;
	m_pCurrentPos += nSize;
	return nValue;
}

bool CMemOperator::ValidPos( int nSize )
{
	return m_pCurrentPos 
		&& m_pCurrentPos + nSize <= (char*)m_pData + m_nDataLength - 1;
}

Pi_NameSpace_End
