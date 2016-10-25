#include "StdAfx.h"
#include "DuiLblPaint.h"
#include "DemonDef.h"

#define NUM_CIRCLE_DRAW	80
#define NUM_CHANGE_VALUE 2

CDuiLblPaint::CDuiLblPaint(void)
{
	m_nPercentDraw		= 0;	
}

CDuiLblPaint::~CDuiLblPaint(void)
{
}

bool CDuiLblPaint::DoFullPaint()
{
	m_hdc;
	m_rcPaint;
	Image* img = NULL;
	img	=	CRenderEngine::LoadGifImage(STRINGorID(m_sBkImage),0, 0);

	AdjustPercent();
	
	int nCoverHeight = (100 - m_nPercentDraw) /100.0 * img->GetHeight();
	Graphics gp(m_hdc); 
	gp.DrawImage(img, m_rcPaint.left, m_rcPaint.top + nCoverHeight
		, m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top - nCoverHeight);

	delete img;
	return true;
}

void CDuiLblPaint::AdjustPercent()
{
	if (m_nPercentDraw < 0)
	{
		m_nPercentDraw = 0;
	}
	else if(m_nPercentDraw > 100)
	{
		m_nPercentDraw = 100;
	}
}

bool CDuiLblPaint::ShowScore( int nScore )
{
	m_nPercentDraw = nScore;
	Invalidate();
	return true;
}
