#pragma once
#include "ObjIdl.h"
class IPiDataObjectHelper
{
public:
	IPiDataObjectHelper(IDataObject* pDO);
	~IPiDataObjectHelper();
public:
	bool IsHasData(UINT cfFormat);
	HGLOBAL GetData(UINT cfFormat);
private:
	IDataObject*		m_pDO;
};

