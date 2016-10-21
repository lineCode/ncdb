// NCMessageBox.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "NCMessageBox.h"
#include "MessageBoxWnd.h"

NCMESSAGEBOX_API int NCMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, UINT uIcon,POINT pPosition)
{
	CPaintManagerUI::SetInstance(GetModuleHandle(NULL));
	// 初始化资源
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	strResourcePath += _T("MessageBoxSkin\\");
	CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);    //CRT检测是否有内存泄漏，检测结果在output窗口输出
	
	return CMessageBoxWnd::LvMessageBox(hWnd, lpText, lpCaption, uType, uIcon, pPosition);
}