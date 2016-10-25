/*************
*
*作者 Abel(李万良)
*
*************/
#pragma once
#include <windows.h>
#include <string>
#include "PiTypeDef.h"
using namespace  std;
#define VALID_STR_POS(pos) ((pos) != string::npos)
#define NextPos(nPos) ((nPos) + 1)


/************************************************************************/
/*  fun: 获取第n个字符c 和 n+1个c 中间的内容
    in: pos:指定从哪个位置开始查找
        cTimes:  指定第几个字符作为左标记
    ret: 返回符合条件的字符(左边这个)的位置
/************************************************************************/
int GetContentBetweenChar(const tstring& strSource,tstring & strDest, 
						  tcpchar c,const UINT cTimes = 1,UINT pos = 0
						  );
/************************************************************************/
/*  fun: 获取两个指定字符中的字符串
    in:
        3: 字符1
        4: 字符2
        5: 第几个字符1后的数据
        6:pos, 开始查找字符1的偏移
    ret: 返回符合条件的字符2的pos
/************************************************************************/
int GetContentBetweenTwoChar(const tstring& strSource,tstring & strDest, 
							 tcpchar c1,tcpchar c2,
							 const UINT cTimes1 = 1,UINT pos = 0
						
							 );
/************************************************************************/
/*  fun: 字符串替换,出现的所有字符替换成指定字符                                                                   
    remark: c,要查找的字符, c2 要替换的字符
/************************************************************************/
bool StrReplace(tstring& strSource,tcpchar c,tcpchar c2 = _T(""), int nCount = -1);
//bool StrReplace(wstring& strSource,PCSTR c,PCSTR c2 = "");

bool StrReplaceFirst(tstring& strSource, tcpchar cFind, tcpchar cDist = _T(""));
bool StrReplaceRange(tstring& strSource,tcpchar nFlagLeft, tcpchar nFlagRight,tcpchar cDist = _T(""), int nCount = -1);
//返回指定字符在字符串中出现第n次时的索引
int GetCharPos(const tstring strSource,tcpchar c,const UINT cTimes = 1, int nOffset = 0);

//获取指定子字符串在字符串中出现的次数
DWORD GetCharHaveCount(const tstring &strSource, tcpchar pCh);
bool EraseNoDigit(tstring& szStrR);

bool IsStrExist(const tstring str1, tstring& str2);
bool IsStrExist(const tstring str1, tcpchar str2);



string ws2s(const wstring& ws);
string ws2s(const wchar_t* ws);

wstring s2ws(const string& s);
wstring s2ws(const char* s);
