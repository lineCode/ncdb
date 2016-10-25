#pragma once
#include "pitypedef.h"
class CTypeUtil
{
public:
	CTypeUtil(void);
	~CTypeUtil(void);
public:
	operator BSTR*();
	operator tstring();
private:
	wchar_t*		m_pWChar;
};

