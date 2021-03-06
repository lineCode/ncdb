#include "stdafx.h"
#include "IrregularWindow.h"
#include <gdiplus.h>
#include <windowsx.h>
#include <locale.h>

#pragma comment(lib, "gdiplus.lib")

const TCHAR* __className  =  _T("__IrregularWindowClass");
const TCHAR* __windowName =  _T("__IrregularWindow");

ULONG_PTR CIrregularWindow::gdiplusToken = 0;
HHOOK CIrregularWindow::hhk = NULL;
CIrregularWindow* CIrregularWindow::g_pIrrglWnd = NULL;


CIrregularWindow::CIrregularWindow(const TCHAR* pBackImgFullPath,DWORD dwAttachWndTransColor)
:m_hWnd(NULL)
,m_hAttachWnd(NULL)
,m_dwAttachWndTransColor(dwAttachWndTransColor)
{
	g_pIrrglWnd = this;
	if(pBackImgFullPath != NULL)
	{
		m_strBackImg = pBackImgFullPath;

		/*if(!RegisterWindowClass() || !Create())
		{
			assert(false && _T("背景窗体创建失败!"));
		}*/
	}

	setlocale(LC_ALL,"");
}


bool CIrregularWindow::RegisterWindowClass()
{
    WNDCLASS wc = { 0 };
    wc.style = 0;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = NULL;
    wc.lpfnWndProc = CIrregularWindow::__WndProc;
	wc.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	
	wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName =__className;
    ATOM ret = ::RegisterClass(&wc);
  
	assert(ret != NULL);

	return ret != NULL;
}

bool CIrregularWindow::Create()
{
	m_hWnd = ::CreateWindowEx(WS_EX_LAYERED, __className, _T(""),WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		300, 300, 
		NULL, NULL, (HINSTANCE)::GetModuleHandle(NULL), 0);

	if(m_hWnd == NULL || !::IsWindow(m_hWnd))
		return false;

	ShowWindow(m_hWnd,SW_SHOWNORMAL);

	return true;
}

bool CIrregularWindow::AttachWindow(HWND hWnd)
{
	if(m_hAttachWnd != NULL)
	{
		assert(false && _T("不能重复AttachWindow!"));
		return false;
	}

	if(!IsWindow(hWnd))
	{
		assert(false && _T("无效的窗口句柄!"));
		return false;
	}

	m_hAttachWnd = hWnd;

	SetWindowLong(m_hWnd,GWL_USERDATA,(LONG)m_hAttachWnd);

	//将附加窗体样式 加上WS_EX_LAYERED
	DWORD dwAttachWndStyleEx = GetWindowLong(m_hAttachWnd,GWL_EXSTYLE);
	SetLastError(0);
	if(::SetWindowLong(m_hAttachWnd,GWL_EXSTYLE,dwAttachWndStyleEx | WS_EX_LAYERED) == 0
		&& GetLastError())
	{
		return 0;
	}
	//::SetWindowPos(m_hAttachWnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER);

	dwAttachWndStyleEx = GetWindowLong(m_hAttachWnd,GWL_EXSTYLE);

	//::SetWindowPos(m_hAttachWnd, NULL, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	SetBackground(m_strBackImg.c_str());
	//SetAttachWndTransColor(m_dwAttachWndTransColor);

	return true;
}

void CIrregularWindow::SetBackground(const TCHAR* pBackImgFullPath)
{
	if(pBackImgFullPath != NULL)
	{
		m_strBackImg = pBackImgFullPath;
	}

	if(!m_strBackImg.empty())
	{
#ifdef UNICODE
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(pBackImgFullPath);
#else
		size_t szHasConv = 0;
		DWORD dwSize = m_strBackImg.size() + 1;
		wchar_t* pImgPath = new wchar_t[dwSize];
		memset((void*)pImgPath,0,sizeof(wchar_t) * dwSize);
		::mbstowcs_s(&szHasConv,pImgPath,dwSize,m_strBackImg.c_str(),m_strBackImg.size());
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(pImgPath);
		delete [] pImgPath;	
#endif
		HWND hWndDist = m_hAttachWnd;
		if(pImage)
		{
			BLENDFUNCTION blendFunc;
			blendFunc.BlendOp = 0;
			blendFunc.BlendFlags = 0;
			blendFunc.AlphaFormat = 1;
			blendFunc.SourceConstantAlpha = 255;//AC_SRC_ALPHA

			SIZE sizeWindow = { pImage->GetWidth(), pImage->GetHeight()};

			HDC hDC = ::GetDC(hWndDist);

			HDC hdcMemory = CreateCompatibleDC(hDC);

			HBITMAP hBitMap = CreateCompatibleBitmap(hDC, sizeWindow.cx, sizeWindow.cy);

			::SelectObject(hdcMemory, hBitMap);

			RECT rcWindow;
			GetWindowRect(hWndDist,&rcWindow);

			BITMAPINFOHEADER stBmpInfoHeader = { 0 };   
			int nBytesPerLine = ((sizeWindow.cx * 32 + 31) & (~31)) >> 3;
			stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);   
			stBmpInfoHeader.biWidth = sizeWindow.cx;   
			stBmpInfoHeader.biHeight = sizeWindow.cy;   
			stBmpInfoHeader.biPlanes = 1;   
			stBmpInfoHeader.biBitCount = 32;   
			stBmpInfoHeader.biCompression = BI_RGB;   
			stBmpInfoHeader.biClrUsed = 0;   
			stBmpInfoHeader.biSizeImage = nBytesPerLine * sizeWindow.cy;   

			PVOID pvBits = NULL;   

			HBITMAP hbmpMem = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, &pvBits, NULL, 0);

			assert(hbmpMem != NULL);

			memset( pvBits, 0, sizeWindow.cx * 4 * sizeWindow.cy);

			if(hbmpMem)   
			{   
				HGDIOBJ hbmpOld = ::SelectObject( hdcMemory, hbmpMem);
				
				POINT ptWinPos = { rcWindow.left, rcWindow.top };

				Gdiplus::Graphics graph(hdcMemory);

				graph.SetSmoothingMode(Gdiplus::SmoothingModeNone);

				graph.DrawImage(pImage, 0, 0, sizeWindow.cx, sizeWindow.cy);

				HMODULE hFuncInst = LoadLibrary(_T("User32.DLL"));

				typedef BOOL (WINAPI *MYFUNC)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);          

				MYFUNC UpdateLayeredWindow;

				UpdateLayeredWindow = (MYFUNC)::GetProcAddress(hFuncInst, "UpdateLayeredWindow");

				POINT ptSrc = { 0, 0};

				CBrush b(RGB(0, 0, 255));
				//HDC hdc = GetDC(hWndDist);

				//FillRect(hdcMemory, CRect(0,0, 100, 100), b);
				//::ReleaseDC(hWndDist, hdc);

				//不会发送 WM_SIZE和WM_MOVE消息
				if(/*0 && */!UpdateLayeredWindow(hWndDist, hDC, &ptWinPos, &sizeWindow, hdcMemory, &ptSrc, 0, &blendFunc, ULW_ALPHA))
				{
					TCHAR tmp[255] = {_T('\0')};
#ifdef UNICODE
	#define	MySprintf swprintf_s
#else
	#define MySprintf sprintf_s
#endif
					MySprintf(tmp,255,__T("UpdateLayeredWindow 调用失败,错误代码:%u"),GetLastError());

					MessageBox(hWndDist,tmp,_T("提示"),MB_OK);
				}

				//::SendMessage(hWndDist,WM_SIZE,0,MAKELONG(sizeWindow.cy,sizeWindow.cx));

				//::SendMessage(hWndDist,WM_MOVE,0,MAKELONG(ptSrc.y,ptSrc.x));


				


				graph.ReleaseHDC(hdcMemory);

				::SelectObject( hdcMemory, hbmpOld);   

				::DeleteObject(hbmpMem); 
			}




			::DeleteDC(hdcMemory);
			
			::DeleteDC(hDC);



		}
		else
		{
			assert(false && _T("背景图片打开失败!"));
		}
	}
}

void CIrregularWindow::SetBackground( HWND hWnd, const TCHAR* pBackImgFullPath )
{
	if(pBackImgFullPath == NULL)
	{
		//pBackImgFullPath = pBackImgFullPath;
		return ;
	}

	if(pBackImgFullPath)
	{
#ifdef UNICODE
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(pBackImgFullPath);
#else
		size_t szHasConv = 0;
		DWORD dwSize = pBackImgFullPath.size() + 1;
		wchar_t* pImgPath = new wchar_t[dwSize];
		memset((void*)pImgPath,0,sizeof(wchar_t) * dwSize);
		::mbstowcs_s(&szHasConv,pImgPath,dwSize,pBackImgFullPath.c_str(),pBackImgFullPath.size());
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(pImgPath);
		delete [] pImgPath;	
#endif
		HWND hWndDist = hWnd;
		if(pImage)
		{
			BLENDFUNCTION blendFunc;
			blendFunc.BlendOp = 0;
			blendFunc.BlendFlags = 0;
			blendFunc.AlphaFormat = 1;
			blendFunc.SourceConstantAlpha = 255;//AC_SRC_ALPHA

			SIZE sizeWindow = { pImage->GetWidth(), pImage->GetHeight()};

			HDC hDC = ::GetDC(hWndDist);

			HDC hdcMemory = CreateCompatibleDC(hDC);

			HBITMAP hBitMap = CreateCompatibleBitmap(hDC, sizeWindow.cx, sizeWindow.cy);

			::SelectObject(hdcMemory, hBitMap);

			RECT rcWindow;
			GetWindowRect(hWndDist,&rcWindow);

			/*BITMAPINFOHEADER stBmpInfoHeader = { 0 };   
			int nBytesPerLine = ((sizeWindow.cx * 32 + 31) & (~31)) >> 3;
			stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);   
			stBmpInfoHeader.biWidth = sizeWindow.cx;   
			stBmpInfoHeader.biHeight = sizeWindow.cy;   
			stBmpInfoHeader.biPlanes = 1;   
			stBmpInfoHeader.biBitCount = 32;   
			stBmpInfoHeader.biCompression = BI_RGB;   
			stBmpInfoHeader.biClrUsed = 0;   
			stBmpInfoHeader.biSizeImage = nBytesPerLine * sizeWindow.cy;   

			//PVOID pvBits = NULL;   

			HBITMAP hbmpMem = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, &pvBits, NULL, 0);
			//assert(hbmpMem != NULL);

			//memset( pvBits, 0, sizeWindow.cx * 4 * sizeWindow.cy);
			
			if(hbmpMem)   
			*/
			if(1)   
			{   
				//HGDIOBJ hbmpOld = ::SelectObject( hdcMemory, hbmpMem);

				POINT ptWinPos = { rcWindow.left, rcWindow.top };

				Gdiplus::Graphics graph(hdcMemory);

				graph.SetSmoothingMode(Gdiplus::SmoothingModeNone);

				graph.DrawImage(pImage, 0, 0, sizeWindow.cx, sizeWindow.cy);

				HMODULE hFuncInst = LoadLibrary(_T("User32.DLL"));
				typedef BOOL (WINAPI *MYFUNC)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);          
				MYFUNC UpdateLayeredWindow;
				UpdateLayeredWindow = (MYFUNC)::GetProcAddress(hFuncInst, "UpdateLayeredWindow");

				POINT ptSrc = { 0, 0};

				CBrush b(RGB(0, 0, 255));
				//HDC hdc = GetDC(hWndDist);

				//FillRect(hdcMemory, CRect(0,0, 100, 100), b);
				//::ReleaseDC(hWndDist, hdc);

				//不会发送 WM_SIZE和WM_MOVE消息
				if(/*0 && */!UpdateLayeredWindow(hWndDist, hDC, &ptWinPos, &sizeWindow, hdcMemory, &ptSrc, 0, &blendFunc, ULW_ALPHA))
				{
					TCHAR tmp[255] = {_T('\0')};
#ifdef UNICODE
#define	MySprintf swprintf_s
#else
#define MySprintf sprintf_s
#endif
					MySprintf(tmp,255,__T("UpdateLayeredWindow 调用失败,错误代码:%u"),GetLastError());

					MessageBox(hWndDist,tmp,_T("提示"),MB_OK);
				}
				//::SendMessage(hWndDist,WM_SIZE,0,MAKELONG(sizeWindow.cy,sizeWindow.cx));
				//::SendMessage(hWndDist,WM_MOVE,0,MAKELONG(ptSrc.y,ptSrc.x));

				graph.ReleaseHDC(hdcMemory);

				//::SelectObject( hdcMemory, hbmpOld);   

				//::DeleteObject(hbmpMem); 
			}




			::DeleteDC(hdcMemory);

			::DeleteDC(hDC);



		}
		else
		{
			assert(false && _T("背景图片打开失败!"));
		}
	}
}

void CIrregularWindow::SetAttachWndTransColor(DWORD dwColor)
{
	m_dwAttachWndTransColor = dwColor;

	HMODULE hInst = LoadLibrary(_T("User32.DLL")); 

	typedef BOOL (WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD); 

	MYFUNC SetLayeredWindowAttributes = NULL; 

	SetLayeredWindowAttributes = (MYFUNC)::GetProcAddress(hInst, "SetLayeredWindowAttributes"); 

	if(SetLayeredWindowAttributes)
	{
		if(!SetLayeredWindowAttributes(m_hAttachWnd, m_dwAttachWndTransColor, 255, LWA_ALPHA))
		{
			assert(false && _T("SetLayeredWindowAttributes 失败了!"));
		}
	}

	::FreeLibrary(hInst);
}

void CIrregularWindow::InitGDIplus()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void CIrregularWindow::UnInitGDIplus()
{
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

void CIrregularWindow::CenterWindow()
{
	RECT rcDlg = {0};
    ::GetWindowRect(m_hWnd, &rcDlg);
    RECT rcArea = { 0 };
    RECT rcCenter = { 0 };
    HWND hWndParent = ::GetParent(m_hWnd);
    HWND hWndCenter = ::GetWindowOwner(m_hWnd);
    ::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
    if( hWndCenter == NULL ) rcCenter = rcArea; else ::GetWindowRect(hWndCenter, &rcCenter);

    int DlgWidth = rcDlg.right - rcDlg.left;
    int DlgHeight = rcDlg.bottom - rcDlg.top;

    // Find dialog's upper left based on rcCenter
    int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
    int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

    // The dialog is outside the screen, move it inside
    if( xLeft < rcArea.left ) xLeft = rcArea.left;
    else if( xLeft + DlgWidth > rcArea.right ) xLeft = rcArea.right - DlgWidth;
    if( yTop < rcArea.top ) yTop = rcArea.top;
    else if( yTop + DlgHeight > rcArea.bottom ) yTop = rcArea.bottom - DlgHeight;
    ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

LRESULT CALLBACK  CIrregularWindow::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hAttackWnd = (HWND)GetWindowLong(hWnd,GWL_USERDATA);
	switch(uMsg)
	{
	case WM_CREATE:
		{
			LONG styleValue = ::GetWindowLong(hWnd, GWL_STYLE);
			styleValue &= ~WS_CAPTION;
			styleValue &= ~WS_MAXIMIZEBOX; 
			styleValue &= ~WS_MINIMIZEBOX; 
			styleValue &= ~WS_THICKFRAME; 
			styleValue &= ~WS_BORDER; 
			styleValue &= ~WS_CAPTION;
			::SetWindowLong(hWnd, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		}
		break;
	/*case WM_PAINT:
		{
			CBrush b(RGB(255, 0, 255));
			HDC hdc = GetDC(hWnd);
			CDC dc;
			dc.Attach(GetDC(hWnd));
			dc.FillRect(CRect(0,0, 300, 300), &b);
			::ReleaseDC(hWnd, hdc);

			

			break;
		}*/
	case WM_SIZE:
		{
			short cx = LOWORD(lParam); 
			short cy = HIWORD(lParam);

			::SetWindowPos(hAttackWnd, NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
		}
		break;
	case WM_MOVE:
		{
			RECT rect;
			::GetWindowRect(hWnd,&rect);
			::SetWindowPos(hAttackWnd, NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			OutputDebugString(_T("IrWnd lbtnDown\n"));
			//::SendMessage( hWnd, WM_SYSCOMMAND, 0xF012, 0);
			//::SendMessage( hAttackWnd, WM_SYSCOMMAND, 0xF012, 0);
		}
		break;
	}
	CIrregularWindow* pWnd = g_pIrrglWnd;
	if(pWnd && pWnd->HandleMessage(uMsg, wParam, lParam))
	{
		return true;
	}
	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}

bool CIrregularWindow::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return ::SendMessage(m_hAttachWnd, uMsg, wParam, lParam);
	return false;
}

HWND CIrregularWindow::CreateBaseTransparentWnd()
{
	SetTransSelf(m_dwAttachWndTransColor);
	return m_hWnd;
}

void CIrregularWindow::SetTransSelf( DWORD dwColor )
{
	/*CBrush b(RGB(255, 0, 255));
	HDC hdc = GetDC(m_hWnd);
	CDC dc;
	dc.Attach(GetDC(m_hWnd));
	dc.FillRect(CRect(0,0, 50, 50), &b);
	::ReleaseDC(m_hWnd, hdc);*/


	m_dwAttachWndTransColor = dwColor;

	HMODULE hInst = LoadLibrary(_T("User32.DLL")); 

	typedef BOOL (WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD); 

	MYFUNC SetLayeredWindowAttributes = NULL; 

	SetLayeredWindowAttributes = (MYFUNC)::GetProcAddress(hInst, "SetLayeredWindowAttributes"); 

	if(SetLayeredWindowAttributes)
	{
		if(!SetLayeredWindowAttributes(m_hWnd, m_dwAttachWndTransColor, 0, LWA_COLORKEY))
		{
			assert(false && _T("SetLayeredWindowAttributes 失败了!"));
		}
	}

	::FreeLibrary(hInst);
}

void CIrregularWindow::SetWndTransColor( HWND hWnd, COLORREF nColor )
{
	//m_dwAttachWndTransColor = dwColor;
	
	
	//CBrush b(RGB(255, 0, 255));
	//HDC hdc = GetDC(m_hWnd);
	//CDC dc;
	
	/*RECT rt = {0};
	GetWindowRect(hWnd, &rt);
	SolidBrush b(RGB(255, 0, 255));
	Graphics gp(hWnd, TRUE);;
	gp.FillRectangle(&b, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top);*/

	//::FillRect(CRect(0,0, 50, 50), &b);
	//::ReleaseDC(m_hWnd, hdc);

	HMODULE hInst = LoadLibrary(_T("User32.DLL")); 

	typedef BOOL (WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD); 

	MYFUNC SetLayeredWindowAttributes = NULL; 

	SetLayeredWindowAttributes = (MYFUNC)::GetProcAddress(hInst, "SetLayeredWindowAttributes"); 

	if(SetLayeredWindowAttributes)
	{ 
		//if(!SetLayeredWindowAttributes(hWnd, nColor, 100, LWA_ALPHA))
		if(!SetLayeredWindowAttributes(hWnd, nColor, 0, LWA_COLORKEY))
		{
			int n= GetLastError();
			assert(false && _T("SetLayeredWindowAttributes 失败了!"));
		}
	}


	::FreeLibrary(hInst);
}

bool CIrregularWindow::SetLayeredStyle( HWND hWnd )
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
