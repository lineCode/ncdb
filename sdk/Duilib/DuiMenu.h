#pragma once
#include "UIMenu.h"
#include "Core\UIManager.h"

class CDuiMenu :
	public CMenuWnd, INotifyUI
{
public:
	CDuiMenu(HWND hParent = NULL);
	~CDuiMenu(void);
public:
	bool Init(CMenuElementUI* pOwner, STRINGorID xml, LPCTSTR pSkinType, POINT point);	//Ìí¼Ó¼àÌýNotify
	virtual bool Popup( CMenuElementUI* pOwner, POINT point) = 0;
	//virtual void Notify(TNotifyUI& msg);

};
