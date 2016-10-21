#include "stdafx.h"
#include "MessageBoxWnd.h"
#include "NCMessageBox.h"

CMessageBoxWnd::CMessageBoxWnd(void)
	: m_pCloseBtn(nullptr)
	, m_pBtn1(nullptr)
	, m_pBtn2(nullptr)
	, m_pBtn3(nullptr)
	, m_pTitle(nullptr)
	, m_pMessage(nullptr)
	, m_pIcon(nullptr)
	, m_pUpCtrl(nullptr)
	, m_pNoWarn(nullptr)
{
}

CMessageBoxWnd::~CMessageBoxWnd(void)
{
}

void CMessageBoxWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

int CMessageBoxWnd::LvMessageBox(HWND hParent, LPCTSTR lpstrMsg, LPCTSTR  lpstrTitle, UINT uType, UINT uIcon, POINT pPosition, BOOL bCheck)
{
	CMessageBoxWnd *s_pInstance = new CMessageBoxWnd();
	s_pInstance->m_strTitle = lpstrTitle;
	s_pInstance->m_strMsg = lpstrMsg;
	s_pInstance->uType = uType;
	s_pInstance->uIcon = uIcon;
	s_pInstance->posWnd = pPosition;
	s_pInstance->bCheck = bCheck;
	s_pInstance->Create(hParent, lpstrTitle, UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
	if (pPosition.x == -1 && pPosition.y == -1)
		s_pInstance->CenterWindow();
	if (uType == BTN_OK)
		return MSGID_OK;
		
	return s_pInstance->ShowModal();
}

DuiLib::CDuiString CMessageBoxWnd::GetSkinFile()
{
	return _T("MessageBoxWnd.xml");
}

LPCTSTR CMessageBoxWnd::GetWindowClassName(void) const
{
	return _T("MessageBoxWnd");
}

void CMessageBoxWnd::InitWindow()
{		
	//设置窗口靠近屏幕边缘不最大化
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);

	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("close")));
	m_pBtn1 = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn1")));
	m_pBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn2")));
	m_pBtn3 = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn3")));
	m_pTitle = static_cast<CLabelUI*>(m_pm.FindControl(_T("title")));
	m_pIcon = static_cast<CControlUI*>(m_pm.FindControl(_T("icon")));
	m_pMessage = static_cast<CRichEditUI*>(m_pm.FindControl(_T("message")));
	m_pUpCtrl = static_cast<CControlUI*>(m_pm.FindControl(_T("upCtrl")));
	m_pNoWarn = static_cast<COptionUI*>(m_pm.FindControl(_T("noWarn")));
	
	m_pTitle->SetText(m_strTitle);
	m_pMessage->SetText(m_strMsg);
	m_pNoWarn->SetVisible(bCheck);

	InitBtn();
	InitIcon();	

	if (posWnd.x != -1 || posWnd.y != -1){
		RECT rc;   //窗口位置
		GetWindowRect(this->GetHWND(), &rc);
		MoveWindow(GetHWND(), posWnd.x, posWnd.y, rc.right - rc.left, rc.bottom - rc.top, true);
	}
	SetTimer(GetHWND(), SIZE_TIMERID, 20, NULL);
}

void CMessageBoxWnd::Notify(TNotifyUI &msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK){
		if (msg.pSender == m_pCloseBtn)
			Close(MSGID_CANCEL);
		if (uType == BTN_OK){
			if (msg.pSender == m_pBtn3){
				if (m_pNoWarn->IsVisible() && m_pNoWarn->IsSelected())
					Close(MSGID_OK_NOWARN);
				else
					Close(MSGID_OK);
			}		
		}
		else if (uType == BTN_OKCANCEL||uType==BTN_YESNO){
			if (m_pNoWarn->IsVisible() && m_pNoWarn->IsSelected()){
				if (msg.pSender == m_pBtn2)
					Close(MSGID_OK_NOWARN);
				else if (msg.pSender == m_pBtn3)
					Close(MSGID_CANCEL_NOWARN);
			}
			else{
				if (msg.pSender == m_pBtn2)
					Close(MSGID_OK);
				else if (msg.pSender == m_pBtn3)
					Close(MSGID_CANCEL);
			}
		}
		else if (uType == BTN_YESRETRYCANCEL){
			if (m_pNoWarn->IsVisible() && m_pNoWarn->IsSelected()){
				if (msg.pSender == m_pBtn1)
					Close(MSGID_OK_NOWARN);
				else if (msg.pSender == m_pBtn2)
					Close(MSGID_RETRY_NOWARN);
				else if (msg.pSender == m_pBtn3)
					Close(MSGID_CANCEL_NOWARN);
			}
			else{
				if (msg.pSender == m_pBtn1)
					Close(MSGID_OK);
				else if (msg.pSender == m_pBtn2)
					Close(MSGID_RETRY);
				else if (msg.pSender == m_pBtn3)
					Close(MSGID_CANCEL);
			}
		}
	}
	return WindowImplBase::Notify(msg);
}

LRESULT CMessageBoxWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MOUSEMOVE){

	}
	else if (uMsg == WM_TIMER){
		OnTimer((UINT_PTR)wParam);
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CMessageBoxWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_NCLBUTTONDBLCLK != uMsg)
		return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	else
		return 0;
}

void CMessageBoxWnd::InitBtn()
{
	if (uType == BTN_OK){
		m_pBtn1->SetVisible(false);
		m_pBtn2->SetVisible(false);
		m_pBtn3->SetText(_T("确定"));
	}
	else if (uType == BTN_OKCANCEL){
		m_pBtn1->SetVisible(false);
		m_pBtn2->SetText(_T("确定"));
		m_pBtn3->SetText(_T("取消"));
	}
	else if (uType == BTN_YESNO){
		m_pBtn1->SetVisible(false);
		m_pBtn2->SetText(_T("是"));
		m_pBtn3->SetText(_T("否"));
	}
	else if (uType == BTN_YESRETRYCANCEL){
		m_pBtn1->SetText(_T("确定"));
		m_pBtn2->SetText(_T("重试"));
		m_pBtn3->SetText(_T("取消"));
	}
}

void CMessageBoxWnd::InitIcon()
{
	CDuiString iconPath;
	switch (uIcon)
	{
	case NCICON_QUESTION:
		iconPath = _T("Ask.png");
		break;
	case NCICON_ERROR:
		iconPath = _T("Error.png");
		break;
	case NCICON_WARN:
		iconPath = _T("Warn.png");
		break;
	case NCICON_HINT:
		iconPath = _T("Hint.png");
		break;
	case NCICON_CORRECT:
		iconPath = _T("Correct.png");
		break;
	default:
		break;
	}
	m_pIcon->SetBkImage(iconPath);
}

void CMessageBoxWnd::OnTimer(UINT_PTR idEvent)
{
	if (idEvent != SIZE_TIMERID)
		return;
	KillTimer(GetHWND(), SIZE_TIMERID);
	
	int lineCount = m_pMessage->GetLineCount();
	if (lineCount != 1)
		m_pUpCtrl->SetFixedHeight(20);
	if (lineCount > 2){
		int height = 0;
		if (lineCount < 30)              //如果文字超过30行，窗口就默认为30行大小
			height = (lineCount - 2) * 20;
		else
			height = (30 - 2) * 20;
		
		RECT rc;   //窗口位置
		GetWindowRect(this->GetHWND(), &rc);
		int editHeight = m_pMessage->GetHeight();
		m_pMessage->SetFixedHeight(editHeight + height);
		ResizeClient(rc.right - rc.left, rc.bottom - rc.top + height);
		if (posWnd.x == -1 && posWnd.y == -1)
			this->CenterWindow();
	}

}