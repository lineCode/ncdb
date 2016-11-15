#pragma once
//////////////////////////////////////////////////////////////////////////
///
#define MOVEWND_TIMERID	    UIMSG_USER+3 
#define ACTIVE_TIMERID	    UIMSG_USER+2  
#define WND_HEIGHT          130                   //窗口高度
#define WND_WIDTH           335                   //窗口宽度
#define GAP_WIDTH           4                     //窗口间距宽度
#define GAP_HEIGHT          4                     //窗口间距高度
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
	virtual LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	void OnTimer(UINT_PTR idEvent);
	void OnMoveWndTimer(UINT_PTR idEvent);
	void SetWndAttribute();
	void SetWndPos();
private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pBtn1;
	CButtonUI* m_pBtn2;
	CButtonUI* m_pBtn3;
	CLabelUI* m_pTitle;
	int wndHeight;
	int oneStepLength;   //动画移动一步距离
	int wndDistance;     //两窗口间距离
	HWND overStepWnd;
	int closeIndex;     //关闭窗口的索引
	int topIndex;       //所关闭列最上面一个窗口索引（如果所关闭列最上方没有窗口，则为前一列最上方）
};