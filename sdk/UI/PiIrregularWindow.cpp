#include "StdAfx.h"
#include "PiIrregularWindow.h"
#include "ResCommon.h"

static const TCHAR* __className  =  _T("PI_IrregularWindowClass");
static const TCHAR* __windowName =  _T("PI_IrregularWindow");
typedef map<HWND, CPiIrregularWindow*> MAP_OBJECT;
MAP_OBJECT g_mapObject;
CPiIrregularWindow::CPiIrregularWindow(void)
{
	m_hdcMem = NULL;
	m_hWndDist	= NULL;
	m_hdcMem 	= NULL;;
	m_bmMem	= NULL;
	m_sizeWindow.cx = m_sizeWindow.cy = 0;
	m_bMouseEnter = false;

	m_clNormal = Color(255, 255, 255, 255);
	m_clEnter = m_clNormal;
	m_bCursorHand	= true;
}

CPiIrregularWindow::~CPiIrregularWindow(void)
{
	DestroyIrregularWindow();
}


bool CPiIrregularWindow::SetTransWnd( HWND hWnd, LPCTSTR szPic /*= NULL*/ )
{
	m_hWndDist = hWnd;
	//SIZE sizeWindow = { pImage->GetWidth(), pImage->GetHeight()};
	/*
	GetWindowRect(m_hWndDist,&m_rcWindow);
		m_sizeWindow.cx = m_rcWindow.right = m_rcWindow.left;
		m_sizeWindow.cy = m_rcWindow.bottom = m_rcWindow.top;
	*/
	
	if (szPic)
	{
		m_strPicBk = szPic;
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(m_strPicBk.c_str());
		if(pImage->GetLastStatus() == Ok)
		{
			UINT nCX = pImage->GetWidth();
			UINT nCY = pImage->GetHeight();
			if (nCX > 0 && nCY > 0)
			{
				m_sizeWindow.cx = nCX;
				m_sizeWindow.cy = nCY;
			}
		}		
	}
	

	CreateMemDC();

	return true;

}


bool CPiIrregularWindow::UpdateLayeredWnd()
{
	BLENDFUNCTION blendFunc;
	blendFunc.BlendOp = 0;
	blendFunc.BlendFlags = 0;
	blendFunc.AlphaFormat = 1;
	blendFunc.SourceConstantAlpha = 255;//AC_SRC_ALPHA

	typedef BOOL (WINAPI *MYFUNC)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);          

	MYFUNC UpdateLayeredWindow;

	HMODULE hFuncInst = LoadLibrary(_T("User32.DLL"));
	UpdateLayeredWindow = (MYFUNC)::GetProcAddress(hFuncInst, "UpdateLayeredWindow");
	if(!UpdateLayeredWindow)
	{
		//AfxMessageBox(_T("null"));
		return false;
	}
	POINT ptSrc = {0, 0};

	CBrush b(RGB(0, 0, 255));


	RECT rcWindow;
	GetWindowRect(m_hWndDist,&rcWindow);
	POINT ptWinPos = { rcWindow.left, rcWindow.top };

	HDC hdc = GetDC(m_hWndDist);
	//

	//不会发送 WM_SIZE和WM_MOVE消息
	if( ! UpdateLayeredWindow(m_hWndDist, hdc, &ptWinPos, &m_sizeWindow, m_hdcMem, &ptSrc, 0, &blendFunc, ULW_ALPHA))
	{
		int nErr = GetLastError();
		TCHAR tmp[255] = {0};
#ifdef UNICODE
#define	MySprintf swprintf_s
#else
#define MySprintf sprintf_s
#endif
		MySprintf(tmp, _countof(tmp), _T("UpdateLayeredWindow 调用失败,错误代码:%d"), nErr);
		OutputDebugString(tmp);
		return false;
	}
	::ReleaseDC(m_hWndDist, hdc);
	return true;
}

bool CPiIrregularWindow::BeginDrawLayer()
{
	//清除旧dc绘制的内容
/*
	SolidBrush b(Color(255, 0,0,0));

	Graphics gp(m_hWndDist, TRUE);
	Rect rtObj(0,0, m_sizeWindow.cx, m_sizeWindow.cy);
	gp.FillRectangle(&b, rtObj);
*/


	HGDIOBJ hBmOld = ::SelectObject(m_hdcMem, m_bmMem);
	ClearObject(m_bmMem);
	HDC hDC = ::GetDC(m_hWndDist);
	//m_bmMem = CreateCompatibleBitmap(hDC, m_sizeWindow.cx, m_sizeWindow.cy);


	BYTE                        * pBits = NULL;
	BITMAPINFO          bmih;
	ZeroMemory( &bmih, sizeof( BITMAPINFO ) );

	bmih.bmiHeader.biSize                  = sizeof (BITMAPINFO) ;
	bmih.bmiHeader.biWidth                 = m_sizeWindow.cx ;
	bmih.bmiHeader.biHeight                = m_sizeWindow.cy ;
	bmih.bmiHeader.biPlanes                = 1 ;
	bmih.bmiHeader.biBitCount              = 32;        //这里一定要是32
	bmih.bmiHeader.biCompression           = BI_RGB ;
	bmih.bmiHeader.biSizeImage             = 0 ;
	bmih.bmiHeader.biXPelsPerMeter         = 0 ;
	bmih.bmiHeader.biYPelsPerMeter         = 0 ;
	bmih.bmiHeader.biClrUsed               = 0 ;
	bmih.bmiHeader.biClrImportant          = 0 ;

	m_bmMem = CreateDIBSection (NULL, (BITMAPINFO *)  &bmih, 0, (VOID**)&pBits, NULL, 0) ;



	SelectObject(m_hdcMem, m_bmMem);

	//绘制背景
	Image img(m_strPicBk.c_str());
	//TODO:检测图片构造是否成功
	if (img.GetLastStatus() == Ok)
	{
		//OutputDebugString(_T("图片加载失败"));
		Graphics gp(m_hdcMem);
		if(gp.DrawImage(&img, 0, 0, img.GetWidth(), img.GetHeight()) != Ok)
		{
			//return false;
		}
	}
	
	OnBeginDrawLayer();
	return true;
}

bool CPiIrregularWindow::CreateIrregularWindow( tcpchar szPic )
{
	//使用图片创建不规则窗口, 指定默认大小和默认位置
	if( ! RegisterWindowClass()
		|| ! this->_Create())
	{
		return false;
	}
	if( ! SetLayeredStyle(m_hWnd)
		|| ! SetTransWnd(m_hWnd, szPic))
	{
		return false;
	}
	bool bSuc = Update();
	if (bSuc)
	{
		g_mapObject[m_hWnd] = this;
	}
	return bSuc;
}

bool CPiIrregularWindow::CreateIrregularWindow( UINT nCX, UINT nCY )
{
	m_sizeWindow.cx = nCX;
	m_sizeWindow.cy = nCY;
	return CreateIrregularWindow(NULL);
}


bool CPiIrregularWindow::RegisterWindowClass()
{
	WNDCLASS wc = { 0 };
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = CPiIrregularWindow::__WndProc;
	wc.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);

	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName =__className;
	ATOM ret = ::RegisterClass(&wc);
	DWORD dwErr = GetLastError();
	if (!ret && dwErr != ERROR_CLASS_ALREADY_EXISTS)
	{
		return false;
	}
	return true;
}

bool CPiIrregularWindow::_Create()
{
	m_clText = m_clNormal;

	m_hWnd = ::CreateWindowEx(NULL/*WS_EX_LAYERED*/, __className, _T(""),	//WS_CAPTION,
		WS_OVERLAPPEDWINDOW,
		0, 0, 
		m_sizeWindow.cx, m_sizeWindow.cy, 
		NULL, NULL, (HINSTANCE)::GetModuleHandle(NULL), 0);

	if(m_hWnd == NULL || !::IsWindow(m_hWnd))
		return false;

	//ShowWindow(m_hWnd,SW_SHOWNORMAL);
	//UpdateWindow(m_hWnd);
	return true;
}


LRESULT CALLBACK  CPiIrregularWindow::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//HWND hAttackWnd = (HWND)GetWindowLong(hWnd,GWL_USERDATA);
	CPiIrregularWindow* pIW = g_mapObject[hWnd];
	switch(uMsg)
	{
	case WM_PAINT:
		break;
	case WM_CREATE:
		{
			LONG styleValue = ::GetWindowLong(hWnd, GWL_STYLE);
			styleValue &= ~WS_CAPTION;
			styleValue &= ~WS_SYSMENU;
			styleValue &= ~WS_MAXIMIZEBOX; 
			styleValue &= ~WS_MINIMIZEBOX; 
			styleValue &= ~WS_THICKFRAME; 
			styleValue &= ~WS_BORDER; 
			::SetWindowLong(hWnd, GWL_STYLE, styleValue | WS_CLIPSIBLINGS);
		}
		break;
	case WM_SIZE:
		{
			
		}
		break;
	case WM_MOUSEMOVE:
		{
			TRACKMOUSEEVENT tme = {0};
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.dwHoverTime = 1;
			tme.hwndTrack = hWnd;
			if(!TrackMouseEvent(&tme))
			{
				OutputDebugString(_T("TrackMouseEvent fail"));
			}
			if (pIW->m_bCursorHand)
			{
				::SetCursor(::LoadCursor(NULL, IDC_HAND));
			}
		}
		break;
	case WM_MOUSEHOVER:		
		if(!pIW->m_bMouseEnter)
		{
			pIW->OnMouseEnter(wParam, lParam);
		}
		break;
	case WM_MOUSELEAVE:
		
		pIW->OnMouseLeave(wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		{
		}
		break;
	case WM_LBUTTONUP:
		{

		}
		break;
	case WM_DESTROY:
		{
			OutputDebugString(_T("piIr destroy \n"));
			//PostQuitMessage(0) ;
			//DestroyWindow(hWnd);
		}
		break;
	case WM_NCDESTROY:
		{
			OutputDebugString(_T("piIr nc destroy \n"));
			//m_hWnd = NULL;
		}
		break;
	}
	bool bHandle = false;
	DWORD dwRet = 0;
	if (pIW)
	{
		dwRet = pIW->HandleMessage(uMsg, wParam, lParam, bHandle);
	}
	if (!bHandle)
	{
		return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return dwRet;	
}


bool CPiIrregularWindow::SetLayeredStyle( HWND hWnd )
{
	DWORD dwAttachWndStyleEx = GetWindowLong(hWnd,GWL_EXSTYLE);
	SetLastError(0);
	if(::SetWindowLong(hWnd,GWL_EXSTYLE,dwAttachWndStyleEx | WS_EX_LAYERED) == 0
		&& GetLastError())
	{
		return false;
	}
	::SetWindowPos(hWnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER);


	dwAttachWndStyleEx = GetWindowLong(hWnd,GWL_EXSTYLE);
	return true;
}

bool CPiIrregularWindow::DestroyIrregularWindow()
{
	if (m_hWnd)
	{
		::SendMessage(m_hWnd, WM_CLOSE, 0, 0);
		m_hWnd = NULL;
	}
	return true;
}

DWORD CPiIrregularWindow::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandle )
{
	bHandle = false;
	return 1;
}

bool CPiIrregularWindow::OnBeginDrawLayer()
{
	throw std::exception("The method or operation is not implemented.");
}

bool CPiIrregularWindow::CreateMemDC()
{
	HDC hDC = ::GetDC(m_hWndDist);

	if (!m_hdcMem)
	{
		m_hdcMem = CreateCompatibleDC(hDC);
	}
	//::ReleaseDC(m_hWndDist, hDC);
	if (m_bmMem)
	{
		::SelectObject(m_hdcMem, m_bmMem);
		ClearObject(m_bmMem);
	}
	//m_bmMem = CreateCompatibleBitmap(hDC, m_sizeWindow.cx, m_sizeWindow.cy);

	BYTE                           * pBits = NULL;
	BITMAPINFO          bmih;
	ZeroMemory( &bmih, sizeof( BITMAPINFO ) );

	bmih.bmiHeader.biSize                  = sizeof (BITMAPINFO) ;
	bmih.bmiHeader.biWidth                 = m_sizeWindow.cx ;
	bmih.bmiHeader.biHeight                = m_sizeWindow.cy ;
	bmih.bmiHeader.biPlanes                = 1 ;
	bmih.bmiHeader.biBitCount              = 32;        //这里一定要是32
	bmih.bmiHeader.biCompression           = BI_RGB ;
	bmih.bmiHeader.biSizeImage             = 0 ;
	bmih.bmiHeader.biXPelsPerMeter         = 0 ;
	bmih.bmiHeader.biYPelsPerMeter         = 0 ;
	bmih.bmiHeader.biClrUsed               = 0 ;
	bmih.bmiHeader.biClrImportant          = 0 ;

	m_bmMem = CreateDIBSection (NULL, (BITMAPINFO *)  &bmih, 0, (VOID**)&pBits, NULL, 0) ;
	::SelectObject(m_hdcMem, m_bmMem);
	return m_bmMem > 0;
}

bool CPiIrregularWindow::Update() 
{
	return BeginDrawLayer()
		&& UpdateLayeredWnd();
}

LRESULT CPiIrregularWindow::OnMouseEnter( WPARAM wp, LPARAM lp )
{
	m_bMouseEnter = true;
	m_clText = m_clEnter;
	
	//OutputDebugString(_T("OnMouseEnter"));
	Update();
	return 1;
}

LRESULT CPiIrregularWindow::OnMouseLeave( WPARAM wp, LPARAM lp )
{
	m_bMouseEnter = false;
	m_clText = m_clNormal;
	//OutputDebugString(_T("OnMouseLeave"));
	Update();
	return 1;
}