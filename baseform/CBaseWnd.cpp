#include "stdafx.h"
#include "CBaseWnd.h"
#include "..\..\DuiLib\UIlib.h"
using namespace DuiLib;

CBaseWnd::CBaseWnd(void)
	: m_pChildLayout(nullptr)
{
}

CBaseWnd::~CBaseWnd(void)
{
}

void CBaseWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CBaseWnd::GetSkinFile()
{
	return _T("..\\Common\\base.xml");
}

LPCTSTR CBaseWnd::GetWindowClassName(void) const
{
	return _T("BaseWnd");
}

CControlUI* CBaseWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("OptionEx")) == 0){
		return new COptionUIEx();
	}
	else if (_tcsicmp(pstrClass, _T("ChildLayoutEx")) == 0){
		return new CChildLayoutUIEx();
	}
	else if (_tcsicmp(pstrClass, _T("ComboEx")) == 0){
		return new CComboUIEx();
	}
	return NULL;
}

void CBaseWnd::SetSkinFile(LPCTSTR pstrName)
{
	m_pChildLayout = static_cast<CChildLayoutUIEx*>(m_pm.FindControl(_T("childxml")));
	m_pChildLayout->SetChildLayoutXML(pstrName);
	m_pChildLayout->Init(this);
}

void CBaseWnd::SetWndSize(int cx, int cy)
{
	ResizeClient(cx, cy);
}
void CBaseWnd::InitWindow()
{	

}

void CBaseWnd::Notify(TNotifyUI &msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK){

	}
	else if (msg.sType == DUI_MSGTYPE_VALUECHANGED)
	{

	}
	return WindowImplBase::Notify(msg);
}

LRESULT CBaseWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MOUSEMOVE){

	}
	else if (uMsg == WM_LBUTTONDOWN){
		
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CBaseWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_NCLBUTTONDBLCLK != uMsg)
		return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	else
		return 0;
}