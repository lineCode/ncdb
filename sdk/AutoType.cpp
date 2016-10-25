#include "StdAfx.h"
#include "AutoType.h"
#include "Math/mathUnit.h"
#include <tchar.h>
#pragma comment(lib, "comsuppw.lib")
Pi_NameSpace_Using

CAutoType::CAutoType(tcpchar p)
{
    //tstring str(p);
    m_str = p;
    m_bstrt = p;
}

CAutoType::CAutoType(__int64 intTemp/*  = 0*/)
{
	m_bstrt = intTemp;
}

CAutoType::CAutoType( const tstring& strL )
{
    m_bstrt = strL.c_str();
}

CAutoType::~CAutoType(void)
{
}


CAutoType::operator int()
{
    return (int)_ttoi64((tcpchar)m_bstrt);
}

CAutoType::operator UNINT()
{
	return (UNINT)_ttoi64((tcpchar)m_bstrt);

}

CAutoType::operator UNLONG()
{
	return (UNLONG)_ttoi64((tcpchar)m_bstrt);

}


CAutoType::operator float()
{
	_CRT_FLOAT f;
	//float nValue = 0;
	_atoflt(&f, CAutoType(m_bstrt));
	return f.f;
}

CAutoType::operator __int64()
{
    return ::_ttoi64((tcpchar)m_bstrt);
}

LLONG CAutoType::operator +(LLONG lrh)
{
    return LLONG(*this) + lrh;
}

int CAutoType::operator+( int lrh )
{
    return int(*this) + lrh;
}

CAutoType& CAutoType::operator ++()
{
    this->operator +(1);
    return *this;
}

CAutoType CAutoType::operator ++(int iUseless)
{
    CAutoType atTemp = *this;
    this->operator +(1);
    return atTemp;
}

CAutoType::operator const char*()
{
    return (const char*)m_bstrt;
}

CAutoType::operator tstring()
{
    return (tstring)m_bstrt;
}

const tcpchar CAutoType::ToHex()
{
    const int nSize = 20;
    m_str.resize(nSize);    //充当临时变量
    ::_stprintf_s(&m_str.at(0), nSize, _T("0x%08X"), (int)*this);
    return m_str.c_str();
}

tstring CAutoType::ToHex(void* pAddress)
{
	return ToHex((DWORD)pAddress);
    /*const int nSize = 20;
    m_str.resize(nSize);    //充当临时变量
    ::_stprintf_s(&m_str.at(0), nSize, _T("%08X"), (int)pAddress);
    return m_str.c_str();*/
}

tstring CAutoType::ToHex( DWORD dwValue)
{
	CAutoType at(dwValue);
	return at.ToHex();
}

CAutoType::operator char*()
{
    return (char*)m_bstrt;
}

CAutoType::operator const wchar_t*()
{
    return (wchar_t*)m_bstrt;
}

CAutoType& CAutoType::SetData(byte iTemp)
{
    m_bstrt = iTemp;
    return *this;
}

CAutoType& CAutoType::SetData(long iTemp)
{
    m_bstrt = iTemp;
    return *this;
}

CAutoType& CAutoType::SetData(UNLONG iTemp)
{
    m_bstrt = iTemp;
    return *this;
}

CAutoType& CAutoType::SetData(__int64 iTemp)
{
    m_bstrt = iTemp;
    return *this;
}

CAutoType& CAutoType::SetData(UNINT64 iTemp)
{
    m_bstrt = iTemp;
    return *this;
}

CAutoType& CAutoType::SetData(tcpchar pChar)
{
    m_bstrt = pChar;
    return *this;
}

CAutoType& CAutoType::SetData( const tstring& pChar )
{
	m_bstrt = pChar.c_str();
	return *this;
}


CAutoType& CAutoType::SetData( float fValue )
{
    m_bstrt = fValue;
    return *this;
}

CAutoType& CAutoType::SetData( int nValue )
{
    m_bstrt = nValue;
    return *this;
}

CAutoType& CAutoType::SetData( UNINT nValue )
{
    m_bstrt = nValue;
    return *this;
}
