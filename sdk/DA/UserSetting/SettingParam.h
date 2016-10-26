#pragma once

class CSettingGeneralParam
{
public:
	CSettingGeneralParam()
	{
		m_bAutoOpen = FALSE;
		m_bExit = FALSE;
		m_bManualOpen = FALSE;
	};

public:
	BOOL m_bAutoOpen;				// 开机启动
	BOOL m_bExit;					// 关闭后退出
	BOOL m_bManualOpen;				// 手动启动软件
};

class CSettingAppParam
{
public:
	CSettingAppParam()
	{
		m_bSoftPackageSave = FALSE;
		m_strSoftPackagePath = _T("C:\\Unicorn");

		m_bDriverPackageSave = FALSE;
		m_strDriverPackagePath = _T("C:\\Unicorn");

		m_strVedioPath = _T("C:\\Unicorn");
		m_cVedioStartHotkey = 'A';
		m_cVedioPauseHotkey = 'B';
		m_cVedioStopHotkey = 'C';

		m_strWorksSavePath = _T("C:\\Unicorn");
		m_strFontPath = _T("C:\\Unicorn");
	};

public:
	BOOL m_bSoftPackageSave;
	CString m_strSoftPackagePath;

	BOOL m_bDriverPackageSave;
	CString m_strDriverPackagePath;

	CString m_strVedioPath;
	char m_cVedioStartHotkey;
	char m_cVedioPauseHotkey;
	char m_cVedioStopHotkey;

	CString m_strWorksSavePath;

	CString m_strFontPath;
};

class CSettingUpdateParam
{
public:
	CSettingUpdateParam()
	{
		m_bUpdateType = 1;
	};

public:
	int m_bUpdateType;
};

class CSettingMonitorParam
{
public:
	CSettingMonitorParam()
	{
		m_bAutoOpen = TRUE;
		m_bTopMost = TRUE;
	}

public:
	BOOL m_bAutoOpen;
	BOOL m_bTopMost;
};

class CSettingTotalParam
{
public:
	CSettingTotalParam() {};

public:
	CSettingGeneralParam m_paramGeneral;
	CSettingAppParam m_paramApp;
	CSettingUpdateParam m_paramUpdate;
	CSettingMonitorParam m_paramMonitor;
};