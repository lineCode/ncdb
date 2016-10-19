#include "stdafx.h"
#include "CMainWnd.h"
#include "CInsertWnd.h"
#include "..\NCChildWindow\NCChildWindow.h"
#pragma comment(lib, "..\\Lib\\NCChildWindow.lib")

CMainWnd::CMainWnd(void)
	:m_pTestBtn(nullptr)
{
}

CMainWnd::~CMainWnd(void)
{
}

void CMainWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CMainWnd::GetSkinFile()
{
	return _T("main.xml");
}

LPCTSTR CMainWnd::GetWindowClassName(void) const
{
	return _T("MainWnd");
}

void CMainWnd::SetWndSize(int cx, int cy)
{
	ResizeClient(cx, cy);
}
void CMainWnd::InitWindow()
{	
	m_pTestBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("testBtn")));
}
typedef	int(*TCFormInitCallBack)(const HWND AFormHandle, const int AWidth, int AHeight);
int InitCallBack(const HWND AFormHandle, const int AWidth, int AHeight)
{
	CInsertWnd* pFrame = new CInsertWnd();
	if (pFrame == NULL) return 0;
	pFrame->Create(AFormHandle, _T("Insert"), UI_WNDSTYLE_CHILD, 0L, 0, 0, 0, 0);
	//pFrame->CenterWindow();
	RECT rc, prc;   //窗口位置  父窗口位置
	GetWindowRect(pFrame->GetHWND(), &rc);
	GetWindowRect(AFormHandle, &prc);
	MoveWindow(pFrame->GetHWND(), 0, 28, prc.right - prc.left, prc.bottom - prc.top - 28, true);
	//MessageBox(NULL, _T(""), _T(""), MB_OK);
	return 0;
}

void CMainWnd::Notify(TNotifyUI &msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK){
		if (msg.pSender == m_pTestBtn)
		{
			TCFormFrame frame;
			frame.caption = "标题栏zzzzzz";
			frame.path = "Logo.png";
			frame.pWnd = GetHWND();
			TCFormSysButton button;
			button.sbClose = true;
			button.sbMax = false;
			button.sbFeedback = false;
			button.sbMin = true;
			button.sbOther = false;
			TCFormInitCallBack callback;
			callback = InitCallBack;
			CShowChildWindow(frame, button, callback);
			
		}
	}
	else if (msg.sType == DUI_MSGTYPE_VALUECHANGED)
	{

	}
	return WindowImplBase::Notify(msg);
}

LRESULT CMainWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MOUSEMOVE){

	}
	else if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN){
		
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_NCLBUTTONDBLCLK != uMsg)
		return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	else
		return 0;
}