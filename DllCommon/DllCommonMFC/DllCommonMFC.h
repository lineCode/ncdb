// DllCommonMFC.h : DllCommonMFC DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <windows.h>
#include <string>
#include "UI\PiUITool.h"
using namespace std;

#if defined(__cplusplus)
#define NCCOMMON_EXTERN_C extern "C"
#else
#define NCCOMMON_EXTERN_C
#endif

#ifdef DLLCOMMON_EXPORTS
#define NCCOMMON_SYMBOL __declspec(dllexport)
#else
#define NCCOMMON_SYMBOL __declspec(dllimport)
#endif // DLLCOMMON_EXPORTS

#define NCCOMMON_API NCCOMMON_EXTERN_C NCCOMMON_SYMBOL
#define NCCOMMON_CALL _cdecl



// CDllCommonMFCApp
// 有关此类实现的信息，请参阅 DllCommonMFC.cpp
//

class CDllCommonMFCApp : public CWinApp
{
public:
	CDllCommonMFCApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};





typedef CPIUITool::tagSELECT_FILE_DIR	tagSELECT_FILE_DIR;

NCCOMMON_API int NCCOMMON_CALL SelectFileOrDir(tagSELECT_FILE_DIR* pTag);
NCCOMMON_API const wchar_t* NCCOMMON_CALL QuerySelectFile(int nIndex);
/************************************************************************
fun:	弹窗保存对话框
param:	szPath: 用户选择后的路径， 缓存区需要在外部分配好， 大小为MAX_PATH  260;
memo:
************************************************************************/
NCCOMMON_API bool NCCOMMON_CALL PopSaveDialog(tagSELECT_FILE_DIR* pTag, wchar_t* szPath);