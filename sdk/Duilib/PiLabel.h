#pragma once
#include "Control\UILabel.h"

class CPiLabel :
	public CLabelUI
{
public:
	CPiLabel(void);
	~CPiLabel(void);
public:
	virtual LPCTSTR	GetClass() const;
	virtual LPVOID	GetInterface(LPCTSTR pstrName);
	virtual void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	virtual void Init();
	//void PaintStatusImage( HDC hDC);
	virtual void DoPaint(HDC hDC, const RECT& rcPaint);
	virtual bool DoFullPaint();
	void DrawTest( float nAngle, POINT ptCenter );
	void DrawPercent( float nPercent, float nAngleBegin, float nAngleEnd );
protected:
	HDC				m_hdc;
};
