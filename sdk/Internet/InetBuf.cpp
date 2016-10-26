#include "StdAfx.h"
#include "InetBuf.h"
#include "StrCoding.h"
#include "k_STLString.h"


CInetBuf::CInetBuf( ENUM_DATA_TYPE dt )
{
	m_enDataType = dt;
}


CInetBuf::~CInetBuf(void)
{
}

bool CInetBuf::DealReadData( void* pData, UNINT nSize )
{
	switch(m_enDataType)
	{
	case dt_data:
		break;
	case dt_string:
		m_strData += (char*)pData;
		break;
	}
	return true;
}

void CInetBuf::AddFlag( DWORD dwFlag )
{
	m_dwFlag |= dwFlag;
}
