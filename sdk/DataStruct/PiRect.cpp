#include "StdAfx.h"
#include "PiRect.h"

CPiRect::CPiRect(const RECT& rt )
{
	m_rt = rt;
}

CPiRect::CPiRect( long x, long y, long nWidth, long nHeight )
{
	m_rt.left	= x;
	m_rt.top	= y;
	m_rt.right	= m_rt.left + nWidth;
	m_rt.bottom	= m_rt.top + nHeight;
}

CPiRect::CPiRect()
{
	memset(&m_rt, 0, sizeof(m_rt));
		
}

CPiRect::~CPiRect(void)
{
}

POINT CPiRect::GetCenterPoint()
{
	POINT ptCenter = {0};
	ptCenter.x = m_rt.right - (m_rt.right - m_rt.left)/2;
	ptCenter.y = m_rt.bottom - (m_rt.bottom - m_rt.top)/2;
	return ptCenter;
}

POINT CPiRect::GetCenterPoint( SIZE szMove )
{
	POINT ptCenter = GetCenterPoint();
	POINT ptPos = {0};
	ptPos.x = ptCenter.x - szMove.cx/2;
	ptPos.y =ptCenter.y - szMove.cy/2;
	return ptPos;
}

CPiRect::operator RECT()
{
	return m_rt;
}

UNINT CPiRect::GetWidth()
{
	return m_rt.right - m_rt.left;
}

UNINT CPiRect::GetHeight()
{
	return m_rt.bottom - m_rt.top;
}

void CPiRect::SetPos( UNINT nX, UNINT nY )
{
	::OffsetRect(&m_rt, nX - m_rt.left, nY - m_rt.top);
}

POINT CPiRect::GetPos()
{
	POINT pt = {m_rt.left, m_rt.top};
	return pt;
}

CPiRect::operator RECT*()
{
	return &m_rt;
}
