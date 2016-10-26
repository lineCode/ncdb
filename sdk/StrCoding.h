#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include "piTypeDef.h"
using namespace std;

#define s2w(str) CStrCoding::AnsiToUnicode((str).c_str())
#define w2s(str) CStrCoding::UnicodeToAnsi((str).c_str())

#ifdef UNICODE
	#define s2wSmart(str) (str)
	#define w2sSmart(str) w2s(str)
	#define ps2pwSmart(str) (CStrCoding::AnsiToUnicode(str).c_str())
    #define pw2psSmart(str) (CStrCoding::UnicodeToAnsi(str).c_str())
	#define ps2t(str) s2w(string(str))
	#define s2t(str) s2w(str)
	#define pw2t(str) (wstring(str))
	#define w2t(str) (str)
	#define t2w(str) (str)
	#define tp2w(str) (wstring(str))
	#define tp2wp(str) (wstring(str).c_str())
	#define t2s(str) w2s(str)
	#define tp2s(str) CStrCoding::UnicodeToAnsi(str)
	#define tp2sp(str) CStrCoding::UnicodeToAnsi(str).c_str()
#else
	#define s2wSmart(str) s2w(str)
	#define w2sSmart(str) (str)
    #define ps2pwSmart(str) (str)
    #define pw2psSmart(str) (str)
	#define ps2t(str) (string(str))
	#define s2t(str) (str)
	#define pw2t(str) (wstring(str))
	#define w2t(str) w2s(str)
	#define t2w(str) s2w(str)
	#define tp2w(str) (CStrCoding::AnsiToUnicode(str))
	#define tp2wp(str) (CStrCoding::AnsiToUnicode(str).c_str())
	#define t2s(str) (str)
	#define tp2s(str) (string(str))

#endif
class CStrCoding
{
public:
    CStrCoding(void);
    ~CStrCoding(void);

    static void UTF_8ToGB2312(string &pOut, char *pText, int pLen);//utf_8转为gb2312
    static void GB2312ToUTF_8(string& pOut,char *pText, int pLen); //gb2312 转utf_8
    
    static wstring AnsiToUnicode(PCSTR szData);
    static string UnicodeToAnsi(const wchar_t* wszString);
	static wstring Utf8ToUnicode(LPCSTR pSour);
	static tstring Utf8ToTStr(LPCSTR pSour);
	static string UnicodeToUtf8( LPCWSTR szData );

    static string Utf8ToAnsi(LPCSTR pSour);
    static string AnsiToUtf8(LPCSTR pSour);

	static string GB2GBK(const char* data);													//简体转繁体
	static string GBK2GB(const char* data);													//繁体转简体

    //url编码
    static string UrlGB2312(char * str);                           //urlgb2312编码
    static string UrlUtf8(char * str);                             //urlutf8 编码
    static string UrlUtf8Decode(string str);                  //urlutf8解码
    static string UrlGB2312Decode(string str);                //urlgb2312解码
    static wstring UrlUnicodeDecode(wstring str);                //urlgb2312解码
    static tstring UrlDecode(tstring strUrl);                //urlgb2312解码
	static tstring HttpDecode( StringCRef strData);

	static string EncodeUrl( const string& str );

    //base64加密

    static tstring EncodeBase64(PCVOID szData, UNINT nSize);
	/************************************************************************
		pnSize:解码后的数据大小
	************************************************************************/
    static void* DecodeBase64(PCSTR szData, UNINT* pnSize = NULL);
    static void* DecodeBase64_T(tcpchar szData, UNINT* pnSize = NULL);
    //base64解密-转成unicode
    static tstring DecodeBase64Str_T( tcpchar szData );
    static wstring Utf8Base64W2Unicode(tcpchar szData);    //解密成unicode
	static string EncodeUrlToUtf8( const tstring& strParam );

private:
    static void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);
    static void UTF_8ToUnicode(WCHAR* pOut,char *pText);
    static void UnicodeToUTF_8(char* pOut,WCHAR* pText);
    static void UnicodeToGB2312(char* pOut,WCHAR uData);
    static char CharToInt(char ch);
    static char StrToBin(char *str);
};
