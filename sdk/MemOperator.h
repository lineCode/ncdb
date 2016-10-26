#pragma once
#include "PiTypeDef.h"
#include "define_gnl.h"
Pi_NameSpace_Begin

class CMemOperator
{
public:
	CMemOperator(void* pData);
	CMemOperator(const char* pData);
	~CMemOperator(void);

public:
	int GetInt();
	void* GetData( int nSize );
	bool ValidPos( int nSize );
private:	
	const void*			m_pData;
	char*				m_pCurrentPos;
	LLONG				m_nDataLength;
};
Pi_NameSpace_End
