#pragma once
#include "Control\UIText.h"
/*------------------------------------------------------
	Fun:	EstimateSize(CPiSize(9999,9999));
	Authon: liwanliang
	memo:	如果需要计算出绘制结果的区域大小, 调用EstimateSize(CPiSize(9999,9999));后绘制大小保存在pos里
	Date:	2016/03/01
------------------------------------------------------*/
class CPiUIText :
	public CTextUI
{
public:
	CPiUIText(void);
	~CPiUIText(void);
public:
	virtual SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintText(HDC hDC);
private:
	bool			m_bAutoSize;
};
