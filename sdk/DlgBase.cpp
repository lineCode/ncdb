#include "StdAfx.h"
#include "DlgBase.h"

CDlgBase::CDlgBase( UINT nIDTemplate, CWnd* pParentWnd /*= NULL*/ )
#ifdef M_DIALOG_EX
:CDialogEx(nIDTemplate, pParentWnd)
#else
:CDialog(nIDTemplate, pParentWnd)
#endif
{
	bEnableF4Close = false;
}

CDlgBase::~CDlgBase(void)
{
}

CDlgBase::operator HWND()
{
	return m_hWnd;
}


BOOL CDlgBase::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)  
	{
		switch(pMsg->wParam) 
		{  
		case VK_RETURN:   
		case VK_ESCAPE:  
			return TRUE; 
		}
	}

	if (!bEnableF4Close &&
		(WM_SYSKEYDOWN == pMsg->message && VK_F4 == pMsg->wParam)
		)
	{
		return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}

bool CDlgBase::SendMessageBtnOk( bool bPost /*= false*/ )
{
	if (bPost)
	{
		PostMessage(WM_COMMAND, IDOK);
	}
	else
	{
		SendMessage(WM_COMMAND, IDOK);
	}
	return true;
}

bool CDlgBase::SendMessageBtnCancel( bool bPost /*= false*/ )
{
	if (bPost)
	{
		PostMessage(WM_COMMAND, IDCANCEL);
	}
	else
	{
		SendMessage(WM_COMMAND, IDCANCEL);
	}
	return true;
}
