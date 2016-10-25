#pragma once
#include "afxwin.h"

class CDlgBase :
#ifdef M_DIALOG_EX
	public CDialogEx
#else
	public CDialog
#endif
{
public:
	CDlgBase(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	~CDlgBase(void);
public:
	/*-------------------- 屏蔽按键 --------------------*/
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool SendMessageBtnOk(bool bPost = false);
	bool SendMessageBtnCancel(bool bPost = false);

	/*-------------------- 对外接口 --------------------*/
	void EnableF4Close(bool bEnable) { bEnableF4Close = bEnable; }

public:
	operator HWND();

private:
	bool bEnableF4Close;
};
