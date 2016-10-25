#pragma once
#include "PiTypeDef.h"
#include <Windows.h>

class CPiRect
{
public:
	CPiRect();
	CPiRect(const RECT& rt);
	CPiRect(long x, long y, long nWidth, long nHeight);
	~CPiRect(void);
public:
	POINT GetCenterPoint();
	POINT GetCenterPoint(SIZE szMove);
	operator RECT();
	operator RECT*();
	UNINT GetWidth();
	UNINT GetHeight();
	void SetPos(UNINT nX, UNINT nY);
	POINT GetPos();
public:
	RECT		m_rt;
};

typedef list<CPiRect>						LST_RECT;
typedef vector<CPiRect>						ARR_RECT;