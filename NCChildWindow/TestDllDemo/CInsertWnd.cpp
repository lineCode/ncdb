#include "stdafx.h"
#include "CInsertWnd.h"

CInsertWnd::CInsertWnd(void)
	:m_pTestBtn(nullptr)
{
}

CInsertWnd::~CInsertWnd(void)
{
}

void CInsertWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CInsertWnd::GetSkinFile()
{
	return _T("insert.xml");
}

LPCTSTR CInsertWnd::GetWindowClassName(void) const
{
	return _T("insertWnd");
}

void CInsertWnd::SetWndSize(int cx, int cy)
{
	ResizeClient(cx, cy);
}
void CInsertWnd::InitWindow()
{	
}

void CInsertWnd::Notify(TNotifyUI &msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK){
		if (msg.pSender == m_pTestBtn)
		{
		}
	}
	else if (msg.sType == DUI_MSGTYPE_VALUECHANGED)
	{

	}
	return WindowImplBase::Notify(msg);
}

LRESULT CInsertWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MOUSEMOVE){

	}
	else if (uMsg == WM_LBUTTONDOWN){
		
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CInsertWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_NCLBUTTONDBLCLK != uMsg)
		return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	else
		return 0;
}