#pragma once

class CDuiCommon
{
public:
	CDuiCommon(CPaintManagerUI* pPaintManager = NULL);
	~CDuiCommon(void);
public:
	static bool IsStaticControl( CControlUI* pControl );
	static bool IsControl( CControlUI* pSender, tcpchar szName );
	void SetVisible( LPCTSTR param1, bool bShow);
public:
	//static 
	CPaintManagerUI*				m_pPaintManager;
};
