#pragma once
//////////////////////////////////////////////////////////////////////////
///
#define SIZE_TIMERID	    UIMSG_USER+1         //
class CMessageBoxWnd : public CBaseWnd
{
public:
	CMessageBoxWnd(void);
	~CMessageBoxWnd(void);
public:
	//virtual void OnFinalMessage(HWND);
	//virtual CDuiString GetSkinFile();
	//virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();
	static int LvMessageBox(HWND hParent, LPCTSTR lpstrMsg, LPCTSTR lpstrTitle, UINT uType, UINT uIcon, POINT pPosition,BOOL bCheck);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void InitBtn();
	void InitIcon();
	void OnTimer(UINT_PTR idEvent);
	void SetWndAttribute();
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pBtn1;
	CButtonUI* m_pBtn2;
	CButtonUI* m_pBtn3;
	CLabelUI* m_pTitle;
	CRichEditUI* m_pMessage;
	CControlUI* m_pIcon;
	CControlUI* m_pUpCtrl;
	COptionUI* m_pNoWarn;

	CDuiString m_strTitle;
	CDuiString m_strMsg;
	unsigned int uType;
	unsigned int uIcon;
	POINT posWnd;
	bool bCheck;
};