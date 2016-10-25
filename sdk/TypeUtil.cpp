#include "StdAfx.h"
#include "TypeUtil.h"


CTypeUtil::CTypeUtil(void)
{
}


CTypeUtil::~CTypeUtil(void)
{
}

CTypeUtil::operator BSTR*()
{
	return &m_pWChar;
}

CTypeUtil::operator tstring()
{
	return (tstring)m_pWChar;
}
