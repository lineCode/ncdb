#pragma once
#include <memory>
#include "PiLock.h"
//#include "RAIILock.h"
using namespace std;

#define NewMemAndInit(pBuf, nFileSize)  void* pBuf = NULL;pBuf = new char[nFileSize]; memset(pBuf, 0, nFileSize);
#define NewMemStrAndInit(pBuf, nFileSize)  char* pBuf = NULL;pBuf = new char[nFileSize]; memset(pBuf, 0, nFileSize);
#define Create_Auto_Ptr(pAuto, Type)  (pAuto) = auto_ptr<Type>(new Type)
#define Create_Auto_Ptr_Param(pAuto, Type, paramR)  (pAuto) = auto_ptr<Type>(new Type(paramR))

#define ClearObject(a)  { ::DeleteObject(a);  (a) = NULL; }
#define ClearDC(a)      { ::DeleteDC(a);      (a) = NULL; }
#define ClearHandle(a)  { ::CloseHandle(a);   (a) = NULL; }
#define ClearMem(a)     { ::delete (a);       (a) = NULL; }
#define ClearMemArr(a)  { ::delete[] (a);     (a) = NULL; }
#define ClearOldObject(dc, obj)     { HGDIOBJ hOld = ::SelectObject(dc, obj); ClearObject(hOld); }
#define DisableCopyAndAssignmentCons(Class)\
	private: \
		Class(const Class);\
		operator =(const Class&));

#define ClearContainerMem(Cn, obj) \
    FOR_EACH_ITER(Cn, obj, it)\
    {                   \
        ClearMem(*it);  \
    }                   