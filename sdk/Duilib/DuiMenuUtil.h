#pragma once
#include "duilib\DuiMenu.h"

#define UM_DUI_MENU_CLICK		WM_USER + 800		//菜单点击事件

class CDuiMenuUtil : public CDuiMenu
{
public:
	CDuiMenuUtil(HWND hParent = NULL);
	~CDuiMenuUtil(void);
public:
	static bool PopupMenu(LPTSTR xml, POINT point, HWND hWnd);
private:
	virtual void Notify(TNotifyUI& msg);
	virtual bool Popup( CMenuElementUI* pOwner, POINT point);
};
