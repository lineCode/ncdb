#pragma once
#include <string>
using std::string;
using std::wstring;
class CStrTrans
{
public:
	//UNICODE×ª»»³ÉANSI
    static string UnicodeToAnsi(const wchar_t* wszString);
    static wstring AnsiToUnicode( const char* szStr );

	static CString AnsiToUnicode_Str( const char* szStr );

    static CString UTF8ToUnicode(const char* UTF8);
	static char* UnicodeToUTF8(CString strUnicode);

	static WCHAR* FontFile_AnsiToUnicode(char* pUnicoDestr, int nLen);
};