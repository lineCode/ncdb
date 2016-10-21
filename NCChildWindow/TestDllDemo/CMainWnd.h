#pragma once
//////////////////////////////////////////////////////////////////////////
///
class CMainWnd : public WindowImplBase
{
public:
	CMainWnd(void);
	~CMainWnd(void);

public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();
	virtual void SetWndSize(int cx,int cy);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	CButtonUI* m_pTestBtn;
	CButtonUI* m_pTest2Btn;
	CButtonUI* m_pTest3Btn;
};