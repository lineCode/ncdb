#include "StdAfx.h"
#include "PiIrregularWindow.h"
#include "ResCommon.h"
#include "functional.h"

static const TCHAR* __className  =  _T("__IrregularWindowClass");
static const TCHAR* __windowName =  _T("__IrregularWindow");

CPiIrregularWindow::CPiIrregularWindow(void)

{
	m_hdcMem = NULL;
	m_hWndDist	= NULL;
	m_hdcMem 	= NULL;;
	m_bmMem	= NULL;
	m_sizeWindow.cx = m_sizeWindow.cy = 0;
		

}

CPiIrregularWindow::~CPiIrregularWindow(void)
{
	DestroyIrregularWindow();
}


bool CPiIrregularWindow::SetTransWnd( HWND hWnd, LPCTSTR szPic /*= NULL*/ )
{
	m_hWndDist = hWnd;
	//SIZE sizeWindow = { pImage->GetWidth(), pImage->GetHeight()};
	GetWindowRect(m_hWndDist,&m_rcWindow);
	m_sizeWindow.cx = m_rcWindow.right = m_rcWindow.left;
	m_sizeWindow.cy = m_rcWindow.bottom = m_rcWindow.top;

	if (szPic)
	{
		m_strPicBk = szPic;
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(m_strPicBk.c_str());
		if(pImage->GetLastStatus() != Ok)
		{
			return false;
		}
		m_sizeWindow.cx = pImage->GetWidth();
		m_sizeWindow.cy = pImage->GetHeight();
		if (m_sizeWindow.cx <= 0 || m_sizeWindow.cy <= 0)
		{
			return false;
		}
	}
	

	HDC hDC = ::GetDC(m_hWndDist);

	if (!m_hdcMem)
	{
		m_hdcMem = CreateCompatibleDC(hDC);
	}

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
		OutInfo(tmp);
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
	if (img.GetLastStatus() != Ok)
	{
		OutInfo(_T("图片加载失败"));
		return false;
	}
	Graphics gp(m_hdcMem);
	if(gp.DrawImage(&img, 0, 0, img.GetWidth(), img.GetHeight()) != Ok)
	{
		return false;
	}
	return true;
}

bool CPiIrregularWindow::CreateIrregularWindow( tcpchar szPic )
{
	if (!szPic)
	{
		return false;
	}
	//使用图片创建不规则窗口, 指定默认大小和默认位置
	if( ! RegisterWindowClass()
		|| ! this->Create())
	{
		return false;
	}
	if( ! SetLayeredStyle(m_hWnd)
		|| ! SetTransWnd(m_hWnd, szPic))
	{
		return false;
	}
	
	return BeginDrawLayer()
		&& UpdateLayeredWnd();
}


bool CPiIrregularWindow::RegisterWindowClass()
{
	WNDCLASS wc = { 0 };
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = CPiIrregularWindow::__WndProc;
	//wc.lpfnWndProc = DefWindowProc;
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

bool CPiIrregularWindow::Create()
{
	m_hWnd = ::CreateWindowEx(NULL/*WS_EX_LAYERED*/, __className, _T(""),	//WS_CAPTION,
		WS_OVERLAPPEDWINDOW,
		300, 300, 
		300, 300, 
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
	switch(uMsg)
	{
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
	case WM_MOVE:
		{
			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			OutputDebugString(_T("IrWnd lbtnDown\n"));
			return 0;
			//::SendMessage( hWnd, WM_SYSCOMMAND, 0xF012, 0);
			//::SendMessage( hAttackWnd, WM_SYSCOMMAND, 0xF012, 0);
		}
	case WM_DESTROY:
		OutputDebugString(_T("destroy \n"));
		PostQuitMessage(0) ;
		break;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
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
		::SendMessage(m_hWnd, WM_DESTROY, 0, 0);
	}
	return true;
}
