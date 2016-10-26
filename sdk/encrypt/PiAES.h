#pragma once
#include "PiTypeDef.h"

class CPiAES
{
public:
	CPiAES(void);
	~CPiAES(void);
public:
	static string AESEncryptString(const string& str_in, const string& key, const string& iv);
	static wstring AESEncryptString(const wstring& str_in, const wstring& key, const wstring& iv);

};
