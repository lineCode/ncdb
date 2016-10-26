
#pragma once

#include <tchar.h>

// XML //////////////////////////////////////////////////////////////////////////

//#include "Markup.h"

// STL //////////////////////////////////////////////////////////////////////////

//
// Include
//
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <deque>
using namespace std;

//
// String Typedef
//
#ifdef UNICODE
	typedef std::wstring StlString;
#else
	typedef std::string StlString;
#endif //!UNICODE


// STL容器类型定义 ////////////////////////////////////////////////////////////////////////

//
// 字符型列表容器
//
typedef vector< StlString > StringVect;

//
// 字符型与字符型的映射表
//
typedef map< StlString, StlString > Str_Str_Map;

//
// 字符型与字符型的映射表的链表
//
typedef map< int, Str_Str_Map > Int_StrStrMap_Map;

// 参数的作用意义标志 ////////////////////////////////////////////////////////////////////////

//
// 表示会被函数逻辑写入的形参
//
#ifndef OUT
#define OUT
#endif

//
// 表示传入函数参与逻辑运算的形参
//
#ifndef IN
#define IN
#endif

// 常量定义及提取方法 ////////////////////////////////////////////////////////////////////////

//
// 空字符串
//
#define EMPTY_STR _T("")

//
// STL 字符串常量的定义
//
#define ConstStlStringVar( key, value )		const StlString key = value
