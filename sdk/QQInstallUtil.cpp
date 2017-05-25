#include "StdAfx.h"
#include "QQInstallUtil.h"
#include "MainHWnd.h"

QQInstallUtil::QQInstallUtil(void)
{
}

QQInstallUtil::~QQInstallUtil(void)
{
}

HWND QQInstallUtil::GetQQSendDialogHWND()
{
	HWND hTarget = NULL;
	CMainHWnd iterWnd;
	iterWnd.EnumWndsByTitle(_T("QQ"), _T("TXGuiFoundation"), FALSE, FALSE);

	
	CMainHWnd iterSend;
	for (int i = 0; i < iterWnd.GetWndCounts(); ++i)
	{
		DWORD processId;
		HWND hMain = iterWnd.GetHWnd(i);
		GetWindowThreadProcessId(hMain, &processId);
		iterSend.EnumWndsByPid(processId);
		for (int j = 0; j < iterSend.GetWndCounts(); ++j)
		{
			HWND hQQ = iterSend.GetHWnd(j);
			wstring strTitle;
			strTitle.resize(100);
			::GetWindowText(hQQ, &strTitle.at(0), 100);
			
			std::wstring titleFormat1 = _T("的 Android手机"),
				titleFormat2 = _T("的 手机"),
				titleFormat3 = _T("的 iPhone");

			if(strTitle.find(titleFormat1) != strTitle.npos
				|| strTitle.find(titleFormat2) != strTitle.npos
				|| strTitle.find(titleFormat3) != strTitle.npos)
			{
				hTarget = hQQ;
				break;
			}
		}
	}
	
	return hTarget;
}

HWND QQInstallUtil::GetQQLoginHWND()
{
	CMainHWnd iterWnd;
	iterWnd.EnumWndsByTitle(_T("QQ"), _T("TXGuiFoundation"), FALSE, FALSE);
	unsigned long nWndqqLogin = iterWnd.GetWndCounts();
	HWND hTarget = NULL;
	for (int i = 0; i < nWndqqLogin; ++i)
	{
		hTarget = iterWnd.GetHWnd(0);
		RECT rt = {0};
		if(!GetWindowRect(hTarget, &rt))
		{
			continue;
		}
		if(abs((rt.right - rt.left) - (rt.bottom - rt.top)) < 70)
		{
			return hTarget;
		}
	}
	return NULL;
}

QQInstallUtil::ARR_HWND QQInstallUtil::GetQQMainPaneWnd()
{
	CMainHWnd iterWnd;
	iterWnd.EnumWndsByTitle(_T("QQ"), _T("TXGuiFoundation"), FALSE, FALSE);
	unsigned long nWndqqLogin = iterWnd.GetWndCounts();
	HWND hTarget = NULL;
	SIZE szMin = {292, 528};
	ARR_HWND arrWnd; 
	for (int i = 0; i < nWndqqLogin; ++i)
	{
		hTarget = iterWnd.GetHWnd(i);
		RECT rt = {0};
		if(!GetWindowRect(hTarget, &rt))
		{
			continue;
		}
		if((rt.right - rt.left) >= szMin.cx
			&& (rt.bottom - rt.top) >= szMin.cy)
		{
			arrWnd.push_back(hTarget);
		}
	}
	return arrWnd;
}