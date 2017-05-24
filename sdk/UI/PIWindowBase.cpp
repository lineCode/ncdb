#include "StdAfx.h"
#include "PIWindowBase.h"
#include "PiWindowPack.h"

CPIWindowBase::CPIWindowBase( HWND hWnd /*= NULL*/ )
:m_hWnd(hWnd)
{

}
CPIWindowBase::~CPIWindowBase(void)
{
}

SIZE CPIWindowBase::GetWindowSize()
{
	return CPiWindowPack::GetWindowSize(m_hWnd);
}


bool CPIWindowBase::IsWindow()
{
	return ::IsWindow(m_hWnd) == TRUE;

}

CPIWindowBase::operator HWND()
{
	return m_hWnd;
}


void CPIWindowBase::SetWndowSize( SIZE szSize )
{
	CPiWindowPack::ChangeSize(m_hWnd, szSize.cx, szSize.cy);

}

void CPIWindowBase::MoveWindow( int x, int y)
{
	CPiWindowPack::MoveWindow(m_hWnd, x, y);
}
