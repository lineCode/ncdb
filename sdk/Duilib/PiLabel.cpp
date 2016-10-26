#include "StdAfx.h"
#include "PiLabel.h"

CPiLabel::CPiLabel(void)
{
	m_hdc = NULL;
}

CPiLabel::~CPiLabel(void)
{
}

LPCTSTR CPiLabel::GetClass() const
{
	return _T("PiLabel");

}

LPVOID CPiLabel::GetInterface( LPCTSTR pstrName )
{
	if( _tcscmp(pstrName, _T("PiLabel")) == 0 ) 
		return static_cast<CPiLabel*>(this);
	return CLabelUI::GetInterface(pstrName);
}

void CPiLabel::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	__super::SetAttribute(pstrName, pstrValue);
}

/*
void CPiLabel::PaintStatusImage( HDC hDC)
{
	__super::PaintStatusImage(hDC);
}*/

void CPiLabel::Init()
{
	__super::Init();
}

void CPiLabel::DoPaint( HDC hDC, const RECT& rcPaint )
{
	if( !::IntersectRect( &m_rcPaint, &rcPaint, &m_rcItem ) ) return;
	//__super::DoPaint(hDC, rcPaint);
	m_hdc = hDC;
	DoFullPaint();

	return ;





	float nAngleBegin = 10 + 180;
	float nAngleEnd = 150 + 180;
	POINT ptC = {50, 50};

	/*for (int n = 0; n <= 360; n+= 10)
	{
		//DrawTest(nAngleBegin, ptC);
		//DrawTest(nAngleEnd, ptC);

		DrawPercent(0, nAngleBegin, nAngleEnd);
		DrawPercent(10, nAngleBegin, nAngleEnd);
		DrawPercent(20, nAngleBegin, nAngleEnd);
		DrawPercent(50, nAngleBegin, nAngleEnd);
		DrawPercent(40, nAngleBegin, nAngleEnd);
		DrawPercent(100, nAngleBegin, nAngleEnd);
		DrawPercent(80, nAngleBegin, nAngleEnd);
		//DrawTest(n);
	}*/

}

void CPiLabel::DrawTest( float nAngle, POINT ptCenter )
{
	
	Graphics graphics(m_hdc);
	Pen pen(Color(255, 255, 0, 0));
	graphics.RotateTransform(nAngle, MatrixOrderPrepend);  // then rotate
	graphics.TranslateTransform(ptCenter.x, ptCenter.y, MatrixOrderAppend);           // first translate
	graphics.DrawLine(&pen, 0, 0, 50, 0);
	graphics.ResetTransform();
}

void CPiLabel::DrawPercent( float nPercent, float nAngleBegin, float nAngleEnd )
{

	float nAnglePercent = (nAngleEnd - nAngleBegin) / 100.0f;
	float nAngleDraw = nAnglePercent * nPercent + nAngleBegin;
	POINT ptC = {50, 50};
	DrawTest(nAngleDraw, ptC);
}

bool CPiLabel::DoFullPaint()
{
	return true;
}
