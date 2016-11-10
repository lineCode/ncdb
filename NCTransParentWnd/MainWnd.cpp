#include "StdAfx.h"
#include "MainWnd.h"
//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CMainWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()
CMainWnd::CMainWnd(void)
{
}

CMainWnd::~CMainWnd(void)
{
}

void CMainWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CMainWnd::GetSkinFile()
{
	return _T("skin.xml");
}

LPCTSTR CMainWnd::GetWindowClassName( void ) const
{
	return _T("MainWnd");
}

void CMainWnd::InitWindow()
{
	SetTimer(GetHWND(), CLOSE_WND_TIMER,3000, NULL);
}

void CMainWnd::OnClick( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CMainWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
 {
	 if (uMsg == WM_MENUCLICK)
	 {
	 }
	 else if (uMsg == WM_TIMER){
		 OnTimer((UINT_PTR)wParam);
	 }
	 bHandled = FALSE;
	 return 0;
 }
void CMainWnd::Notify( TNotifyUI &msg )
{
	if (msg.sType == DUI_MSGTYPE_SCROLL){
	}
	return WindowImplBase::Notify(msg);
}

void CMainWnd::OnTimer(UINT_PTR idEvent)
{
	if (idEvent != CLOSE_WND_TIMER)
		return;
	KillTimer(GetHWND(), CLOSE_WND_TIMER);
	this->Close();
}