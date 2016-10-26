#include "stdafx.h"
#include "UIOptionEx.h"
#include "..\..\DuiLib\UIlib.h"
IMPLEMENT_DUICONTROL(COptionUIEx)
/////////////////////////////////////////////////////////////////////////////////////
//
//

COptionUIEx::COptionUIEx()
: COptionUI()
, IsDrag(false)
{
}

COptionUIEx::~COptionUIEx()
{
}

bool COptionUIEx::IsDraging()
{
	return IsDrag;
}

void COptionUIEx::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
	{
		if (::PtInRect(&m_rcItem, event.ptMouse))
		{
			m_uButtonState |= UISTATE_CAPTURED;
			m_ptLastMouse = event.ptMouse;
			m_rcNewPos = m_rcItem;

			if (m_pManager)
				m_pManager->AddPostPaint(this);
			return;
		}
	}
	if (event.Type == UIEVENT_BUTTONUP)
	{
		if ((m_uButtonState & UISTATE_CAPTURED) != 0)
		{
			m_uButtonState &= ~UISTATE_CAPTURED;
			if (m_pManager)
			{
				m_pManager->RemovePostPaint(this);
				m_pManager->Invalidate(m_rcNewPos);
			}
			NeedParentUpdate();
			if (::PtInRect(&m_rcItem, event.ptMouse)) Activate();
			TNotifyUI msg;
			msg.sType = DUI_MSGTYPE_BUTTONUP;
			msg.pSender = this;
			m_pManager->SendNotify(msg);
			IsDrag = false;
			return;
		}
	}
	if (event.Type == UIEVENT_MOUSEMOVE)
	{
		if ((m_uButtonState & UISTATE_CAPTURED) != 0)
		{
			IsDrag = true;
			LONG cx = event.ptMouse.x - m_ptLastMouse.x;
			LONG cy = event.ptMouse.y - m_ptLastMouse.y;

			m_ptLastMouse = event.ptMouse;

			RECT rcCurPos = m_rcNewPos;

			rcCurPos.left += cx;
			rcCurPos.right += cx;
			rcCurPos.top += cy;
			rcCurPos.bottom += cy;

			//将当前拖拽块的位置 和 当前拖拽块的前一时刻的位置，刷新
			CDuiRect rcInvalidate = m_rcNewPos;
			m_rcNewPos = rcCurPos;
			rcInvalidate.Join(m_rcNewPos);
			if (m_pManager) m_pManager->Invalidate(rcInvalidate);
			return;
		}
	}
	if (event.Type == UIEVENT_MOUSEENTER)
	{
		if (IsEnabled()) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
			TNotifyUI msg;
			msg.sType = DUI_MSGTYPE_MOUSEENTER;
			msg.pSender = this;
			m_pManager->SendNotify(msg);
		}
	}
	if (event.Type == UIEVENT_DBLCLICK)
		return;
	//if (event.Type == UIEVENT_SETCURSOR) {
	//	HCURSOR curStraw = CCachData::getInstance()->GetCurStraw();
	//	if (curStraw)
	//		::SetCursor(curStraw);
	//	return;
	//}

	COptionUI::DoEvent(event);
}
void COptionUIEx::DoPostPaint(HDC hDC, const RECT& rcPaint)
{
	if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
		CDuiRect rcParent = m_pParent->GetPos();
		RECT rcUpdate = { 0 };
		rcUpdate.left = m_rcNewPos.left < rcParent.left ? rcParent.left : m_rcNewPos.left;
		rcUpdate.top = m_rcNewPos.top < rcParent.top ? rcParent.top : m_rcNewPos.top;
		rcUpdate.right = m_rcNewPos.right > rcParent.right ? rcParent.right : m_rcNewPos.right;
		rcUpdate.bottom = m_rcNewPos.bottom > rcParent.bottom ? rcParent.bottom : m_rcNewPos.bottom;
		if (!m_sPushedImage.IsEmpty()){
			DrawImage(hDC, (LPCTSTR)m_sPushedImage);
			CRenderEngine::DrawImageString(hDC, m_pManager, rcUpdate, rcUpdate, (LPCTSTR)m_sPushedImage, NULL, m_instance);
		}
		else
			CRenderEngine::DrawColor(hDC, rcUpdate, 0xAA000000);
		
	}
}

void COptionUIEx::SetPos(RECT rc, bool bNeedInvalidate)
{
	if (rc.right < rc.left) rc.right = rc.left;
	if (rc.bottom < rc.top) rc.bottom = rc.top;

	CDuiRect invalidateRc = m_rcItem;
	if (::IsRectEmpty(&invalidateRc)) invalidateRc = rc;

	if (m_bFloat) {
		CControlUI* pParent = GetParent();
		if (pParent != NULL) {
			RECT rcParentPos = pParent->GetPos();
			RECT rcCtrl = { rcParentPos.left + rc.left, rcParentPos.top + rc.top,
				rcParentPos.left + rc.right, rcParentPos.top + rc.bottom };
			m_rcItem = rcCtrl;

			LONG width = rcParentPos.right - rcParentPos.left;
			LONG height = rcParentPos.bottom - rcParentPos.top;
			RECT rcPercent = { (LONG)(width*m_piFloatPercent.left), (LONG)(height*m_piFloatPercent.top),
				(LONG)(width*m_piFloatPercent.right), (LONG)(height*m_piFloatPercent.bottom) };
			m_cXY.cx = rc.left - rcPercent.left;
			m_cXY.cy = rc.top - rcPercent.top;
			m_cxyFixed.cx = rc.right - rcPercent.right - m_cXY.cx;
			m_cxyFixed.cy = rc.bottom - rcPercent.bottom - m_cXY.cy;
		}
	}
	else {
		m_rcItem = rc;
	}
	if (m_pManager == NULL) return;

	if (!m_bSetPos) {
		m_bSetPos = true;
		if (OnSize) OnSize(this);
		m_bSetPos = false;
	}

	m_bUpdateNeeded = false;

	if (bNeedInvalidate && IsVisible()) {
		invalidateRc.Join(m_rcItem);
		CControlUI* pParent = this;
		RECT rcTemp;
		RECT rcParent;
		while (pParent = pParent->GetParent()) {
			if (!pParent->IsVisible()) return;
			rcTemp = invalidateRc;
			rcParent = pParent->GetPos();
			if (!::IntersectRect(&invalidateRc, &rcTemp, &rcParent)) return;
		}
		m_pManager->Invalidate(invalidateRc);
	}
}