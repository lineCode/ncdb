#pragma once

#include "StdAfx.h"
#include <afx.h>
#include <locale.h>
#include <string>
#include <afxtempl.h>

class TrayIconInfo
{
public:
	BOOL bShow;
	int idCommand;
	HWND hMainWnd;
	CString strFilePath;
	CString strTip;
	CRect rcIcon;

public:
	TrayIconInfo();
	TrayIconInfo(TrayIconInfo& obj);
	TrayIconInfo& operator=(TrayIconInfo& obj);
	BOOL operator==(TrayIconInfo& obj);
	void Dump();
};

class CTrayIconHelper
{
private:
	typedef struct _TBBUTTON32 {
		int iBitmap;
		int idCommand;
		BYTE fsState;
		BYTE fsStyle;
		BYTE bReserved[2];          // padding for alignment
		DWORD_PTR dwData;
		INT_PTR iString;
	} TBBUTTON32;

	typedef struct _TBBUTTON64 {
		int iBitmap;
		int idCommand;
		BYTE fsState;
		BYTE fsStyle;
		BYTE bReserved[6];          // padding for alignment
		DWORD_PTR dwData;
		INT_PTR iString;
	} TBBUTTON64;

	// 32位时的结构，64位通过buffer偏移解析
	typedef struct TRAYDATA32
	{
		HWND hWnd;
		UINT uID;
		UINT uCallbackMessage;
		DWORD Reserved1[2];
		HICON hIcon;
		DWORD Reserved2[3];
		TCHAR szExePath[MAX_PATH];
		TCHAR szTip[128]; 
	} TRAYDATA32;

private:
	HWND GetTrayWnd();
	HWND GetNotifyIconOverflowWindow();
	BOOL IsWow64();
	CArray<TrayIconInfo, TrayIconInfo> *GetTrayIconInfoArrayForWnd(HWND hWnd, BOOL bHiddenBar);

public:
	CTrayIconHelper(void);
	~CTrayIconHelper(void);

	CArray<TrayIconInfo, TrayIconInfo> *GetTrayIconInfoArray();
    BOOL GetTrayIconRect(CString strTip, CRect &rcIcon, BOOL &bShow);
	BOOL GetTrayIconRectWithPath(CString strPath, CRect &rcIcon, BOOL &bShow);
};

