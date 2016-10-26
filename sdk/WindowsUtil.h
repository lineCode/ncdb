#pragma once

#include "WindowsUtilDef.h"
#include "ListMng.h"

class CWindowsUtil: public CListMng<tagWindowInfo, LST_WINDOW>
{
public:
    CWindowsUtil(void);
    ~CWindowsUtil(void);
public:
    bool Init();
    bool EnumAllTopWindow();
    tagWindowInfo* GetWindowsFullScreen();
    bool FilterWnd( tagWindowInfo* pWndInfo );
	
protected:
private:
    list<tstring>               m_lstFilder;    //要过滤的窗口类名
};


BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam);