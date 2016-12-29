#pragma once
#include "E:\work\svn\nc\src\DuiLib\Core\UIControl.h"


//////////////////////////////////////////////////////////////////////////  
// CIconUI  
class CIconUI : public DuiLib::CControlUI
{
	friend class CIconWnd;
public:
	CIconUI(void);
	~CIconUI(void);

	LPCTSTR GetClass() const;
	LPVOID  GetInterface(LPCTSTR pstrName);
	void    SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	
	void    SetPos(RECT rc);
	void    SetInset(RECT rc);
	void    SetVisible(bool bVisible);
	void    SetInternVisible(bool bVisible);
	void    DoPaint(HDC hDC, const RECT& rcPaint);
	void    DoEvent(TEventUI& event);
private:
	DuiLib::CDuiString  m_sIcoImage;
	HICON				m_hIcon;
	RECT                m_rcInset;
};