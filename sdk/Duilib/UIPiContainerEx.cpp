#include "StdAfx.h"
#include "UIPiContainerEx.h"
#include "Duilib\DuiHeader.h"

CPiContainerExUI::CPiContainerExUI(void)
{
	m_uButtonState = 0;
}

CPiContainerExUI::~CPiContainerExUI(void)
{
}

void CPiContainerExUI::SetHotImage( LPCTSTR pStrImage )
{
	if( m_diHot.sDrawString == pStrImage && m_diHot.pImageInfo != NULL ) return;
	m_diHot.Clear();
	m_diHot.sDrawString = pStrImage;
	Invalidate();
}

void CPiContainerExUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	int n = 0;
	if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
	else
	{
		CContainerUI::SetAttribute(pstrName, pstrValue);
	}
}

void CPiContainerExUI::DoEvent( TEventUI& event )
{
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		//::SendMessage(m_pParent->GetManager()->GetPaintWindow(), UM_CHILD_MOUSE_ENTER, (WPARAM)&event, 0);
		if( IsEnabled() ) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSEENTER);
		// return;
	}
	if( event.Type == UIEVENT_MOUSELEAVE )
	{
		if( IsEnabled() ) {
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSELEAVE);
		// return;
	}
}

void CPiContainerExUI::PaintStatusImage( HDC hDC )
{
	if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		//if( !m_sHotImage.IsEmpty() ) {
			DrawImage(hDC, m_diHot);
		//}
	}
}

LPCTSTR CPiContainerExUI::GetClass() const
{
		return _T("CPiContainerExUI");
}
