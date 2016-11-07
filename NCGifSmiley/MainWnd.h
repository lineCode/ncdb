#pragma once
//////////////////////////////////////////////////////////////////////////
///
#include "UIGifAnimEx.h"

class CMainWnd : public WindowImplBase
{
public:
	CMainWnd(void);
	~CMainWnd(void);

public:
	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	virtual void InitWindow();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	void LoadGif();
	void LoadSelfGif();

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CVerticalLayoutUI* m_pGifLayout;
	CVerticalLayoutUI* m_pSelfLayout;
	CListUI* m_pList;
	CListContainerElementUI* m_pListEle;

	CMenuWnd*  m_pMenu;
	vector<CGifAnimUIEx*> vecGif;
};
