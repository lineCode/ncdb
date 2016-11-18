#pragma once
//////////////////////////////////////////////////////////////////////////
///
#include "UIGroupsTreelist.h"
#include "UIOrganizationTreelist.h"

#define CLOSE_WND_TIMER       UIMSG_USER+101
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
	void OnTimer(UINT_PTR idEvent);

	void InitListData();
	void InitOriganizationData();
	void LoadGroup();
	void LoadOriganization();
	CDuiString IntToDuistring(const int num);

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	CButtonUI* m_pCloseBtn;
	CGroupsTreelistUI*  m_pGroupList;
	COrganizationTreelistUI* m_pOrganizationList;
	vector<GroupsListItemInfo> vecListInfo;
	vector<OrganizationListItemInfo> vecOrganizationInfo;
	CMenuWnd*  m_pMenu;
};
