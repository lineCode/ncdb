#include "stdAfx.h"
#include "PiNCWke.h"
#include "PiString.h"
#include "System/LogSystemDll.h"
#include "PiWindowPack.h"
#include "RAIILock.h"
#include "wke\WkeCommon.h"
#include "StrCoding.h"
#include "PathLight.h"
#include "functional.h"
#include "PiURl.h"

Pi_NameSpace_Using

#define UM_RELOAD					WM_USER + 100
#define UM_DESTROY					WM_USER + 101

#define NUM_TIMER_ID_DRAW_WKE		270			//定时绘制HWebView
#define NUM_TIMERID_RELOAD				30
#define NUM_RELOAD_LIMIT				30
#define NUM_RELOAD_PERIOD				1000

//#define M_DESTROY_NEW			//使用NCDESTROY 消息, 目前外部先析构了父窗口， 再执行Free接口， 先临时在free接口里释放

const LPCWSTR wkeWebViewClassName = L"wkeWebView2";
const int NUM_STRING_ARRAY_SIZE = 1024;

CWkeMng				CPiNCWke::g_wkeMng;
HINSTANCE			CPiNCWke::m_hInstance;

CPiNCWke::CPiNCWke()
	:m_bTryReload(true)
	, m_bDestroying(false)
	, m_web(NULL)
	, m_webDestroy(NULL)
{
}


CPiNCWke::~CPiNCWke()
{
	if (m_web || 
		(m_bDestroying && m_webDestroy) )
	{
	}
}


void CPiNCWke::NCDraw()
{
	wkeSetDirty(m_web, true);
	NCDrawIfNeed();
}


void CPiNCWke::NCDrawIfNeed()
{
	if (!m_pWData || !m_pWData->pRender)
	{
		return;
	}
	CRender* pRender = m_pWData->pRender;
	if (pRender)
	{
		pRender->render(m_web);
	}
	else
	{
		LogSystem::WriteLogToFileErrorFormat(_T("web %d draw if need has null render"), m_web);
	}
}


LRESULT CALLBACK CPiNCWke::WebViewDllWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/************************************************************************
			通过响应的窗口句柄获取到嵌入在它之下的HWebView
			通过HWebView 获取到web对象CPiNCWke
	************************************************************************/
	bool handled = true;
	bool bCustom = false;
	DWORD dwRetCustom = 0;
	HNCwkeWebView pWeb = static_cast<HNCwkeWebView>(g_wkeMng.GetWebView(hWnd));//TODO:获取的方式要更改
	switch (message)
	{
		//cexer 增加拖放加载
	case WM_TIMER:
	{
		//OutInfo(_T("WebView WM_TIMER"));
		int nTimerID = wParam;
		CPiNCWke* pWke = WkeGetObject(pWeb);
		if (!pWke)
		{
			break;
		}
		pWke->DealTimer(nTimerID);
	}
	break;
	case WM_CREATE:
	{
		/*这边设置不起作用， 在创建完后设置可以
		::SetTimer(hWnd, NUM_TIMER_ID_DRAW_WKE, 100, NULL);*/
		::DragAcceptFiles(hWnd, TRUE);
	}
	break;
	case WM_DESTROY:
	{
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d 's wnd destroying"), pWeb);
		::KillTimer(hWnd, NUM_TIMER_ID_DRAW_WKE);
		break;
	}
	#ifdef M_DESTROY_NEW
case WM_NCDESTROY:
	{
		LogSystem::WriteLogToFileMsgFormat(_T("web %d 's wnd ncdestroying"), pWeb);
		CPiNCWke* pWke = WkeGetObject(pWeb);
		if (!pWke)
		{
			break;
		}
		g_wkeMng.Erase(pWke);
	}
		break;
#endif
	case WM_PAINT:
		//LogSystem::WriteLogToFileMsg(_T("WebViewDllWndProc WM_PAINT"));
	{
		CPiNCWke* pWke = WkeGetObject(pWeb);
		if (!pWke)
		{
			break;
		}
		pWke->NCDraw();
		handled = false;		//需要调用默认的处理， 否则可能导致界面没有绘制出来
	}
		break;

	case WM_DROPFILES:
	{
		CPiNCWke* pWke = WkeGetObject(pWeb);
		if (!pWke)
		{
			break;
		}
		HDROP hDrop = reinterpret_cast<HDROP>(wParam);
		tstring strName = CPiWindowPack::GetOneDragFilePath(hDrop, true);
		pWke->NotifyDropFile(strName);		
	}
	break;

	case WM_COMMAND:
		SendMessage(::GetParent(hWnd), message, wParam, lParam);
		break;

	case WM_SIZE:
		//OutInfo(_T("dll Wnd WM_SIZE"));
		break;

	case WM_KEYDOWN:
	{
		unsigned int virtualKeyCode = wParam;
		unsigned int flags = 0;
		//OutInfo(_T("ncBrowser get key down:"), virtualKeyCode);
		if (HIWORD(lParam) & KF_REPEAT)
			flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)
			flags |= WKE_EXTENDED;

		//flags = HIWORD(lParam);
		if (virtualKeyCode == VK_BROWSER_BACK
			|| virtualKeyCode == VK_BROWSER_FORWARD
			|| virtualKeyCode == VK_BACK)
		{
			
			CPiNCWke* pWke = g_wkeMng.GetObj(pWeb);
			if (!pWeb)
			{
				break;
			}
			if (!pWke->IsEnableKeyNavigation())
			{
				break;
			}
		}
		if (pWeb)
		{
			handled = wkeFireKeyDownEvent(pWeb, virtualKeyCode, flags, false);
		}
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
		if (pWeb)
		{
			handled = wkeFireKeyUpEvent(pWeb, virtualKeyCode, flags, false);
		}
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
		if (pWeb)
		{
			handled = wkeFireKeyPressEvent(pWeb, charCode, flags, false);
		}
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
			::SetFocus(hWnd);
			::SetCapture(hWnd);
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
		if (pWeb)
		{
			handled = wkeFireMouseEvent(pWeb, message, x, y, flags);
		}
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
		if (pWeb)
		{
			handled = wkeFireContextMenuEvent(pWeb, pt.x, pt.y, flags);
		}
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
		if (pWeb)
		{
			handled = wkeFireMouseWheelEvent(pWeb, pt.x, pt.y, delta, flags);
		}
	}
	break;

	case WM_SETFOCUS:
		if (pWeb)
		{
			wkeSetFocus(pWeb);
		}
		break;

	case WM_KILLFOCUS:
		if (pWeb)
		{
			wkeKillFocus(pWeb);
		}
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
	case UM_RELOAD:
		bCustom = true;
		NCReLoad((HNCwkeWebView)wParam);
		break;
	case UM_DESTROY:
	{
		bCustom = true;
		CPiNCWke* pWke = WkeGetObject(pWeb);
		if (!pWke)
		{
			break;
		}
		dwRetCustom = pWke->DestroySync();
	}
		break;
	default:
		handled = false;
		break;
	}
	if (message == WM_KEYDOWN
		|| message == WM_KEYUP)
	{
		HWND hParent = ::GetParent(hWnd);
		::PostMessage(hParent, message, wParam, lParam);
	}
	if (bCustom)
	{
		return dwRetCustom; 
	}
	//if (!handled)
	return DefWindowProc(hWnd, message, wParam, lParam);


}

bool CPiNCWke::registerWebViewDllWindowClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = WebViewDllWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = 0;
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = wkeWebViewClassName;
	wcex.hIconSm = 0;

	return !!RegisterClassEx(&wcex);
}

HWND CPiNCWke::CreateHostWnd(HWND hParent)
{
	hParent = hParent ? hParent : GetDesktopWindow();
	registerWebViewDllWindowClass();
	HWND hViewWindow = CreateWindow(wkeWebViewClassName, 0,
		WS_CHILD /*| WS_VISIBLE*/ | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		//WS_OVERLAPPEDWINDOW,
		0, 0, 10, 10,
		hParent,

		0,
		m_hInstance, 0);
	int n = 0;
	if (!hViewWindow)
	{
		LogSystem::WriteLogToFileErrorFormat(_T("create window fail, parent:%d"), hParent);
		n = GetLastError();
	}
	return hViewWindow;
}


void CPiNCWke::onTitleChanged(HNCwkeWebView webView, void* param, const wkeString* title)
{
	//SetWindowTextW(hMainWnd, wkeGetStringW(title));
}

void CPiNCWke::onURLChanged(HNCwkeWebView webView, void* param, const wkeString* url)
{
	//SetWindowTextW(hURLBarWnd, wkeGetStringW(url));
}

bool CPiNCWke::onNavigation(HNCwkeWebView webView, void* param, wkeNavigationType type, const wkeString* url_)
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


HNCwkeWebView CPiNCWke::onCreateView(HNCwkeWebView webView, void* param, const wkeNewViewInfo* info)
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
		HNCwkeWebView newWindow = wkeCreateWebWindow(WKE_WINDOW_TYPE_POPUP, NULL, info->x, info->y, info->width, info->height);
		wkeShowWindow(newWindow, true);
		return newWindow;
	}

	return NULL;
}



bool CPiNCWke::OnwkeNavigationCallback(HNCwkeWebView webView, void* param, wkeNavigationType navigationType, const wkeString* url)
{
	if (!webView || !url)
	{
		LogSystem::WriteLogToFileError(_T("OnwkeNavigationCallback url is null:"));
		return false;
	}

	wstring strUrl(wkeGetStringW(url));
	CPiNCWke* pWke = WkeGetObject(webView);
	if (!pWke)
	{
		return false;
	}
	pWke->NotifyNavigation(strUrl);
	return true;
}

void CPiNCWke::OnwkeAlertBoxCallback(HNCwkeWebView webView, void* param, const wkeString* msg)
{
	if (!webView || !msg)
	{
		return;
	}

	wstring strMsg(wkeGetStringW(msg));

	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d OnwkeAlertBoxCallback: %s"), webView, strMsg.c_str());

	CPiNCWke* pWke = WkeGetObject(webView);
	if (!pWke)
	{
		return;
	}
	pWke->NotifyAlert(strMsg);
	
	return;
}


void CPiNCWke::OnwkeConsoleMessageCallback(HNCwkeWebView webView, void* param, const wkeConsoleMessage* message)
{
	if (!webView || !message || !message->message)
	{
		return;
	}
	wstring strMsg(wkeGetStringW(message->message));

	CPiNCWke* wke = WkeGetObject(webView);
	if (!wke)
	{
		return;
	}
	wke->CheckMsg(strMsg.c_str());
	wke->NotifyConsoleMsg(strMsg);
}


void CPiNCWke::OnwkeLoadingFinishCallback(HNCwkeWebView webView, void* param, const wkeString* url, wkeLoadingResult result, const wkeString* failedReason)
{
	CPiNCWke* pWke = WkeGetObject(webView);
	if (!pWke || !url)
	{
		return;
	}
	wstring strUrlTemp = wkeGetStringW(url);
	CPiURl piUrl(strUrlTemp);
	strUrlTemp = piUrl.DecodeParam();

	if (pWke->GetUrl() != strUrlTemp)	//url中文会url加密
	{
		LogSystem::WriteLogToFileErrorFormat(_T("web %d loadEnd other web :%s"), webView, strUrlTemp.c_str());
		return;
	}
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d loadEnd, code:%d"), webView, result);
	pWke->SetStatus(result, failedReason);

	if (pWke->IsCancel())
	{
		LogSystem::WriteLogToFileErrorFormat(_T("web %d had cancel in url<%s>, code:%d"), webView, pWke->GetUrl().c_str(), result);
	}
	else if (pWke->IsLoadError()
		&& pWke->CanReLoad())
	{
		pWke->NotifyLoadError();
	}
	else if (pWke->HasConsoleError())
	{
		pWke->NotifyConsoleError();
	}
	else
	{
		pWke->LoadOk();
		pWke->NotifyLoadEnd();
	}

	pWke->LoadEnd();
	return;
}


bool CPiNCWke::Create(HWND hParent, tagCallBack* pTagCallBack)
{
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("init browser begin"));
	m_pWData = new tagWKE_DATA;

	HWND hHost = CreateHostWnd(hParent);
	m_pWData->hParent = hHost;
	if (!hHost)
	{
		LogSystem::WriteLogToFileError(_T("init browser, CreateHostWnd ret null"));
		return false;
	}
	::SetTimer(hHost, NUM_TIMER_ID_DRAW_WKE, 100, NULL);		//WebView需要定时绘制

#ifdef M_TEST_DUMP
	{
		char* pMem = new char[1024 * 1024 * 1024];
		pMem = new char[1024 * 1024 * 1024];
		pMem = new char[1024 * 1024 * 1024];
		pMem = new char[1024 * 1024 * 1024];
		memset(pMem, 0, 1024 * 1024 * 1024 + 5);
		*pMem = 3;
		throw 3;
	}
#endif


	wkeSettings settings;
	memset(&settings, 0, sizeof(settings));
	wkeConfigure(&settings);

	m_webDestroy = m_web = wkeCreateWebView();
	HNCwkeWebView hWebView = m_web;
	if (!hWebView)
	{
		LogSystem::WriteLogToFileError(_T("init browser, wkeCreateWebView ret null"));
		return false;
	}
	wkeSetTransparent(hWebView, false);

	wkeOnTitleChanged(hWebView, &CPiNCWke::onTitleChanged, NULL);
	wkeOnURLChanged(hWebView,	&CPiNCWke::onURLChanged, NULL);
	//wkeOnNavigation(hWebView, &CPiNCWke::onNavigation, NULL);
	wkeOnCreateView(hWebView,	&CPiNCWke::onCreateView, NULL);

	wkeSetUserAgent(hWebView, "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36");

	wkeSetHostWindow(hWebView, hHost);
	LogSystem::WriteLogToFileMsg(LogSystem::LOG_LIGHT, _T("init browser, create done"));

	tagWKE_DATA* WD = m_pWData;
	WD->pWeb = hWebView;
	WD->hParent = hHost;
	if (pTagCallBack)
	{
		WD->tagCB = *pTagCallBack;
	}
	//wkeJSBindFunction("LvClientWebFunc", JS_CallBack_P2, 2);

	//NCBindJsFunction(_T("LvClientWebFunc"), WD->tagCB.pOnWebFunc);

	wkeOnNavigation(hWebView, OnwkeNavigationCallback, nullptr);
	wkeOnConsoleMessage(hWebView, OnwkeConsoleMessageCallback, nullptr);
	if (WD->tagCB.pOnAlertEvent)
	{
		wkeOnAlertBox(hWebView, OnwkeAlertBoxCallback, nullptr);
	}
	wkeOnLoadingFinish(hWebView, OnwkeLoadingFinishCallback, nullptr);
	LogSystem::WriteLogToFileMsg(LogSystem::LOG_LIGHT, _T("init browser, setCallBack"));

	WD->pRender = CRender::create(CRender::D3D_RENDER);
	WD->pLock = new CLock;
	if (!WD->pRender)
	{
		LogSystem::WriteLogToFileError(_T("init browser, CRender::create fail!"));
		return false;
	}
	if (!WD->pRender->init(hHost))
	{
		LogSystem::WriteLogToFileError(_T("init browser, Render init fail! retry"));
		if (!WD->pRender->init(hHost))
		{
			LogSystem::WriteLogToFileError(_T("init browser, Render retry init fail!"));
			return false;
		}
	}
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d init browser end ok"), hWebView);
	return true;
}

bool CPiNCWke::LoadURL(tcpchar szUrl)
{
	if (!szUrl || !*szUrl)
	{
		LogSystem::WriteLogToFileErrorFormat(_T("web %d NCLoadURL url is null"), m_web, szUrl);
		return false;
	}

	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_NORMAL, _T("web:%d, load url: %s"), m_web, szUrl);
	
	m_pWData->url = CStrCoding::UrlDecode(szUrl);
	m_mapErrorConsole.clear();	//清空上个页面加载完成后的控制台信息，以免影响到本次

	wkeStopLoading(m_web);
	wkeLoadURLW(m_web, szUrl);
	wkeSetFocus(m_web);
	return true;
}


bool CPiNCWke::LoadFile(tcpchar szPath)
{
	if (!CPathLight::IsFileExist(szPath))
	{
		LogSystem::WriteLogToFileErrorFormat(_T("web loadLocal file is no exist:%s"), m_web, szPath);
		return false;
	}

	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web:%d, load file: %s"), m_web, szPath);
	CPiString strUrl(_T("file:///"));
	strUrl += szPath;
	strUrl.Replace(_T("\\"), _T("/"));//路径在回调时返回的是坐斜杠， 保证数据一致性
	m_pWData->url = strUrl;
	wkeLoadURLW(m_web, strUrl.c_str());
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web loadLocal %d wkeLoadURLW ok "), m_web);
	return true;
}

bool CPiNCWke::Show(bool bShow)
{
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d, show:%d"), m_web, bShow);
	::ShowWindow(m_pWData->hParent, bShow ? SW_SHOW : SW_HIDE);

	return true;
}

bool CPiNCWke::SetFocus()
{
	::SetFocus(m_pWData->hParent);
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d set focus"), m_web);
	wkeSetFocus(m_web);
	return true;
}

void CPiNCWke::Move(int x, int y)
{
	throw std::logic_error("The method or operation is not implemented.");
	return ;
}

void CPiNCWke::Resize(int cx, int cy)
{

}

void CPiNCWke::ChangeSize(int x, int y, int width, int height)
{
	if (!m_pWData)
	{
		return;
	}
	//TODO:g_wkeMng.GetStruct返回的数据也要做线程处理， 应对外部多线程调用释放接口
	RECT rt = { 0 };
	rt = CPiWindowPack::GetClientRectToParent(m_pWData->hParent);

	x = (x == -1 ? rt.left : x);
	y = (y == -1 ? rt.top : y);
	width = (width == -1 ? rt.right - rt.left : width);
	height = (height == -1 ? rt.bottom - rt.top : height);

	if (::MoveWindow(m_pWData->hParent, x, y, width, height, TRUE))
	{
		wkeResize(m_web, width, height);
		m_pWData->pRender->resize(width, height);
	}
}

bool CPiNCWke::Destroy()
{
	//子窗口和父窗口在同一个线程， 直接销毁
	return DestroySync();


	/*LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_NORMAL, _T("web %d will Free browser"), m_web);
	//通知销毁， 等待销毁完成
	::PostMessage(m_pWData->hParent, UM_DESTROY, 0, 0);	//如果正在绘制， 是否会等待绘制完成
	//TODO:等待销毁完成
*/

	return true;
}

void CPiNCWke::CheckMsg(const wchar_t * szMsg)
{
	vector<tstring> lstErr;
	lstErr.push_back(_T("TypeError"));
	lstErr.push_back(_T("SyntaxError"));
	CPiString strMsg(szMsg);
	for (auto iter : lstErr)
	{
		if (strMsg.Exist(iter.c_str()))
		{
			m_mapErrorConsole.insert(pair<tstring, tstring>(iter, szMsg));
		}
	}
	
}

bool CPiNCWke::HasConsoleError()
{
	return m_mapErrorConsole.size() > 0;
}

bool CPiNCWke::NotifyConsoleError()
{
	LogSystem::WriteLogToFileErrorFormat(_T("web %d occur some web Console err when load"), m_web);
	/************************************************************************
			当检测到有控制台网页加载错误信息， post消息来重新加载页面，进行重试， 不能在任何回调里同步调用Reload 会崩溃
	************************************************************************/
	PostReload();
	//NotifyError();

	return true;
}

bool CPiNCWke::IsLoadError()
{
	bool bOk = true;
	
	if (m_result == WKE_LOADING_FAILED)
	{
		bOk = false;
		wstring strMsg;
		if (m_failedReason)
		{
			strMsg = wkeGetStringW(m_failedReason);
		}
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d load fail, code: %d, reason: %s"), m_web, m_result, strMsg.c_str());
	}

	//bool bOk = wkeIsLoadingSucceeded(m_web);	//有时成功但wkeIsLoadingSucceeded返回false， wke bug?
	bool bFail = wkeIsLoadingFailed(m_web);
	bool bError = (!bOk || bFail);
	if (bError)
	{
		return true;
	}
	return false;
}

void CPiNCWke::NotifyLoadError()
{
	LogSystem::WriteLogToFileErrorFormat(_T("web %d occur some web err when load"), m_web);
	//PostReload();
	//NotifyError();
}


bool CPiNCWke::CanReLoad()
{
	return m_bTryReload && m_nReloadTimes < NUM_RELOAD_LIMIT;
}


bool CPiNCWke::NotifyLoadEnd()
{
	FunOnLoadEndEventFunc pCB = m_pWData->tagCB.pOnLoadEndEvent;
	if (pCB)
	{
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d LoadEnd cb bg"), m_web);
		(*pCB)(m_pWData->pWeb);
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d LoadEnd cb end"), m_web);
	}
	return true;
}

CPiNCWke::operator HWebView()
{
	return m_web;
}

FunOnDragFiles CPiNCWke::GetDropFilesFun()
{
	return m_pWData ? m_pWData->tagCB.pOnDragFiles : nullptr;
}

bool CPiNCWke::ExecScript(tcpchar szScript, tstring& strReturnString)
{
	strReturnString.clear();
	if (!szScript || !*szScript)
	{
		return false;
	}
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d will exeScript: %s"), m_web, szScript);

	//有该接口并且调用成功则视为执行成功
	CRAIILock raii(m_pWData->pLock);
	/************************************************************************
			区分执行失败和执行成功没返回值的情况
	************************************************************************/
	wkeJSValue vVal = wkeRunJSW(m_web, szScript);
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d had exec Script"), m_web);
	if (CWkeCommon::IsJSUnderined(m_web, vVal))
	{
		LogSystem::WriteLogToFileErrorFormat(_T("web %d, exeScript <%s> failed!"), m_web, szScript);
		return false;
	}
	else
	{
		strReturnString = CWkeCommon::GetString(m_web, vVal);
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d exeScript <%s> ok"), m_web, szScript);
	}

	return true;
}

bool CPiNCWke::EnableDrag(bool bEnabel)
{
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d set is allow draw file:%d"), m_web, bEnabel);
	m_pWData->bEnabelDrag = bEnabel;
	::DragAcceptFiles(m_pWData->hParent, bEnabel ? TRUE : FALSE);
	return true;
}

bool CPiNCWke::EnableKeyNavigate(bool bEnabel)
{
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d set will allow KeyNavigate:%d"), m_web, bEnabel);
	m_pWData->bEnabelKeyNavigation = bEnabel;
	::DragAcceptFiles(m_pWData->hParent, bEnabel ? TRUE : FALSE);
	return true;
}

bool CPiNCWke::NotifyConsoleMsg(const tstring& strMsg)
{
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_NORMAL, _T("web %d OnwkeConsoleMessageCallback, %s"), m_web, strMsg.c_str());
	FunOnConsoleMsgEventFunc pCB = m_pWData->tagCB.pOnConsoleMsgEvent;
	if (pCB)
	{
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d ConsoleMsg cb bg"), m_web);
		(*pCB)(m_pWData->pWeb, strMsg.c_str());
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d ConsoleMsg cb end"), m_web);
	}
	return true;
}

bool CPiNCWke::ReLoad()
{	
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d will reload"), m_web);
	
	wkeStopLoading(m_web);
	wkeReload((HNCwkeWebView)m_web);
	return true;
}

wkeJSValue JS_CALL CPiNCWke::JS_CallBack_P2(wkeJSState* es)
{
	tstring strP1(wkeJSToTempStringW(es, wkeJSParam(es, 0)));
	tstring strP2(wkeJSToTempStringW(es, wkeJSParam(es, 1)));

	/************************************************************************
	目前未实现绑定多个对应的接口， 只获取第一个绑定的函数
	回调时并不知道网页接口和外部设置的对应关系， 如果有回调头ID就可以识别
	************************************************************************/
	FunOnClientWebFunc pFun = (FunOnClientWebFunc)g_wkeMng.GetJsFirstBind();
	wstring str(1024, 0);
	bool bRet = true;
	if (pFun)
	{
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("webFun cb bg"));
		bRet = (*pFun)(strP1.c_str(), strP2.c_str(), &str.at(0));
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("webFun cb end"));
		
		if (!bRet)
		{
			LogSystem::WriteLogToFileError(_T("外层回调WebFun执行失败"));
		}
	}
	else
	{
		return wkeJSUndefined(es);
	}

	//服务器没有提示文本
	return wkeJSStringW(es, str.c_str());
	//string sName = __FUNCDNAME__;
	//g_wkeMng.GetJS()
}

bool CPiNCWke::BindJsFunction(tcpchar szFunc, FunOnClientWebFunc pCallBack)
{
	if (!szFunc || *szFunc == 0
		|| !pCallBack)
	{
		return false;
	}
	wkeJSBindFunction(pw2psSmart(szFunc), JS_CallBack_P2, 2);
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("bind js src %s,  dist: %d"), szFunc, pCallBack);

	if (szFunc && pCallBack)
	{
		g_wkeMng.AddJsBind(szFunc, pCallBack);
		return true;
	}
	return false;
}

bool CPiNCWke::NotifyNavigation(tstring& strUrl)
{
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d navigation to url:%s"), m_web, strUrl.c_str());
	tagWKE_DATA* pWData = m_pWData;
	if (!pWData)
	{
		return false;
	}
	FunOnRequestEventFunc pCB = pWData->tagCB.pOnRequestEvent;
	if (pCB)
	{
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d Navigation cb bg"), m_web);
		(*pCB)(pWData->pWeb, strUrl.c_str());
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d Navigation cb end"), m_web);
	}
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d Navigation end"), m_web);
	return true;

}

HWND CPiNCWke::GetParent()
{
	return m_pWData->hParent;
}

bool CPiNCWke::IsEnableKeyNavigation()
{
	return m_pWData->bEnabelKeyNavigation;
}

CPiNCWke* CPiNCWke::WkeGetObject(HNCwkeWebView pWeb)
{
	return g_wkeMng.GetObj(pWeb);
}

void CPiNCWke::NotifyAlert(const wstring& strMsg)
{
	FunOnAlertEventFunc pCB = m_pWData->tagCB.pOnAlertEvent;
	if (pCB)
	{
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d Alert cb bg"), m_web);
		(*pCB)(m_pWData->pWeb, strMsg.c_str());
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d Alert cb end"), m_web);
	}
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d OnwkeAlertBoxCallback ok"), m_web);
}

void CPiNCWke::SetInst(HINSTANCE hModule)
{
	m_hInstance = hModule;
}

void CPiNCWke::LoadOk()
{
	m_nReloadTimes = 0;
	m_mapErrorConsole.clear();
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d loadEnd ok"), m_web);
}

void CPiNCWke::SetLoadError(FunOnLoadError pFun)
{
	m_pWData->pOnLoadError = pFun;
}

void CPiNCWke::NotifyError()
{
	/*if (m_pWData->pOnLoadError)
	{
		m_pWData->pOnLoadError(m_web);
	}*/

	//PostReload();
}

void CPiNCWke::LoadEnd()
{
	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_NORMAL, _T("web %d loadEnd end"), m_web);
	m_mapErrorConsole.clear();
}

void CPiNCWke::SetStatus(wkeLoadingResult result, const wkeString* failedReason)
{
	m_result = result;
	m_failedReason = failedReason;
}

bool CPiNCWke::IsCancel()
{
	return m_result == WKE_LOADING_CANCELED;
}

tstring CPiNCWke::GetUrl()
{
	return m_pWData->url;
}


bool CPiNCWke::PostReload()
{
	if ( !CanReLoad() )
	{
		return false;
	}
	m_nReloadTimes++;

	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d reload timer, times:%d"), m_web, m_nReloadTimes);
	::SetTimer(m_pWData->hParent, NUM_TIMERID_RELOAD, NUM_RELOAD_PERIOD, NULL);
	return true;
}

bool CPiNCWke::DealTimer(int nTimerID)
{
	
	switch (nTimerID)
	{
	case NUM_TIMER_ID_DRAW_WKE:
		{
			if (m_bDestroying)
			{
				break;
			}
			NCDrawIfNeed();
		}
		break;
	case NUM_TIMERID_RELOAD:
		::KillTimer(m_pWData->hParent, NUM_TIMERID_RELOAD);
		if (m_bDestroying)
		{
			break;
		}
		ReLoad();
		break;
	default:
		break;
	}
	return true;
}

void CPiNCWke::ClearData()
{
	m_web = nullptr;
	if (m_pWData)
	{
		ClearMem(m_pWData->pLock);
	}
	ClearMem(m_pWData);
}

HNCwkeWebView CPiNCWke::GetWeb()
{
	return m_web ? m_web : m_webDestroy;
}

bool CPiNCWke::NotifyDropFile(StringCRef strName)
{
	if (strName.empty())
	{
		return false;
	}
	FunOnDragFiles pFun = GetDropFilesFun();
	if (pFun)
	{
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d DropFile cb bg"), m_web);
		(*pFun)(m_web, strName.c_str());
		LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d DropFile cb end"), m_web);
	}
	return true;
}

bool CPiNCWke::DestroySync()
{
	/************************************************************************
		从安全性来讲应该在窗口销毁后(WM_NCDESTROY)再释放自身数据。 但目前由于外部delphi先释放了wke的父窗口
		再调用销毁接口， 于是不能依赖改销毁下次， 暂时直接释放。	
	************************************************************************/
	

	//释放流程:取消定时器， 删除绘制， 销毁wke， 销毁父窗口, 

	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d Freeing"), m_web);
	m_bDestroying = true;
	::KillTimer(m_pWData->hParent, NUM_TIMER_ID_DRAW_WKE);

	if (!m_pWData)
	{
		LogSystem::WriteLogToFileErrorFormat(_T("web %d Free fail"), m_web);
		return false;
	}

	CRAIILock raii(m_pWData->pLock);		//防止执行脚本跟释放接口多线程调用

	//销毁父窗口时， 父窗口会收到一些focus, 销毁等消息， 完成对wke的最后释放前交接工作， 之后释放wke
	::DestroyWindow(m_pWData->hParent);		
	
	if (m_pWData->pRender)
	{
		m_pWData->pRender->destroy();
		m_pWData->pRender = nullptr;
	}
	wkeDestroyWebView(m_web);
	m_web = nullptr;

	HNCwkeWebView hWeb = m_webDestroy;
	raii.reset();

#ifndef M_DESTROY_NEW
	g_wkeMng.Erase(this);
#endif // M_DESTROY_NEW

	LogSystem::WriteLogToFileMsgFormat(LogSystem::LOG_LIGHT, _T("web %d has Free"), hWeb);
	return true;
}
