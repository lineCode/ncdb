#pragma once

class QQInstallUtil
{
public:
	typedef vector<HWND> ARR_HWND;
	QQInstallUtil(void);
	~QQInstallUtil(void);
public:
	static HWND GetQQSendDialogHWND();
	static HWND GetQQLoginHWND();
	static ARR_HWND GetQQMainPaneWnd();
};
