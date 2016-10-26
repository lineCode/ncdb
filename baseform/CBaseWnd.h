#pragma once
//////////////////////////////////////////////////////////////////////////
///
#include "..\DuiLibEx\UIChildLayoutEx.h"
#include "..\DuiLibEx\UIOptionEx.h"
#include "..\DuiLibEx\UIComboEx.h"
class CBaseWnd : public DuiLib::WindowImplBase
{
public:
	CBaseWnd(void);
	~CBaseWnd(void);
	
public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();
	virtual void SetSkinFile(LPCTSTR pstrName);
	virtual void SetWndSize(int cx,int cy);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	CChildLayoutUIEx* m_pChildLayout;
};