#pragma once
#include <string>
using std::string;
class CExceptionLight
{
public:
	typedef int ErrCodeType ;

	CExceptionLight(const char* szDesc);
	CExceptionLight(ErrCodeType err);
    ~CExceptionLight(void);

public:
    const string What();
	operator ErrCodeType();
private:
    string          m_strExceptionDesc;
	ErrCodeType				m_errCode;
};
