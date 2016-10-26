#pragma once
#include "Math\PiEquationDef.h"
Pi_NameSpace_Begin

class CPiEquation
{
public:
	CPiEquation(void);
	~CPiEquation(void);
public:
	bool SolveStraightLine(const tagSTRAIGHT_LINE_CONDITION& tagSL);
	float GetStraightLineX(float y);
	float GetStraightLineY(float x);
private:
	//直线方程:y = kx + b
	float			m_factor_k;
	float			m_factor_b;

	DataTypeMathCoordinate	m_lineX;	//直线 x = n;
	bool			m_bVertical;
};

Pi_NameSpace_End