// NCChildWindow.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "NCChildWindow.h"
#include "CBaseWnd.h"


// 这是导出变量的一个示例
NCCHILDWINDOW_API int nNCChildWindow=0;

// 这是导出函数的一个示例。
NCCHILDWINDOW_API int fnNCChildWindow(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 NCChildWindow.h
CNCChildWindow::CNCChildWindow()
{
	return;
}

NCCHILDWINDOW_API HWND CShowChildWindow(TCFormFrame frame, TCFormSysButton button, TCFormInitCallBack callback)
{
	// 初始化UI管理器
	CPaintManagerUI::SetInstance(GetModuleHandle(NULL));
	// 初始化资源
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	strResourcePath += _T("skin\\");
	CPaintManagerUI::SetResourcePath(strResourcePath.GetData());

	//CBaseWnd* pFrame = new CBaseWnd(frame, button);
	//if (pFrame == NULL) return NULL;
	//pFrame->Create(frame.pWnd, _T("BaseWnd"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
	//pFrame->CenterWindow();
	bool btnStatus[5] = {0};
	btnStatus[0] = button.sbClose;
	btnStatus[1] = button.sbFeedback;
	btnStatus[2] = button.sbMax;
	btnStatus[3] = button.sbMin;
	btnStatus[4] = button.sbOther;
	CBaseWnd* pFrame = CBaseWnd::getInstance(frame.pWnd, frame.caption, frame.path, btnStatus);

	RECT rc;   //窗口位置  父窗口位置
	GetWindowRect(pFrame->GetHWND(), &rc);
	callback(pFrame->GetHWND(), rc.right - rc.left, rc.bottom - rc.top);
	return pFrame->GetHWND();
}
