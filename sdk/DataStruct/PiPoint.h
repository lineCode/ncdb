#pragma once
#include "PiTypeDef.h"

Pi_NameSpace_Begin


class CPiPoint
{
public:
	typedef float PointDataType;

	CPiPoint(PointDataType x = 0, PointDataType y = 0);
	CPiPoint(POINT pt);
	~CPiPoint(void);
public:
	operator POINT();
	bool operator == ( const CPiPoint& ptDist);
	bool operator != ( const CPiPoint& ptDist);
	bool IsVertical( const CPiPoint& ptDist );
	//成员数据
public:
	PointDataType m_x;
	PointDataType m_y;
};

Pi_NameSpace_End
