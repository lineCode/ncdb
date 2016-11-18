// NCTreeList.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "NCTreeList.h"
#include "MainWnd.h"

void initDuilib()
{
	CPaintManagerUI::SetInstance(GetModuleHandle(NULL));
	// 初始化资源
	CDuiString path = CPaintManagerUI::GetResourcePath();
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	strResourcePath += _T("..\\Skins\\Default\\TreeListSkin\\");
	CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);    //CRT检测是否有内存泄漏，检测结果在output窗口输出
}
// 这是导出函数的一个示例。
NCTREELIST_API void fnNCTreeList(void)
{
	initDuilib();
	CMainWnd* pFrame = new CMainWnd();
	if (pFrame == NULL) return;
	pFrame->Create(NULL, _T("TreeListWnd"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
	pFrame->CenterWindow();
}