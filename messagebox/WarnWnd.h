#pragma once
//////////////////////////////////////////////////////////////////////////
///
#define ACTIVE_TIMERID	    UIMSG_USER+2  
#define WND_HEIGHT          130
#define WND_WIDTH           335
class CWarnWnd : public CBaseWnd
{
public:
	CWarnWnd(void);
	~CWarnWnd(void);
public:
	virtual void OnFinalMessage(HWND);
	//virtual CDuiString GetSkinFile();
	//virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();
	static void LvWarnWnd(LPCTSTR pTitle);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnTimer(UINT_PTR idEvent);
	//void OnMoveWndTimer(UINT_PTR idEvent);
	void SetWndAttribute();
	void SetWndPos();
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pBtn1;
	CButtonUI* m_pBtn2;
	CButtonUI* m_pBtn3;
	CLabelUI* m_pTitle;
};