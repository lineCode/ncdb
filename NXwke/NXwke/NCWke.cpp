#include "NCWke.h"
#include "WkeRef.h"
#include "render.h"
#include "PiHeadKernel.h"
#include "WkeMng.h"

#pragma comment(lib, "Imm32.lib")


const LPCWSTR wkeWebViewClassName = L"wkeWebView2";

CWkeMng g_wkeMng;
CWkeRef* g_wkeRef = nullptr;
CRender* g_render = NULL;
HHOOK  g_hHook = NULL;
HINSTANCE g_hModule = NULL;
LRESULT CALLBACK ProcWnd(int nCode,
	WPARAM wParam,
	LPARAM lParam
	);

extern "C"  BOOL __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID /*lpReserved*/)
{
	BOOL ret = FALSE;
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		//TODO:加载wke.dll模块， 初始化相关类对象
		/*g_wkeRef = new CWkeRef;
		g_wkeRef->Init();*/
		//WebCore::setInstanceHandle(hModule);
		g_hModule = hModule;
		ret = TRUE;
		break;

	case DLL_PROCESS_DETACH:
		/*WebCore::RenderThemeWin::setWebKitIsBeingUnloaded();
		pthread_win32_thread_detach_np();*/
		//delete g_wkeRef;
		break;

	case DLL_THREAD_ATTACH:
		//TODO:暂时没提供
		/*ExceptionHandler::SetThreadExceptionHandlers();
		pthread_win32_thread_attach_np();*/
		break;

	case DLL_THREAD_DETACH:
		//pthread_win32_thread_detach_np();
		break;
	}

	//CoreFoundationDllMain(hModule, ul_reason_for_call, 0);
	return ret;
}


void onTitleChanged(wkeWebView* webView, void* param, const wkeString* title)
{
	//SetWindowTextW(hMainWnd, wkeGetStringW(title));
	int* p = new int;
}

void onURLChanged(wkeWebView* webView, void* param, const wkeString* url)
{
	int* p = new int;
	//SetWindowTextW(hURLBarWnd, wkeGetStringW(url));
}

bool onNavigation(wkeWebView* webView, void* param, wkeNavigationType type, const wkeString* url_)
{
	const wchar_t* url = wkeGetStringW(url_);
	//if (wcsstr(url, L"baidu.com") != NULL)
	//    return false;

	if (wcsstr(url, L"exec://") == url)
	{
		PROCESS_INFORMATION processInfo = { 0 };
		STARTUPINFOW startupInfo = { 0 };
		startupInfo.cb = sizeof(startupInfo);
		BOOL succeeded = CreateProcessW(NULL, (LPWSTR)url + 7, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
		if (succeeded)
		{
			CloseHandle(processInfo.hProcess);
			CloseHandle(processInfo.hThread);
		}
		return false;
	}

	return true;
}

wkeWebView* onCreateView(wkeWebView* webView, void* param, const wkeNewViewInfo* info)
{
	const wchar_t* target = wkeGetStringW(info->target);
	const wchar_t* url = wkeGetStringW(info->url);

	if (wcscmp(target, L"") == 0 || wcscmp(target, L"_blank") == 0)
	{
		/*if (wcsnicmp(url, L"file:///", 8) == 0)
			url += 8;*/
		ShellExecuteW(NULL, L"open", (LPWSTR)url, NULL, NULL, SW_SHOW);
		return NULL;
	}
	else if (wcscmp(target, L"_self") == 0)
	{
		return webView;
	}
	else if (wcscmp(target, L"wontOpen") == 0)
	{
		return NULL;
	}
	else
	{
		wkeWebView* newWindow = wkeCreateWebWindow(WKE_WINDOW_TYPE_POPUP, NULL, info->x, info->y, info->width, info->height);
		wkeShowWindow(newWindow, SW_SHOW);
		return newWindow;
	}

	return NULL;
}

/*
bool HookParentMsg(HWND hParent)
{
	DWORD dwThread = GetWindowThreadProcessId(hParent, NULL);
	g_hHook = SetWindowsHookEx(WH_CALLWNDPROC, ProcWnd, NULL, dwThread);
	if (!g_hHook)
	{
		return false;
	}
	return true;
}*/


LRESULT CALLBACK WebViewDllWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool handled = true;
	wkeWebView* pWeb = g_wkeMng.GetWebView(hWnd);
	switch (message)
	{
		//cexer 增加拖放加载
	case WM_TIMER:
		g_render->render(pWeb);
		break;
	case WM_CREATE:
	{
		DragAcceptFiles(hWnd, TRUE);
	}
	return 0;

	case WM_DROPFILES:
	{
		
	}
	return 0;
	case WM_COMMAND:
		SendMessage(GetParent(hWnd), message, wParam, lParam);
		return 0;
	case WM_SIZE:
		if (pWeb)
		{
			wkeResize(pWeb, LOWORD(lParam), HIWORD(lParam));
			g_render->resize(LOWORD(lParam), HIWORD(lParam));
		}
		break;
	/*case WM_PAINT:
		NCDraw(pWeb);
		break;*/
	case WM_KEYDOWN:
	{
		unsigned int virtualKeyCode = wParam;
		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)
			flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)
			flags |= WKE_EXTENDED;

		//flags = HIWORD(lParam);

		handled = wkeFireKeyDownEvent(pWeb, virtualKeyCode, flags, false);
	}
	break;
	
	case WM_KEYUP:
	{
		unsigned int virtualKeyCode = wParam;
		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)
			flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)
			flags |= WKE_EXTENDED;

		//flags = HIWORD(lParam);

		handled = wkeFireKeyUpEvent(pWeb, virtualKeyCode, flags, false);
	}
	break;

	case WM_CHAR:
	{
		unsigned int charCode = wParam;
		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)
			flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)
			flags |= WKE_EXTENDED;

		//flags = HIWORD(lParam);

		handled = wkeFireKeyPressEvent(pWeb, charCode, flags, false);
	}
	break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:

	case WM_MOUSEMOVE:
	{
		if (message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_RBUTTONDOWN)
		{
			SetFocus(hWnd);
			SetCapture(hWnd);
		}
		else if (message == WM_LBUTTONUP || message == WM_MBUTTONUP || message == WM_RBUTTONUP)
		{
			ReleaseCapture();
		}

		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		unsigned int flags = 0;

		if (wParam & MK_CONTROL)
			flags |= WKE_CONTROL;
		if (wParam & MK_SHIFT)
			flags |= WKE_SHIFT;

		if (wParam & MK_LBUTTON)
			flags |= WKE_LBUTTON;
		if (wParam & MK_MBUTTON)
			flags |= WKE_MBUTTON;
		if (wParam & MK_RBUTTON)
			flags |= WKE_RBUTTON;

		//flags = wParam;

		handled = wkeFireMouseEvent(pWeb, message, x, y, flags);
	}
	break;

	case WM_CONTEXTMENU:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (pt.x != -1 && pt.y != -1)
			ScreenToClient(hWnd, &pt);

		unsigned int flags = 0;

		if (wParam & MK_CONTROL)
			flags |= WKE_CONTROL;
		if (wParam & MK_SHIFT)
			flags |= WKE_SHIFT;

		if (wParam & MK_LBUTTON)
			flags |= WKE_LBUTTON;
		if (wParam & MK_MBUTTON)
			flags |= WKE_MBUTTON;
		if (wParam & MK_RBUTTON)
			flags |= WKE_RBUTTON;

		handled = wkeFireContextMenuEvent(pWeb, pt.x, pt.y, flags);
	}
	break;

	case WM_MOUSEWHEEL:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		ScreenToClient(hWnd, &pt);

		int delta = GET_WHEEL_DELTA_WPARAM(wParam);

		unsigned int flags = 0;

		if (wParam & MK_CONTROL)
			flags |= WKE_CONTROL;
		if (wParam & MK_SHIFT)
			flags |= WKE_SHIFT;

		if (wParam & MK_LBUTTON)
			flags |= WKE_LBUTTON;
		if (wParam & MK_MBUTTON)
			flags |= WKE_MBUTTON;
		if (wParam & MK_RBUTTON)
			flags |= WKE_RBUTTON;

		//flags = wParam;

		handled = wkeFireMouseWheelEvent(pWeb, pt.x, pt.y, delta, flags);
	}
	break;

	case WM_SETFOCUS:
		wkeSetFocus(pWeb);
		break;

	case WM_KILLFOCUS:
		wkeKillFocus(pWeb);
		break;

	case WM_IME_STARTCOMPOSITION:
	{
		wkeRect caret = wkeGetCaretRect(pWeb);

		CANDIDATEFORM form;
		form.dwIndex = 0;
		form.dwStyle = CFS_EXCLUDE;
		form.ptCurrentPos.x = caret.x;
		form.ptCurrentPos.y = caret.y + caret.h;
		form.rcArea.top = caret.y;
		form.rcArea.bottom = caret.y + caret.h;
		form.rcArea.left = caret.x;
		form.rcArea.right = caret.x + caret.w;

		HIMC hIMC = ImmGetContext(hWnd);
		ImmSetCandidateWindow(hIMC, &form);
		ImmReleaseContext(hWnd, hIMC);
	}
	break;

	default:
		handled = false;
		break;
	}

	if (!handled)
		return DefWindowProc(hWnd, message, wParam, lParam);

	return 0;
}

bool registerWebViewDllWindowClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = WebViewDllWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_hModule;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = 0;
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = wkeWebViewClassName;
	wcex.hIconSm = 0;

	return !!RegisterClassEx(&wcex);
}

HWND CreateHostWnd(HWND hParent)
{
	registerWebViewDllWindowClass();
	HWND hViewWindow = CreateWindow(wkeWebViewClassName, 0,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		//WS_OVERLAPPEDWINDOW,
		0, 0, 0, 0,
		hParent,

		0,
		g_hModule, 0);

	
	return hViewWindow;
}

NCWKE_API HWebView NCWKE_CALL NCInitBrowser(HWND hParent, tagCallBack* pTagCallBack)
{
	//TODO:只在第一次调用
	//g_wkeRef->RwkeInitialize();
	//HookParentMsg(hParent);
	HWND hHost = CreateHostWnd(hParent);
	RECT rcClient;
	::GetClientRect(hParent, &rcClient);
	::MoveWindow(hHost, 0, 0, rcClient.right - 30, rcClient.bottom - 30, TRUE);

	wkeInitialize();

	wkeSettings settings;
	memset(&settings, 0, sizeof(settings));
	wkeConfigure(&settings);

	//wkeWebView* hwebView = g_wkeRef->RwkeCreateWebView();
	wkeWebView* hWebView = wkeCreateWebView();
	wkeSetTransparent(hWebView, false);

	wkeOnTitleChanged(hWebView, onTitleChanged, NULL);
	wkeOnURLChanged(hWebView, onURLChanged, NULL);
	wkeOnNavigation(hWebView, onNavigation, NULL);
	wkeOnCreateView(hWebView, onCreateView, NULL);

	wkeSetUserAgent(hWebView, "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36");

	wkeSetHostWindow(hWebView, hHost);

	::SetFocus(hHost);
	::SetTimer(hHost, 260, 10, NULL);
	
	g_wkeMng[hHost]->pWeb = hWebView;
	g_wkeMng[hHost]->hParent = hHost;

	if (pTagCallBack)
	{
		wkeOnNavigation(hWebView, pTagCallBack->pOnRequestEvent, nullptr);
		wkeOnConsoleMessage(hWebView, pTagCallBack->pOnConsoleMsgEvent, nullptr);
		wkeOnAlertBox(hWebView, pTagCallBack->pOnAlertEvent, nullptr);
		wkeOnLoadingFinish(hWebView, pTagCallBack->pOnLoadEndEvent, nullptr);
	}
	

	//TODO:自己绘制的方式
	g_render = CRender::create(CRender::GDI_RENDER);
	g_render->init(hHost);

	wkeLoadHTMLW(hWebView, L"<p style=\"background-color: #00FF00\">Testing</p><img id=\"webkit logo\" src=\"http://webkit.org/images/icon-gold.png\" alt=\"Face\"><div style=\"border: solid blue; background: white;\" contenteditable=\"true\">div with blue border</div><ul><li>foo<li>bar<li>baz</ul>");
	return hWebView;
}

NCWKE_API void NCWKE_CALL NCFreeBrowser(HWebView hBrowser)
{
	wkeDestroyWebView(hBrowser);

	//TODO:如果所有web都卸载完成则进行卸载
	wkeFinalize();
	
}

NCWKE_API void NCWKE_CALL NCReSizeBrowser(HWebView hBrowser, int x, int y, int width, int height)
{
	//TODO:需要有move功能？？
	//move();
	if (!hBrowser)
	{
		return;
	}
	//TODO:通过web找到结构体
	tagWKE_DATA* pWD = g_wkeMng.GetStruct(hBrowser);
	if (!pWD)
	{
		return;
	}
	::MoveWindow(pWD->hParent, 0, 0, width, height, TRUE);
	wkeResize(hBrowser, width, height);
	
	//wkeResize(g_webView, LOWORD(lParam), HIWORD(lParam));
	g_render->resize(width, height);
}

NCWKE_API void NCWKE_CALL NCShowBrowser(HWebView hBrowser, bool bShow)
{
	wkeShowWindow(hBrowser, bShow);
}

NCWKE_API void NCWKE_CALL NCSetFocusBrowser(HWebView hBrowser)
{
	wkeSetFocus(hBrowser);
}

NCWKE_API void NCWKE_CALL NCLoadURL(HWebView hBrowser, const wchar_t* szUrl)
{
	wkeLoadURLW(hBrowser, szUrl);
	wkeSetFocus(hBrowser);
}

NCWKE_API bool NCWKE_CALL NCExecScript(HWebView hBrowser, const wchar_t* szScript, wchar_t* szReturnString/*1024个字符*/)
{
	wkeJSValue vVal = wkeRunJSW(hBrowser, szScript);
	if (vVal)
	{

	}
	//TODO:返回值
	return true;

}

void NCDraw(HWebView hBrowser)
{
	if (!hBrowser)
	{
		return;
	}
	g_render->render(hBrowser);
}
