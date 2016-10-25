
// UseNXwkeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UseNXwke.h"
#include "UseNXwkeDlg.h"
#include "afxdialogex.h"
#include "..\NXwke\render.h"
#include "..\..\sdk\functional.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "NXwke.lib")

//HWebView g_webView;
#if 0
HWND		g_hWndMain = NULL;
CRender* g_render = NULL;
HWND g_hViewWindow = NULL;
#endif


wkeWebView* g_webView = NULL;
const LPCWSTR wkeWebViewClassName = L"wkeWebView2";

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUseNXwkeDlg 对话框



CUseNXwkeDlg::CUseNXwkeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUseNXwkeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUseNXwkeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUseNXwkeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CUseNXwkeDlg 消息处理程序

LRESULT CALLBACK WebViewDllWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool handled = true;
	switch (message)
	{
		//cexer 增加拖放加载
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
		if (g_webView /*&& g_render*/)
		{
			/*wkeResize(g_webView, LOWORD(lParam), HIWORD(lParam));
			g_render->resize(LOWORD(lParam), HIWORD(lParam));*/
			//NCReSizeBrowser(g_webView, 0, 0, LOWORD(lParam), HIWORD(lParam));
		}
		break;
		
	case WM_KEYDOWN:
	{
		unsigned int virtualKeyCode = wParam;
		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)
			flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)
			flags |= WKE_EXTENDED;

		//flags = HIWORD(lParam);

		//handled = wkeFireKeyDownEvent(g_webView, virtualKeyCode, flags, false);
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

		//handled = wkeFireKeyUpEvent(g_webView, virtualKeyCode, flags, false);
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

		//handled = wkeFireKeyPressEvent(g_webView, charCode, flags, false);
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
		OutInfo(_T("窗口消息:"), message);
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

		//handled = wkeFireMouseEvent(g_webView, message, x, y, flags);
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

		//handled = wkeFireContextMenuEvent(g_webView, pt.x, pt.y, flags);
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

		//handled = wkeFireMouseWheelEvent(g_webView, pt.x, pt.y, delta, flags);
	}
	break;
	
	case WM_SETFOCUS:
		//NCSetFocusBrowser(g_webView);
		break;

	/*case WM_KILLFOCUS:
		wkeKillFocus(g_webView);
		break;
*/


	default:
		handled = false;
		break;
	}

	if (!handled)
		return DefWindowProc(hWnd, message, wParam, lParam);

	return 0;
}

#if 0
LRESULT CALLBACK WebViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool handled = true;
	switch (message)
	{
		//cexer 增加拖放加载
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
		if (g_webView && g_render)
		{
			wkeResize(g_webView, LOWORD(lParam), HIWORD(lParam));
			g_render->resize(LOWORD(lParam), HIWORD(lParam));
		}
		break;

	case WM_KEYDOWN:
	{
		unsigned int virtualKeyCode = wParam;
		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)
			flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)
			flags |= WKE_EXTENDED;

		//flags = HIWORD(lParam);

		handled = wkeFireKeyDownEvent(g_webView, virtualKeyCode, flags, false);
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

		handled = wkeFireKeyUpEvent(g_webView, virtualKeyCode, flags, false);
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

		handled = wkeFireKeyPressEvent(g_webView, charCode, flags, false);
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

		handled = wkeFireMouseEvent(g_webView, message, x, y, flags);
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

		handled = wkeFireContextMenuEvent(g_webView, pt.x, pt.y, flags);
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

		handled = wkeFireMouseWheelEvent(g_webView, pt.x, pt.y, delta, flags);
	}
	break;

	case WM_SETFOCUS:
		wkeSetFocus(g_webView);
		break;

	case WM_KILLFOCUS:
		wkeKillFocus(g_webView);
		break;

	break;

	default:
		handled = false;
		break;
	}

	if (!handled)
		return DefWindowProc(hWnd, message, wParam, lParam);

	return 0;
}


void resizeSubViews()
{
	{
		RECT rcClient;
		GetClientRect(g_hWndMain, &rcClient);
		
		MoveWindow(g_hViewWindow, 0, 30, rcClient.right, rcClient.bottom - 30, TRUE);

		wkeResize(g_webView, rcClient.right, rcClient.bottom - 30);
	}
}
bool registerWebViewWindowClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = WebViewWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = AfxGetApp()->m_hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = 0;
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = wkeWebViewClassName;
	wcex.hIconSm = 0;

	return !!RegisterClassEx(&wcex);
}
#endif


bool registerWebViewDllWindowClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = WebViewDllWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = AfxGetApp()->m_hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = 0;
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = wkeWebViewClassName;
	wcex.hIconSm = 0;

	return !!RegisterClassEx(&wcex);
}


BOOL CUseNXwkeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	
	TestNcWke();

	return FALSE;



#if 0

/*
	HWND hURLBarWnd = CreateWindow(L"EDIT", 0,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOVSCROLL,
		0, 0, 0, 0,
		hMainWnd,
		0,
		hInstance, 0);*/

	g_hWndMain = GetSafeHwnd();
	wkeInitialize();

	wkeSettings settings;
	memset(&settings, 0, sizeof(settings));

	wkeConfigure(&settings);


	//jsBindObject("testObj", js_getObjectProp, js_setObjectProp);
/*
	wkeJSBindFunction("msgBox", js_msgBox, 2);
	wkeJSBindGetter("testCount", js_getTestCount);
	wkeJSBindSetter("testCount", js_setTestCount);*/

	g_webView = wkeCreateWebView();

	wkeSetTransparent(g_webView, false);

	registerWebViewWindowClass();
	g_hViewWindow = CreateWindow(wkeWebViewClassName, 0,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, 0, 0,
		GetSafeHwnd(),
		0,
		AfxGetApp()->m_hInstance, 0);


	
	wkeSetHostWindow(g_webView, g_hViewWindow);

	resizeSubViews();

	g_render = CRender::create(CRender::GDI_RENDER);
	g_render->init(g_hViewWindow);
	/*DefEditProc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hURLBarWnd, GWL_WNDPROC));
	SetWindowLongPtr(hURLBarWnd, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(UrlEditProc));
	SetFocus(hURLBarWnd);*/

	wkeLoadHTMLW(g_webView, L"<p style=\"background-color: #00FF00\">Testing</p><img id=\"webkit logo\" src=\"http://webkit.org/images/icon-gold.png\" alt=\"Face\"><div style=\"border: solid blue; background: white;\" contenteditable=\"true\">div with blue border</div><ul><li>foo<li>bar<li>baz</ul>");


	//NCLoadURL(g_webView, _T("https://www.baidu.com/"));
#endif

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUseNXwkeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUseNXwkeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
		NCDraw(g_webView);
		//g_render->render(g_webView);
	}
	else
	{
		//CDialogEx::OnPaint();
		NCDraw(g_webView);
		//g_render->render(g_webView);

	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUseNXwkeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUseNXwkeDlg::TestNcWke()
{
	/*registerWebViewDllWindowClass();
	HWND hViewWindow = CreateWindow(wkeWebViewClassName, 0,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, 0, 0,
		GetSafeHwnd(),
		0,
		AfxGetApp()->m_hInstance, 0);
	
	RECT rcClient;
	GetClientRect( &rcClient);
	::MoveWindow(hViewWindow, 0, 30, rcClient.right, rcClient.bottom - 30, TRUE);
	g_webView = NCInitBrowser(hViewWindow, NULL);*/

	g_webView = NCInitBrowser(GetSafeHwnd(), NULL);
	NCReSizeBrowser(g_webView, 0, 0, 800, 700);

	NCShowBrowser(g_webView, true);
	NCLoadURL(g_webView, _T("https://www.baidu.com/index.php?tn=mswin_oem_dg"));
	NCSetFocusBrowser(g_webView);
}



void CUseNXwkeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	
	NCReSizeBrowser(g_webView, 0, 0, cx, cy);
}


BOOL CUseNXwkeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN 
		|| pMsg->message == WM_KEYUP)
	{
		OutInfo(_T("main WM_KEYDOWN, WM_KEYUP"));
		return FALSE;

	}

	if (pMsg->message == WM_CHAR)
	{
		//pMsg->hwnd = m_hWnd;
		OutInfo(_T("main WM_CHAR"));
		NCSetFocusBrowser(g_webView);
		return FALSE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CUseNXwkeDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nChar == 0xD)
	{
		
	}
	CString ss;
	ss.Format(L"%c", nChar);
	OutInfo(_T("MainDlg按下："), ss);
	
	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
}
