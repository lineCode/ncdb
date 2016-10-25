#include "StdAfx.h"
#include "DuiMenuUtil.h"



CDuiMenuUtil::CDuiMenuUtil( HWND hParent /*= NULL*/ )
:CDuiMenu(hParent)
{

}

CDuiMenuUtil::~CDuiMenuUtil(void)
{
}

bool CDuiMenuUtil::PopupMenu( LPTSTR xml, POINT point, HWND hWnd )
{
	CDuiMenuUtil* pMenu = new CDuiMenuUtil(hWnd);
	
	if(!pMenu->Init(NULL, xml, _T("xml"), point))
	{
		return false;
	}
	return true;
}

bool CDuiMenuUtil::Popup( CMenuElementUI* pOwner, POINT point )
{
	return Init(pOwner, _T("menu.xml"), _T("xml"), point);
}

void CDuiMenuUtil::Notify( TNotifyUI& msg )
{
	if (msg.sType == DUI_MSGTYPE_ITEMCLICK)
	{
		CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(msg.pSender);
		int nIndex = pMenuItem->GetIndex();
		CDuiString strItemName = pMenuItem->GetName();
		//OutInfo(_T("µã»÷ÁË²Ëµ¥:"), strItemName);
		::SendMessage(GetParent(m_hWnd), UM_DUI_MENU_CLICK, (WPARAM)&strItemName, 0);
	}
}
