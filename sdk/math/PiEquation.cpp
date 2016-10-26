#include "StdAfx.h"
#include "PiEquation.h"

Pi_NameSpace_Using

CPiEquation::CPiEquation(void)
{
	m_bVertical = false;
	m_lineX = 0;
}

CPiEquation::~CPiEquation(void)
{
}

bool CPiEquation::SolveStraightLine( const tagSTRAIGHT_LINE_CONDITION& tagSL )
{
	if (tagSL.pt1.x == tagSL.pt2.x)	
	{
		//垂直线没有斜率
		m_bVertical = true;
		m_lineX = tagSL.pt1.x;
		return true;
	}
	m_factor_k = 1.0 * ( tagSL.pt2.y - tagSL.pt1.y)/ (tagSL.pt2.x - tagSL.pt1.x);
	m_factor_b = 1.0 * tagSL.pt1.y - m_factor_k * tagSL.pt1.x;
	return true;
}

float CPiEquation::GetStraightLineY( float x )
{
	//直线方程:y = kx + b
	if(m_bVertical)
	{
		assert( 0 && "逻辑错误, 垂直线无法根据横坐标获取纵坐标");
		return -1;
	}
	return m_factor_k * x + m_factor_b;
}

float CPiEquation::GetStraightLineX( float y )
{
	//直线方程:y = kx + b
	if(m_bVertical)
	{
		return m_lineX;
	}
	return (y - m_factor_b) / m_factor_k ;
}
