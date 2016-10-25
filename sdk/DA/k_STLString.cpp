/***********************************************************************
 Copyright (c), 2012, Unicorn Tech. Co., Ltd.

 File name   : k_STLString.cpp
 Description : 基于STL扩展为支持ANSI和Unicode两种字符集的条件编译，
               及支持Format类型转换；
 Other       : 
 Version     : 2.0
 Author      : xuemingzhi
 Date        : 2014/06/13
 -----------------------------------------------------------------------
 History 1 
      Date   : 2014/06/13
      Author : xuemingzhi
      Modification : 支持字符串型数字转换成实数；

 History 2 
      Date   : 2014/06/16
      Author : xuemingzhi
      Modification : 添加ToAnscii及ToUnicode；
***********************************************************************/

#include "stdafx.h"
#include "k_STLString.h"
#include <stdarg.h>

// --------------------------------------------------------------------------
// Function : Format
// Thinking : 获取不定参数组合后总字符数》申请相应的memory》组合完写回自身控件》释放临时memory
// --------------------------------------------------------------------------
void STLString::Format(const TCHAR* format,...)
{
  TCHAR *buff = (TCHAR*)0;
  va_list arglist;
  va_start(arglist, format);
  int len = _vsctprintf(format,arglist) + 1;
  buff = (TCHAR*)malloc(len * sizeof(TCHAR));
  if(buff) {
    memset(buff,0,len * sizeof(TCHAR));
    _vstprintf_s(buff,len,format,arglist);
	//_vstprintf(buff,format,arglist);
    assign(buff);
    free(buff);
  } 
  va_end(arglist);
}
// --------------------------------------------------------------------------
// Function : MakeLower
// Thinking : 转换成小写
// --------------------------------------------------------------------------
void STLString::MakeLower()
{
  if( 0 == this->length() ) return;
  DWORD len = (DWORD)size() + 1;
  TCHAR* buff = new TCHAR[ len ];
  memset(buff,0,sizeof(TCHAR) * len);
  _tcscpy(buff,c_str());
  CharLowerBuff(buff,len);
  clear();
  assign(buff);
}
// --------------------------------------------------------------------------
// Function : MakeUpper
// Thinking : 转换成大写
// --------------------------------------------------------------------------
void STLString::MakeUpper()
{
  if( 0 == this->length() ) return;
  DWORD len = (DWORD)size() + 1;
  TCHAR* buff = new TCHAR[ len ];
  memset(buff,0,sizeof(TCHAR) * len);
  _tcscpy(buff,c_str());
  CharUpperBuff(buff,len);
  clear();
  assign(buff);
}
// --------------------------------------------------------------------------
// Function : ToInt
// Thinking : 利用_ttoi函数将字符串转换成实数，如果字符串为空就返回-1
// --------------------------------------------------------------------------
int STLString::ToInt()
{
  if(!empty())
    return _ttoi(c_str());
  return -1;
}
// --------------------------------------------------------------------------
// Function : ToAnscii
// Thinking : 当编译为Unicode时，转换成多字节；当编译为多字节时，返回本身；
// --------------------------------------------------------------------------
string STLString::ToAnscii(int codepage /*= CP_ACP*/)
{
#if defined(UNICODE) || defined(_UNICODE)
  return Unicode2Anscii(*this,codepage);
#else
	if(CP_ACP == codepage)
		return *this;
  wstring wcs = Anscii2Unicode(*this,CP_ACP);
  return Unicode2Anscii(wcs,codepage);
#endif
}
// --------------------------------------------------------------------------
// Function : ToUnicode
// Thinking : 当编译为多字节时，转换成Unicode；当编译为Unicode时，返回本身；
// --------------------------------------------------------------------------
wstring STLString::ToUnicode(int codepage /*= CP_ACP*/)
{
#if defined(UNICODE) || defined(_UNICODE)
	if(CP_ACP == codepage)
		return *this;
	string str = Unicode2Anscii(*this,CP_ACP);
	return Anscii2Unicode(str,codepage);
#else
	return Anscii2Unicode(*this,codepage);
#endif
}
// --------------------------------------------------------------------------
// Function : Unicode2Anscii、Anscii2Unicode
// Thinking : 
// --------------------------------------------------------------------------
string STLString::Unicode2Anscii(const wstring& wcs,int codepage)
{
	if( wcs.empty() )
		return string();

	string _oper;
	int len = WideCharToMultiByte(codepage,0,wcs.c_str(),-1,NULL,0,NULL,NULL);
	char* buff = new char[len + 1];
	memset(buff,0,(len + 1) * sizeof(char));
	WideCharToMultiByte(codepage,0,wcs.c_str(),len,buff,len+1,NULL,NULL);
	_oper = buff;
	delete[] buff;
	return _oper;
}

wstring STLString::Anscii2Unicode(const string& str,int codepage)
{
	if( str.empty() )
		return wstring();

	wstring _oper;
	int len = MultiByteToWideChar(codepage,0,str.c_str(),-1,NULL,0);
	wchar_t* buff = new wchar_t[len + 1];
	memset(buff,0,(len + 1) * sizeof(char));
	MultiByteToWideChar(codepage,0,str.c_str(),len,buff,len+1);
	_oper = buff;
	delete[] buff;
	return _oper;
}
// --------------------------------------------------------------------------
// Function : ConvertEndCharInString
// Thinking : 循环比较字符串每个字符，等于'\0'时，替换为cReplace
// --------------------------------------------------------------------------
STLString STLString::ConvertEndCharInString(LPCTSTR inSrc,int inSize,TCHAR cReplace)
{
	if(NULL == inSrc) return STLString();
	STLString result;
	bool isEnd = false;
	TCHAR* dist = new TCHAR[inSize];
	memset(dist,0,inSize * sizeof(TCHAR));
	for(int i = 0; i < inSize; ++i)
	{
		TCHAR c = inSrc[i];
		if(_T('\0') == c) {
			if(isEnd)
				break;
			c = cReplace;
			isEnd = true;
		}
		else {
			isEnd = false;
		}
		dist[i] = c;
	}
	result = dist;
	delete[] dist;
	return result;
}

// --------------------------------------------------------------------------
// Function : operator=
// Thinking : 运算符重载
// --------------------------------------------------------------------------
STLString& STLString::operator=(const STLString& str)
{
	assign(str);
	return *this;
}

/******************************* STLStringArray *******************************************/

// --------------------------------------------------------------------------
// Function : STLStringArray类构造函数
// Thinking : 将一由分隔符分隔的字符串拆成数组；
// --------------------------------------------------------------------------
STLStringArray::STLStringArray()
{

}

STLStringArray::STLStringArray(LPCTSTR szBuff,int nSize,TCHAR cSeparator)
{
	set(szBuff,nSize,cSeparator);
}
// --------------------------------------------------------------------------
// Function : STLStringArray类构造函数
// Thinking : 将一由分隔符分隔的字符串拆成数组；
// --------------------------------------------------------------------------
void STLStringArray::set(LPCTSTR szBuff,int nSize,TCHAR cSeparator)
{
	bool isEnd = false;
	STLString save;
	for(int i = 0; i < nSize; ++i) 
	{
		TCHAR c = szBuff[i];
		if(c != cSeparator) {
			save += c;
			isEnd = false;
		}
		else {
			// 出现一样的间隔符时，间隔为1，默认为结束符；
			if(isEnd) {
				break;
			}
			this->push_back(save);
			save.clear();
			isEnd = true;
		}
	}

	// 容错处理，未以分隔号结尾也属于合法范围；
	if(!save.empty()) {
		this->push_back(save);
	}
}
