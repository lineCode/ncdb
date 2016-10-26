/***********************************************************************
 Copyright (c), 2012, Unicorn Tech. Co., Ltd.

 File name   : k_STLString.h
 Description : 基于STL扩展为支持ANSI和Unicode两种字符集的条件编译，
               及支持Format类型转换；
 Other       : 
 Version     : 2.0
 Author      : 
 Date        : 2014/06/13
 -----------------------------------------------------------------------
 History 1 
      Date   : 2014/06/13
      Author : 
      Modification : 支持字符串型数字转换成实数；

 History 2 
      Date   : 2014/06/16
      Author : 
      Modification : 添加所有构造函数的支持；添加ToAnscii及ToUnicode；

 History 3 
	  Date   : 2014/08/12
	  Author : 
	  Modification : 支持字符集的选择；

 History 4 
      Date   : 2014/08/27
      Author : 
      Modification : 添加大小写的转换；

***********************************************************************/

#ifndef K_STLSTRING_H
#define K_STLSTRING_H

#include <tchar.h>
#include <string>
#include <vector>
using namespace std;
#include <winnls.h>

/* 字符串类
*/
class STLString
  : public basic_string< TCHAR, char_traits<TCHAR>,allocator<TCHAR> >
{
  // [格式化]
public:
  void    Format(const TCHAR* format,...);
  STLString    MakeLower(); // 转换成大写
  STLString    MakeUpper(); // 转换成小写

  // [对象数据转换]
public:
  int     ToInt();
  string  ToAnscii(int codepage = CP_ACP);
  wstring ToUnicode(int codepage = CP_ACP);

  // [静态转换]
  static string Unicode2Anscii(const wstring& wcs,int codepage);
  static wstring Anscii2Unicode(const string& str,int codepage);

  // [运算符重载]
public:
  operator LPCTSTR() { return c_str(); }
  STLString& operator=(const STLString& str);
 
public:
	// 转换字符串里出现的结束字符‘\0’
	static STLString ConvertEndCharInString(LPCTSTR inSrc,int inSize,TCHAR cReplace);

public:
  STLString() : basic_string() { }
  STLString(const _Myt& _Right) : basic_string(_Right) { }
  explicit STLString(const _Alloc& _Al) : basic_string(_Al) { }
  STLString(const _Myt& _Right, size_type _Roff,size_type _Count = npos)
    : basic_string(_Right,_Roff,_Count) { }
  STLString(const _Myt& _Right, size_type _Roff, size_type _Count,const _Alloc& _Al)
    : basic_string(_Right,_Roff,_Count,_Al) { }
  STLString(const TCHAR *_Ptr, size_type _Count) : basic_string(_Ptr,_Count) { }
  STLString(const TCHAR *_Ptr, size_type _Count, const _Alloc& _Al) : basic_string(_Ptr,_Count,_Al) { }
  STLString(const TCHAR *_Ptr) : basic_string(_Ptr) { }
  STLString(const TCHAR *_Ptr, const _Alloc& _Al) : basic_string(_Ptr,_Al) { }
  STLString(size_type _Count, TCHAR _Ch) : basic_string(_Count,_Ch) { }
  STLString(size_type _Count, TCHAR _Ch, const _Alloc& _Al) : basic_string(_Count,_Ch,_Al) { }
  template<class _It>
  STLString(_It _First, _It _Last) : basic_string(_First,_Last) { }
  template<class _It>
  STLString(_It _First, _It _Last, const _Alloc& _Al) : basic_string(_First,_Last,_Al) { }
  
  // 构造扩展
  STLString(int n) : basic_string() {
    Format(_T("%d"),n);
  }
  STLString(LPCTSTR cmd,int n) : basic_string() {
    if(cmd) Format(cmd,n);
  }
};

/* 常量字符串的引用类型定义
*/
typedef const STLString& ConstSTLStringRef;

/* 字符串数组类
*/
class STLStringArray : public vector< STLString >
{
public:
	STLStringArray();
	STLStringArray(LPCTSTR szBuff,int nSize,TCHAR cSeparator);

public:
	void set(LPCTSTR szBuff,int nSize,TCHAR cSeparator);
};

#endif // !K_STLSTRING_H