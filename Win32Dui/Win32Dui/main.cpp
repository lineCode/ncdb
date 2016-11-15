#include <windows.h>
#include "Duilib/DuiHeader.h"
#include "DuiMain.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	// 初始化UI管理器
	CPaintManagerUI::SetInstance(hInstance);
	// 初始化资源
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	strResourcePath += _T("skin\\");
	CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
	CDuiMain* pMain = new CDuiMain;
	pMain->Create(NULL, _T("LogIn"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
	pMain->ShowModal();

	return 1;
}