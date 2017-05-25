#include "StdAfx.h"
#include "PiWindowPack.h"
#include "define_gnl.h"
#include "DataStruct\PiRect.h"
#include <assert.h>

CPiWindowPack::CPiWindowPack(void)
{
}

CPiWindowPack::~CPiWindowPack(void)
{
}

bool CPiWindowPack::_ModifyStyle(HWND hWnd, UNINT nRemove, UNINT nAdd, int nStyle)
{
	DWORD dwAttachWndStyleEx = GetWindowLong(hWnd,nStyle);
	return ::SetWindowLong(hWnd,nStyle,dwAttachWndStyleEx & ~nRemove | nAdd) != 0;
}

bool CPiWindowPack::ModifyStyle(HWND hWnd, UNINT nRemove, UNINT nAdd )
{
	return _ModifyStyle(hWnd, nRemove, nAdd, GWL_STYLE);
}

bool CPiWindowPack::ModifyStyleEx(HWND hWnd, UNINT nRemove, UNINT nAdd )
{
	return _ModifyStyle(hWnd, nRemove, nAdd, GWL_EXSTYLE);
}

bool CPiWindowPack::MoveWindow( HWND hWnd, POINT ptDist )
{
	MoveWindow(hWnd, ptDist.x, ptDist.y);
	return true;
}

bool CPiWindowPack::MoveWindow( HWND hWnd, int nX, int nY )
{
	POINT pt = { 0 };
	GetWindowPos(hWnd, pt);
	if (nX == -1)
	{
		nX = pt.x;
	}
	else if(nY == -1)
	{
		nY = pt.y;
	}
	pt.x = nX;
	pt.y = nY;
	HWND hParent = ::GetParent(hWnd);
	
	if (hParent 
		&& (GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD))
	{
		ScreenToClient(hParent, &pt);
	}
	::SetWindowPos(hWnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
		| SWP_NOSENDCHANGING);
	return true;
}

bool CPiWindowPack::ChangeSize( HWND hWnd, POINT ptSize )
{
	ChangeSize(hWnd, ptSize.x, ptSize.y);
	return true;
}

bool CPiWindowPack::ChangeSize( HWND hWnd, int nWidth, int nHeight )
{
	SIZE szSource = GetWindowSize(hWnd);
	int nWidthT = nWidth;
	int nHeightT = nHeight;
	if (nWidthT == -1)
	{
		nWidthT = szSource.cx;
	}
	if (nHeightT == -1)
	{
		nHeightT = szSource.cy;

	}
	::SetWindowPos(hWnd, NULL, 0, 0, nWidthT, nHeightT, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
		| SWP_NOREDRAW | SWP_NOSENDCHANGING);
	return true;
}

bool CPiWindowPack::SetTrackMouse( HWND hWnd )
{
	TRACKMOUSEEVENT tme = {0};
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.hwndTrack = hWnd;
	tme.dwHoverTime = 1;
	if(!TrackMouseEvent(&tme))
	{
		assert(0 && _T("set track leave fail"));
		return false;
	}
	return true;
}

bool CPiWindowPack::GetWindowPos( HWND hWnd, POINT& pt )
{
	RECT rt = {0};
	if(!GetWindowRect(hWnd, &rt))
	{
		return false;
	}
	pt.x = rt.left;
	pt.y = rt.top;
	return true;
}

bool CPiWindowPack::TopMostWindow( HWND hWnd , bool bTopMost /*= true*/ )
{
	HWND hTop = (bTopMost ? HWND_TOPMOST : HWND_NOTOPMOST);
	SetForegroundWindow(hWnd);
	::SetWindowPos(hWnd, hTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	return true;
}

bool CPiWindowPack::TopWindow( HWND hWnd )
{
	SetForegroundWindow(hWnd);
	::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	return true;
}

SIZE CPiWindowPack::GetWindowSize( HWND hWnd )
{
	RECT rt = {0};
	GetWindowRect(hWnd, &rt);
	SIZE sz = {rt.right - rt.left, rt.bottom - rt.top};
	return sz;
}

bool CPiWindowPack::SetNoTaskWindow( HWND hWnd )
{
	return CPiWindowPack::ModifyStyleEx(hWnd, WS_EX_APPWINDOW, WS_EX_TOOLWINDOW)
	&& SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
		| SWP_NOREDRAW | SWP_FRAMECHANGED);
}

POINT CPiWindowPack::CalcWndCloseRightBottomPos( HWND hWnd, POINT ptSpace )
{
	POINT ptDeskTopSize = GetDeskTopSize();
	RECT rt = {0};
	::GetWindowRect(hWnd, &rt);
	POINT ptDist = {0};
	ptDist.x = ptDeskTopSize.x - (rt.right - rt.left) - ptSpace.x;
	ptDist.y = ptDeskTopSize.y - (rt.bottom - rt.top) - ptSpace.y;
	return ptDist;
}

POINT CPiWindowPack::GetDeskTopSize()
{
	POINT pt = {0};
	RECT rtWorkArea = {0};
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rtWorkArea, 0);
	pt.x = rtWorkArea.right - rtWorkArea.left;
	pt.y = rtWorkArea.bottom - rtWorkArea.top;
	return pt;
}

RECT CPiWindowPack::GetDeskTopRect()
{
	RECT rtWorkArea = {0};
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rtWorkArea, 0);
	return rtWorkArea;
}

POINT CPiWindowPack::GetScreenSize()
{
	POINT pt = {0};
	pt.x = GetSystemMetrics(SM_CXSCREEN);
	pt.y = GetSystemMetrics(SM_CYSCREEN);
	return pt;
}



POINT CPiWindowPack::CalcWndCloseRightBottomPos( int nCx, int nCy  )
{
	POINT ptDeskTopSize = GetDeskTopSize();
	POINT ptLeftTopPos = {0};

	ptLeftTopPos.x = ptDeskTopSize.x - nCx;
	ptLeftTopPos.y = ptDeskTopSize.y - nCy;

	return ptLeftTopPos;
}

POINT CPiWindowPack::CalcWndCloseRightTopPos( HWND hWnd, POINT ptSpace )
{
	POINT ptDeskTopSize = GetDeskTopSize();
	RECT rt = {0};
	::GetWindowRect(hWnd, &rt);
	POINT ptDist = {0};
	ptDist.x = ptDeskTopSize.x - (rt.right - rt.left) - ptSpace.x;
	ptDist.y = 0 + ptSpace.y;
	return ptDist;
}

HWND CPiWindowPack::GetWindowHWND( tcpchar strTitle, tcpchar strClassName )
{
	return ::FindWindow(strClassName, strTitle);
}

bool CPiWindowPack::EnableDrawFile( HWND hWnd, bool bAccept /*= true*/ )
{
	DragAcceptFiles(hWnd, bAccept);
	return true;
}

tstring CPiWindowPack::GetOneDragFilePath( const HDROP& hd, bool bDropFinish /*= true*/ )
{
	tstring strName;
	strName.resize(FILENAME_MAX);

	DragQueryFile(hd, 0, &strName.at(0), FILENAME_MAX);

	if (bDropFinish)
	{
		DragFinish(hd);
	}
	return strName.c_str();
}

tstring CPiWindowPack::GetOneDragFilePath(const HDROP& hd)
{
	tstring strName;
	strName.resize(FILENAME_MAX);

	int nTotal = DragQueryFile(hd, 0xFFFFFFFF, 0, 0);
	if (!nTotal)
	{
		return strName;
	}
	DragQueryFile(hd, 0, &strName.at(0), FILENAME_MAX);
	return strName.c_str();
}

LST_STRING CPiWindowPack::GetDragFilePathList( const HDROP& hd, bool bDropFinish /*= true*/ )
{
	LST_STRING lstPath;

	tstring strName;
	UINT nIndex = 0;
	UINT nTotal = 0;
	UINT nBufNeed = 0;
	nTotal = DragQueryFile(hd, 0xFFFFFFFF, 0, 0);
	FOR_COUNT(nTotal, nIndex)
	{
		nBufNeed = DragQueryFile(hd, nIndex, NULL, 0);
		++nBufNeed;
		strName.resize(nBufNeed, 0);
		if(!DragQueryFile(hd, nIndex, &strName.at(0), nBufNeed))
		{
			continue;
		}
		lstPath.push_back(strName);
	}

	if (bDropFinish)
	{
		DragFinish(hd);
	}
	return lstPath;
}

bool CPiWindowPack::IsOwnerWindow( HWND hParent, HWND hFind )
{
	return ( hParent == hFind )
		|| (::IsChild(hParent, hFind) == TRUE );
	
}

bool CPiWindowPack::CenterWindow( HWND hWnd, HWND hWndBase )
{
	//获取基准窗口位置, 尺寸.  2. 获取移动窗口尺寸, 
	RECT rtBase = {0};
	if(!GetWindowRect(hWndBase, &rtBase))
	{
		return false;
	}
	
	SIZE szNewWnd = GetWindowSize(hWnd);
	CPiRect rt(rtBase);
	POINT ptNewWnd = rt.GetCenterPoint(szNewWnd);
	
	::SetWindowPos(hWnd, NULL, ptNewWnd.x, ptNewWnd.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
		| SWP_NOSENDCHANGING);
	return true;
}

int CPiWindowPack::CenterPosX( ARR_RECT& lstRect, int nSpace, int nAreaWidth )
{
	/*-------------------- 
		第一控件横坐标 区域宽度/2 - (所有按钮宽度 +  (控件个数-1)*控件之间的空隙)/2
		第二个控件横坐标: 前面一个的横坐标+ 前面一个的宽度 + 空隙
		第三个控件横坐标: 前面一个的横坐标+ 前面一个的宽度 + 空隙
	--------------------*/
	int nAllBtnWidth = 0;	
	const int nPadding = 5;

	for (size_t i = 0; i < lstRect.size(); ++i)
	{
		nAllBtnWidth += lstRect[i].GetWidth();
	}
	int nTotalWidth = (nAllBtnWidth + (((int)lstRect.size() - 1) * nSpace));
	nAreaWidth = max(nAreaWidth, nTotalWidth + nPadding * 2);

	for (size_t i = 0; i < lstRect.size(); ++i)
	{
		int nNewX = 0;
		long nYOrigion = lstRect[i].m_rt.top;
		if (i == 0)
		{
			nNewX = nAreaWidth/2 - nTotalWidth/2;
			lstRect[i].SetPos(nNewX, nYOrigion);
		}
		else
		{
			CPiRect& rtPrevious = lstRect[i - 1];
			nNewX = rtPrevious.m_rt.left + rtPrevious.GetWidth() + nSpace;
			lstRect[i].SetPos(nNewX, nYOrigion);
		}
	}

	return nAreaWidth;
}

RECT CPiWindowPack::GetClientRectToParent(HWND hWnd)
{
	RECT rtTemp = { 0 };
	RECT rt = { 0 };
	::GetWindowRect(hWnd, &rtTemp);
	rtTemp;
	POINT pt = { rtTemp.left, rtTemp.top };
	ScreenToClient(::GetParent(hWnd), &pt);
	rt.left = pt.x;
	rt.top = pt.y;
	rt.right = rt.left + rtTemp.right - rtTemp.left;
	rt.bottom = rt.top + (rtTemp.bottom - rtTemp.top);
	return rt;
}

