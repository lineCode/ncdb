#include "stdafx.h"
#include "PiHook.h"
#include "PiTypeDef.h"
#include "PiMsgDef.h"
#include "functional.h"

#pragma data_seg("mydata")

HWND g_hWndHooker = NULL; // 显示目标窗口标题编辑框的句柄
HHOOK g_hHook = NULL; // 安装的鼠标钩子句柄
HINSTANCE glhInstance = NULL; // DLL实例句柄

#pragma data_seg()
#pragma comment(linker, "/SECTION:mydata,RWS")

FunWndProc g_WndProcOld;
CPiHook::CPiHook()
{
	m_hDll = nullptr;
}


CPiHook::~CPiHook()
{

	if (m_hDll)
	{
		FreeLibrary(m_hDll);
	}
}

bool CPiHook::StartHook(HWND hWnd)
{
	BOOL result = FALSE;

	// 安装钩子
	m_hDll = LoadLibrary(_T("DllHook.dll"));
	g_hHook = SetWindowsHookEx(WH_MOUSE, MouseProc, m_hDll, 0);

	if (g_hHook != NULL)
		g_hWndHooker = hWnd;
		result = TRUE;

	//glhDisplayWnd = hWnd; // 设置显示目标窗口标题编辑框的句柄

	return result;
}

bool CPiHook::StopHook()
{
	BOOL result = FALSE;

	if (g_hHook){

		result = UnhookWindowsHookEx((HHOOK)g_hHook); // 卸载钩子

		if (result)
		{
			g_hWndHooker = NULL;
			g_hHook = NULL;
		}

	}

	return result;
}

bool CPiHook::StartWndProcHook(HWND hWndUI, HMODULE hDll)
{
	BOOL result = FALSE;
	// 安装钩子

	if (!hDll)
	{
		hDll = LoadLibrary(_T("DllHook.dll"));
		m_hDll = hDll;
	}

	HWND hWnd = FindWindowEx(NULL, NULL, _T("Shell_TrayWnd"), NULL);
	hWnd = FindWindowEx(hWnd, NULL, _T("ReBarWindow32"), NULL);

	//HWND hWnd = FindWindow(_T("Notepad"), NULL);

	DWORD dwThread = GetWindowThreadProcessId(hWnd, NULL);
	g_hHook = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, hDll, dwThread);

	if (g_hHook != NULL)
	{
		hWndUI = hWndUI ? hWndUI : g_hWndHooker;
		g_hWndHooker = hWndUI;
	}
	result = TRUE;
	OutInfo(_T("设置钩子时窗口句柄"), (int)g_hWndHooker);
	//glhDisplayWnd = hWnd; // 设置显示目标窗口标题编辑框的句柄

	return result;
}

bool CPiHook::StartGetMessageHook(HWND hWndNotify, HMODULE hDll)
{ 
	BOOL result = FALSE;
	// 安装钩子

	if (!hDll)
	{
		hDll = LoadLibrary(_T("DllHook.dll"));
		m_hDll = hDll;
	}

	/*HWND hWnd = FindWindowEx(NULL, NULL, _T("Shell_TrayWnd"), NULL);
	hWnd = FindWindowEx(hWnd, NULL, _T("ReBarWindow32"), NULL);*/

	//HWND hWnd = FindWindowEx(NULL, NULL, _T("Notepad"), NULL);
	HWND hWnd = FindWindowEx(NULL, NULL, _T("wndclass_desked_gsk"), NULL);
	

	if (!hWnd)
	{
		return false;
	}
	DWORD dwThread = GetWindowThreadProcessId(hWnd, NULL);
	
	g_hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, hDll, dwThread);
	
	if (g_hHook == NULL)
	{
		return false;
	}
	g_hWndHooker = hWndNotify;
	result = TRUE;
	//TODO:更改窗口过程
	/*g_WndProcOld = (FunWndProc)::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)::ProcNew);
	if (!g_WndProcOld)
	{
		OutInfo(_T("设置窗口过程错误:"), GetLastError());
	}
	else
	{
		OutInfo(_T("设置窗口过程成功"));
	}*/
	//glhDisplayWnd = hWnd; // 设置显示目标窗口标题编辑框的句柄

	return result;
}

t_MSGNOTE* GetMsg(UINT msgId)
{
	int nCount = _countof(messagesnote);
	for (int i = 0; i < nCount; ++i)
	{
		if (messagesnote[i].dwMsg == msgId)
		{	
			return &messagesnote[i];
		}
	}
	return NULL;
}

LRESULT CALLBACK GetMsgProc(int nCode,
	WPARAM wParam,
	LPARAM lParam
	)
{
	//return CallNextHookEx(g_hHook, nCode, wParam, lParam);


	CWPSTRUCT* pMsg = (CWPSTRUCT*)lParam;
	MSG* msg = (tagMSG*)lParam;
	t_MSGNOTE* msgNote = GetMsg(msg->message);
	if (msgNote)
	{
		tstring strMsg;
		strMsg += _T("msg:");
		strMsg += msgNote->szMsgText;
		if (msgNote->dwMsg != WM_NULL)
		{
			OutInfo(strMsg.c_str());
		}
	}
	
	if (msg->message == WM_SIZE)
	{
		OutputDebugString(_T("get msg: WM_SIZE"));
		//UnhookWindowsHookEx(g_hHook);
		//msg->message = WM_NULL;
		return CallNextHookEx(g_hHook, nCode, wParam, (LPARAM)msg);
	}
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}



LRESULT WINAPI MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	LPMOUSEHOOKSTRUCT pMouseHook = (MOUSEHOOKSTRUCT FAR *) lParam;

	if (nCode >= 0) 
	{

		HWND glhTargetWnd = pMouseHook->hwnd; // 取目标窗口句柄

		HWND ParentWnd = glhTargetWnd;

		/*while (ParentWnd != NULL)
		{

			glhTargetWnd = ParentWnd;

			ParentWnd = GetParent(glhTargetWnd); // 取应用程序主窗口句柄

		}*/

		switch (wParam)
		{
		case WM_LBUTTONUP:
		{
			tstring strTemp;
			strTemp.resize(100);
			::GetClassName(pMouseHook->hwnd, &strTemp.at(0), 100);
			strTemp = strTemp.c_str();
			if (strTemp != _T("Button"))
			{
				break;
			}
			HWND hP = ::GetParent(pMouseHook->hwnd);
			if (!hP)
			{
				break;
			}
			::GetClassName(hP, &strTemp.at(0), 100);
			//strTemp.assign(strTemp.c_str());
			tstring str2 = strTemp.c_str();
			if (str2 != _T("TrayNotifyWnd"))
			{
				break;
			}
			LRESULT lR = CallNextHookEx((HHOOK)g_hHook, nCode, wParam, lParam);
			//Sleep(1000);
			::PostMessage(g_hWndHooker, UM_HOOK_NOTIFY, NULL, NULL);
			return lR;
		}

			break;
		default:
			break;
		}

	}

	// 继续传递消息

	return CallNextHookEx((HHOOK)g_hHook, nCode, wParam, lParam);

}


LRESULT CALLBACK CallWndProc(int nCode,
	WPARAM wParam,
	LPARAM lParam
	)
{
	CWPSTRUCT* pMsg = (CWPSTRUCT*)lParam;
	t_MSGNOTE* msg = GetMsg(pMsg->message);
	tstring strMsg;
	if (msg)
	{
		strMsg += _T("msg:");
		strMsg += msg->szMsgText;
		//OutInfo(strMsg.c_str());
	}

	
		if (pMsg->message != WM_SIZING
			&& pMsg->message != WM_WINDOWPOSCHANGING)
		{
			return CallNextHookEx((HHOOK)g_hHook, nCode, wParam, lParam);
		}
		OutInfo(strMsg.c_str());
		//OutInfo(_T("WM_SIZING"));
		/*if (!(pMsg->hwnd == (HWND)0x100e0)/ *会调整的窗口大小* / )
		{
			return CallNextHookEx((HHOOK)glhHook, nCode, wParam, lParam);
		}
		//*/
		//OutInfo(_T("窗口句柄值2:"), (int)g_hWndHooker);
		if (g_hWndHooker)
		{
			::SendMessage(g_hWndHooker, UM_HOOK_NOTIFY, NULL, NULL);
		}
	
	return CallNextHookEx((HHOOK)g_hHook, nCode, wParam, lParam);
}


LRESULT CALLBACK ProcNew(HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	OutInfo(_T("新的窗口过程里的消息"));
	return g_WndProcOld(hwnd, uMsg, wParam, lParam);
}