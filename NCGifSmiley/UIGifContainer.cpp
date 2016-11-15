#include "StdAfx.h"
#include "UIGifContainer.h"
#include "UIGifAnimEx.h"

IMPLEMENT_DUICONTROL(CGifContainerUI)
CGifContainerUI::CGifContainerUI()
{
}

void CGifContainerUI::DoEvent(TEventUI& event)
{

	if (event.Type == UIEVENT_MOUSEENTER)
	{	
		point = event.ptMouse;
		m_pManager->SetTimer(this, MOUSE_IN_TIMER, 100);
	}
	else if (event.Type == UIEVENT_BUTTONUP)
	{
		CControlUI* pSubCtrl = this->FindSubControl(_T("gifBtn"));
		TEventUI event = { 0 };
		event.Type = UIEVENT_BUTTONUP;
		pSubCtrl->Event(event);
		return;
	}
	if (event.Type == UIEVENT_TIMER)
	{
		if ((UINT_PTR)event.wParam == MOUSE_IN_TIMER)
			MouseInTimer((UINT_PTR)event.wParam);
	}
	CVerticalLayoutUI::DoEvent(event);
}

void CGifContainerUI::DoInit()
{
	CVerticalLayoutUI::DoInit();
	//
}
void CGifContainerUI::MouseInTimer(UINT_PTR idEvent)
{
	if (idEvent != MOUSE_IN_TIMER)
		return;
	m_pManager->KillTimer(this, MOUSE_IN_TIMER);

	CDuiPoint pt;
	::GetCursorPos(&pt);
	ScreenToClient(GetManager()->GetPaintWindow(),&pt);
	pt.x -= 4;
	pt.y -= 4;
	RECT rc = GetPos();
	CGifAnimUIEx* pSubCtrl = static_cast<CGifAnimUIEx*>(this->FindSubControl(_T("gifBtn")));
	TEventUI event = { 0 };
	if (pt.x > rc.left&&pt.x<rc.right&&pt.y>rc.top&&pt.y < rc.bottom){
		event.Type = UIEVENT_MOUSEENTER;
		m_pManager->SetTimer(this, MOUSE_IN_TIMER, 100);
	}	
	else 
		event.Type = UIEVENT_MOUSELEAVE;
	event.wParam = 1;
	//if (pSubCtrl->gifHeight < HEIGHT_RECT || pSubCtrl->gifWidth < WIDTH_RECT)
	pSubCtrl->Event(event);

}