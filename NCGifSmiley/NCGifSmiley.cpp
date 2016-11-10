// NCGifSmiley.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "NCGifSmiley.h"
#include "MainWnd.h"


// 这是导出变量的一个示例
NCGIFSMILEY_API int nNCGifSmiley=0;

// 这是导出函数的一个示例。
NCGIFSMILEY_API int fnNCGifSmiley(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 NCGifSmiley.h
CNCGifSmiley::CNCGifSmiley()
{
	return;
}

CDuiString path;
void initDuilib()
{
	CPaintManagerUI::SetInstance(GetModuleHandle(NULL));
	// 初始化资源
	path = CPaintManagerUI::GetResourcePath();
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	strResourcePath += _T("..\\Skins\\Default\\GifSkin\\");
	CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);    //CRT检测是否有内存泄漏，检测结果在output窗口输出
}


NCGIFSMILEY_API void CShowGifWindow()
{
	initDuilib();
	CMainWnd* pFrame = new CMainWnd();
	if (pFrame == NULL) return;
	pFrame->Create(NULL, _T("T2Lv"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
	pFrame->CenterWindow();
	pFrame->ShowWindow();
	//if (!path.IsEmpty())
	//	CPaintManagerUI::SetResourcePath(path.GetData());
}