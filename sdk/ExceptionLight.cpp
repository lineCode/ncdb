#include "StdAfx.h"
#include "ExceptionLight.h"

CExceptionLight::CExceptionLight(const char* szDesc)
{
    m_strExceptionDesc  = szDesc;
	m_errCode = 0;
}

CExceptionLight::CExceptionLight( ErrCodeType err )
{
	m_errCode = err;
}

CExceptionLight::~CExceptionLight(void)
{
}

const string CExceptionLight::What()
{
    return m_strExceptionDesc;
}

CExceptionLight::operator ErrCodeType()
{
	return m_errCode;
}
