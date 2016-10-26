#pragma once
#include "pilabel.h"

class CDuiLblPaint :
	public CPiLabel
{
public:
	CDuiLblPaint(void);
	~CDuiLblPaint(void);
public:
	virtual bool DoFullPaint();
	void AdjustPercent();
	bool ShowScore( int nScore );
private:
	int				m_nPercentDraw;		//当前要绘制的百分比
};
