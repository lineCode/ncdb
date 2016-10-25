#pragma once
#include <comutil.h>
#include <string>
#include "PiTypeDef.h"
using namespace std;
class CAutoType
{
public:
    CAutoType(tcpchar);
    CAutoType(const tstring& strL);
    CAutoType(__int64 = 0);
    ~CAutoType(void);
    
public:
    /************************************************************************/
    /*  针对
    /************************************************************************/
    CAutoType& SetData(byte iTemp);
    CAutoType& SetData(long iTemp);
    CAutoType& SetData(UNLONG iTemp);
    CAutoType& SetData(__int64 iTemp);
    CAutoType& SetData(UNINT64 iTemp);
    CAutoType& SetData(float fValue);
    CAutoType& SetData(int nValue);
    CAutoType& SetData(UNINT nValue);

    //CAutoType& SetData(const char*);
	CAutoType& SetData(tcpchar pChar);
    CAutoType& SetData(const tstring& pChar);


    //前递增运算符
    CAutoType& operator ++();
    //后递增运算符
    CAutoType operator ++(int iUseless);
    //CAutoType& operator +(long lrhj);
    int operator +(int lrh);
    LLONG operator +(LLONG lrh);

	operator int();
	operator UNINT();
	operator UNLONG();
    operator float();
    operator __int64();
    operator const char*();
    operator const wchar_t*();
    operator char*();
    operator tstring();

    //operator const wchar_t*();

    const tcpchar  ToHex();
	static tstring  ToHex(void* );
    static tstring  ToHex(DWORD dwValue);

    
    
private:
    tstring      m_str;
    //__int64     m_Int;
    //double      m_dbValue;
    _bstr_t     m_bstrt;
};
