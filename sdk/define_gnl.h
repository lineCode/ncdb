#pragma once
#include <Windows.h>
#include <string>
#include <tchar.h>
using std::string;
using std::wstring;

//日志信息
#define INFO_NORMAL     _T("[INFO]     || ")
#define INFO_WARNING    _T("[WARNING]  || ")
#define INFO_ERROR      _T("[ERROR]    || ")

#define DEBUG_ERROR(a)	OutputDebugStringA(string(string(INFO_ERROR)    + a + "\n").c_str())
#define DEBUG_WARN(a)	OutputDebugStringA(string(string(INFO_WARNING)  + a + "\n").c_str())
#define DEBUG_INFO(a)   OutputDebugStringA(string(string(INFO_NORMAL)   + a + "\n").c_str())

//stl


#ifdef M_PI_CPP11
#define FOR_EACH_ITER_C11(CtnObj, IterPtr)\
    for (auto IterPtr = (CtnObj).begin(); IterPtr != (CtnObj).end(); ++IterPtr)
#endif
#define FOR_EACH_ITER(CtnType, CtnObj, IterPtr)\
    for (CtnType::iterator IterPtr = (CtnObj).begin(); IterPtr != (CtnObj).end(); ++IterPtr)

#ifdef M_PI_CPP11
#define FOR_EACH_ITER_CONST_C11(CtnObj, IterPtr)\
    for (auto IterPtr = (CtnObj).begin(); IterPtr != (CtnObj).end(); ++IterPtr)
#endif

#define FOR_EACH_ITER_CONST(CtnType, CtnObj, IterPtr)\
    for (CtnType::const_iterator IterPtr = (CtnObj).begin(); IterPtr != (CtnObj).end(); ++IterPtr)

#ifdef M_PI_CPP11
#define FOR_EACH_ITER_NO_ADD_C11(CtnObj, IterPtr)\
    for (auto IterPtr = (CtnObj).begin(); IterPtr != (CtnObj).end();)
#endif
#define FOR_EACH_ITER_NO_ADD(CtnType, CtnObj, IterPtr)\
    for (CtnType::iterator IterPtr = (CtnObj).begin(); IterPtr != (CtnObj).end();)


#define FOR_EACH_ITER_NO_DESC(CtnObj, IterPtr)\
	for (IterPtr = (CtnObj).begin(); IterPtr != (CtnObj).end(); ++IterPtr)

#ifdef M_PI_CPP11
#define FOR_EACH_STRING_C11(CtnObj, IterPtr)\
	for (auto IterPtr = (CtnObj).begin(); IterPtr != (CtnObj).end(); ++IterPtr)
#endif
#define FOR_EACH_STRING(CtnObj, IterPtr)\
	for (LST_STRING::iterator IterPtr = (CtnObj).begin(); IterPtr != (CtnObj).end(); ++IterPtr)

#define IsValidIter(It, Cn) \
    ((It) != (Cn).end())
#define IsValidStrPos(nPos) ( (nPos) != tstring::npos )
#define NextPos(nPos) ((nPos) + 1)

//值为false 则返回
#define DealBoolRet(bRet) if(!(bRet)) \
{\
    return false;\
}
#define DealBoolBreak(bRet) if(!(bRet)) \
{\
	break;\
}

#define FOR_COUNT(n, i) \
    for (UNINT i = 0; i < (n); ++i)
#define FOR_COUNT_DESC(n, i) \
	for (INT i = n - 1; i >= 0; --i)

#define TO_CSTRING(str) (CString(str.c_str()))



#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
