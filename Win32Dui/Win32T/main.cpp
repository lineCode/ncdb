#include <windows.h>
#include "..\..\..\nc\src\samples\UseNCCef\NCCef.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	CNCCef* m_pCef = new CNCCef(hInstance);

	m_pCef->Parent(NULL);
	OutputDebugString(L"cef enter\n");
	int nRet = 0;
	if (nRet = m_pCef->NeedReturn() >= 0)
	{
		OutputDebugString(L"cef enter return\n");
		return nRet;
	}
	m_pCef->Init(false);
	//m_pCef->Create(NULL);
	m_pCef->Start();
	OutputDebugString(L"cef will Uninit\n");
	m_pCef->UnInit();
	//m_pCef->Load(_T("sdf"));
	return 1;
}