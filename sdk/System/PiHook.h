#pragma once
#include <windows.h>
#ifdef DLLHOOK_EXPORTS
#define DLL_DESC _declspec(dllexport)
#else
#define DLL_DESC _declspec(dllimport)
#pragma comment(lib, "DllHook.lib")
#endif

#define UM_HOOK_NOTIFY WM_USER + 400

class DLL_DESC CPiHook
{
public:
	CPiHook();
	~CPiHook();
public:
	bool StartHook(HWND hWnd);
	bool StopHook();
	bool StartWndProcHook(HWND hWnd, HMODULE hDll);
	bool StartGetMessageHook(HWND hWndNotify, HMODULE hDll);
private:
	HMODULE m_hDll;
};

typedef LRESULT (*FunWndProc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern "C" __declspec(dllexport) LRESULT WINAPI MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
extern "C" __declspec(dllexport) LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
extern "C" __declspec(dllexport) LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcNew(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);