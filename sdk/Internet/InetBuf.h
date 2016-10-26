#pragma once
#include "internet\internethttp.h"
class CInetBuf :
	public CInternetHttp
{
public:
	enum ENUM_DATA_TYPE{dt_data = 0, dt_string};

	CInetBuf(ENUM_DATA_TYPE dt);
	~CInetBuf(void);
public:
	const string& GetDataStr()const		{ return m_strData;}
	void*	GetDataBuf()		{return m_pBuf;}
protected:
	virtual bool DealReadData(void* pData, UNINT nSize);
	void AddFlag( DWORD dwFlag );
private:
	void*				m_pBuf;
	string				m_strData;

	ENUM_DATA_TYPE		m_enDataType;
};

