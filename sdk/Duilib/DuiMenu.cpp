#include "StdAfx.h"
#include "DuiMenu.h"
#include "functional.h"

CDuiMenu::CDuiMenu( HWND hParent /*= NULL*/ )
:CMenuWnd(hParent)
{

}

CDuiMenu::~CDuiMenu(void)
{
}


bool CDuiMenu::Init( CMenuElementUI* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point )
{
	m_pm.AddNotifier(this);
	__super::Init(pOwner, xml, pSkinType, point);
	return true;
}






