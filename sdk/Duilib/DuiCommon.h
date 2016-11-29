#pragma once

class CDuiCommon
{
public:
	CDuiCommon(DuiLib::CPaintManagerUI* pPaintManager = NULL);
	~CDuiCommon(void);
public:
	static bool IsStaticControl(DuiLib::CControlUI* pControl);
	static bool IsControl(DuiLib::CControlUI* pSender, tcpchar szName);
	void SetVisible( LPCTSTR param1, bool bShow);
public:
	//static 
	DuiLib::CPaintManagerUI*				m_pPaintManager;
};
