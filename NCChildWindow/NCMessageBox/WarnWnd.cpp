#include "stdafx.h"
#include "WarnWnd.h"
#include "NCMessageBox.h"

vector<HWND> vecWnd;
CDuiString strTitle;
HWND pForeground = NULL;

CWarnWnd::CWarnWnd(void)
	: m_pCloseBtn(nullptr)
	, m_pBtn1(nullptr)
	, m_pBtn2(nullptr)
	, m_pBtn3(nullptr)
	, m_pTitle(nullptr)
{
}

CWarnWnd::~CWarnWnd(void)
{
}

void CWarnWnd::OnFinalMessage(HWND hWnd)
{
	for (vector<HWND>::iterator it = vecWnd.begin(); it != vecWnd.end(); it++)
	{
		if (*it == hWnd){
			vecWnd.erase(it);
			break;
		}
	}
	__super::OnFinalMessage(hWnd);
	delete this;
}

void CWarnWnd::LvWarnWnd(LPCTSTR pTitle)
{
	pForeground = GetForegroundWindow();
	strTitle = pTitle;
	CWarnWnd *s_pInstance = new CWarnWnd();
	s_pInstance->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_TOOLWINDOW , 0, 0, 0, 0);
	//s_pInstance->Create(NULL, _T(""), UI_WNDSTYLE_FRAME /*WS_POPUPWINDOW*/, 0/*WS_EX_NOACTIVATE*/, 0, 0, 0, 0);
	//s_pInstance->ShowWindow(true, false);
	//::ShowWindow(s_pInstance->GetHWND(), SW_SHOWNOACTIVATE);
	vecWnd.push_back(s_pInstance->GetHWND()); 

	//if (pForeground){
	//	//SetForegroundWindow(pForeground);
	//	//SetFocus(pForeground);
	//	//BringWindowToTop(pForeground);
	//	SetActiveWindow(pForeground);
	//}
		
}

DuiLib::CDuiString CWarnWnd::GetSkinFile()
{
	return _T("WarnWnd.xml");
}

LPCTSTR CWarnWnd::GetWindowClassName(void) const
{
	return _T("MessageBoxWnd");
}

void CWarnWnd::InitWindow()
{		
	//设置窗口靠近屏幕边缘不最大化
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);

	SetTimer(GetHWND(), ACTIVE_TIMERID, 10, NULL);
	
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("close")));
	m_pBtn1 = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn1")));
	m_pBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn2")));
	m_pBtn3 = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn3")));
	m_pTitle = static_cast<CLabelUI*>(m_pm.FindControl(_T("title")));

	m_pTitle->SetText(strTitle);

	//窗口位置
	RECT rc;   
	GetWindowRect(this->GetHWND(), &rc);

	//获取工作区域
	RECT rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);

	////有窗口存在时
	//if (vecWnd.size()){
	//	RECT preRc;
	//	GetWindowRect(vecWnd.back(), &preRc);
	//	if (preRc.left > 0){
	//		if (preRc.top - (rc.bottom - rc.top)>0)
	//			MoveWindow(GetHWND(), preRc.left, preRc.top - (rc.bottom - rc.top), rc.right - rc.left, rc.bottom - rc.top, false);
	//		else{
	//			MoveWindow(GetHWND(), preRc.left - (rc.right - rc.left), rt.bottom - rt.top - (rc.bottom - rc.top), rc.right - rc.left, rc.bottom - rc.top, false);
	//		}
	//	}
	//	else{
	//		MoveWindow(GetHWND(), rt.right - rt.left - (rc.right - rc.left), rt.bottom - rt.top - (rc.bottom - rc.top), rc.right - rc.left, rc.bottom - rc.top, false);
	//	}
	//}
	//else{
	//	MoveWindow(GetHWND(), rt.right - rt.left - (rc.right - rc.left), rt.bottom - rt.top - (rc.bottom - rc.top), rc.right - rc.left, rc.bottom - rc.top, false);
	//}

	//设置窗口一直在最前
	if (vecWnd.size()){
		RECT preRc;
		GetWindowRect(vecWnd.back(), &preRc);
		if (preRc.left > 0){
			if (preRc.top - (rc.bottom - rc.top)>0)
				SetWindowPos(GetHWND(), HWND_TOPMOST, preRc.left, preRc.top - (rc.bottom - rc.top), rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE);
			else{
				SetWindowPos(GetHWND(), HWND_TOPMOST, preRc.left - (rc.right - rc.left), rt.bottom - rt.top - (rc.bottom - rc.top), rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE);
			}
		}
		else{
			SetWindowPos(GetHWND(), HWND_TOPMOST, rt.right - rt.left - (rc.right - rc.left), rt.bottom - rt.top - (rc.bottom - rc.top), rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE);
		}
	}
	else{
		SetWindowPos(GetHWND(), HWND_TOPMOST, rt.right - rt.left - (rc.right - rc.left), rt.bottom - rt.top - (rc.bottom - rc.top), rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE);
	}

}

void CWarnWnd::Notify(TNotifyUI &msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK){
		if (msg.pSender == m_pCloseBtn)
			Close(MSGID_CANCEL);
	}
	return WindowImplBase::Notify(msg);
}

LRESULT CWarnWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MOUSEMOVE){

	}
	else if (uMsg == WM_TIMER){
		OnTimer((UINT_PTR)wParam);
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CWarnWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_ACTIVATE == uMsg)
		return 0;
	if (WM_NCLBUTTONDBLCLK != uMsg)
		return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	else
		return 0;
}

void CWarnWnd::OnTimer(UINT_PTR idEvent)
{
	if (idEvent != ACTIVE_TIMERID)
		return;
	KillTimer(GetHWND(), ACTIVE_TIMERID);
	if (pForeground){
		SetActiveWindow(pForeground);
	}
}