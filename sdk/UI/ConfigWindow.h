#pragma once
#include "PiIni.h"

struct tagWindow
{
	tstring strTitle;
	tstring strClassName;
	tagWindow(const tstring& strTitleR, const tstring& strClassNameR)
		:strTitle(strTitleR)
		,strClassName(strClassNameR)
	{

	}
};
typedef list<tagWindow>	LST_WINDOW;


class CConfigWindow :
	public ns_PiPi::CPiIni
{
public:
	

	CConfigWindow(tcpchar szConfigPath = NULL);
	~CConfigWindow(void);
public:
	bool GetWindowList( LST_WINDOW& lstWnd );
protected:
	bool AnalyDesc( const tstring& strWindowDesc, LST_WINDOW& lstWnd );
};
