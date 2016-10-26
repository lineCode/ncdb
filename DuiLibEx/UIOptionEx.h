#ifndef __UIOPTIONEX_H__
#define __UIOPTIONEX_H__

#pragma once
#define DUI_MSGTYPE_BUTTONUP 		   	   (_T("buttonup"))
class  COptionUIEx : public COptionUI
{
	DECLARE_DUICONTROL(COptionUIEx)
public:
	COptionUIEx();
	virtual ~COptionUIEx();

	virtual void DoEvent(TEventUI& event);
	virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
	bool IsDraging();
protected:

private:
	CDuiRect m_rcNewPos;
	CDuiPoint m_ptLastMouse;
	bool IsDrag;
};


#endif //__UIBUTTONEX_H__