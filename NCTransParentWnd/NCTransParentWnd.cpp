// NCTransParentWnd.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "NCTransParentWnd.h"
#include "MainWnd.h"

void initDuilib()
{
	CPaintManagerUI::SetInstance(GetModuleHandle(NULL));
	// 初始化资源
	CDuiString path = CPaintManagerUI::GetResourcePath();
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	strResourcePath += _T("..\\Skins\\Default\\TransparentSkin\\");
	CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);    //CRT检测是否有内存泄漏，检测结果在output窗口输出
}

NCTRANSPARENTWND_API void CShowTransParentWindow(HWND pWnd)
{
	initDuilib();
	CMainWnd* pFrame = new CMainWnd();
	if (pFrame == NULL) return;
	pFrame->Create(pWnd, _T("TransParentWnd"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
	pFrame->CenterWindow();
}