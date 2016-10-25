#include "StdAfx.h"
#include "PiPoint.h"
#include "Math\mathUnit.h"

Pi_NameSpace_Using

CPiPoint::CPiPoint( PointDataType x /*= 0*/, PointDataType y /*= 0*/ )
:m_x(x)
,m_y(y)
{

}

CPiPoint::CPiPoint( POINT pt )
 :m_x(pt.x)
 ,m_y(pt.y)
{
	
}

CPiPoint::~CPiPoint(void)
{
}


ns_PiPi::CPiPoint::operator POINT()
{
	POINT pt = {m_x, m_y};
	return pt;
}

bool ns_PiPi::CPiPoint::IsVertical( const CPiPoint& ptDist )
{
	//return m_x == ptDist.m_x;
	return FlowEqual(m_x, ptDist.m_x);
}

bool ns_PiPi::CPiPoint::operator==( const CPiPoint& ptDist )
{
	/*return m_x == ptDist.m_x
		&& m_y == ptDist.m_y;*/
	return 	FlowEqual(m_x, ptDist.m_x)
		&& FlowEqual(m_y, ptDist.m_y);
}

bool ns_PiPi::CPiPoint::operator!=( const CPiPoint& ptDist )
{
	return !(*this == ptDist);
}

 