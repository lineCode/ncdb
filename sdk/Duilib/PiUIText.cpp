#include "StdAfx.h"
#include "PiUIText.h"

CPiUIText::CPiUIText(void)
{
	m_bAutoSize = false;
}

CPiUIText::~CPiUIText(void)
{
}

void CPiUIText::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	int n = 0;
	if( _tcscmp(pstrName, _T("autosize")) == 0 ) m_bAutoSize = (_tcsicmp(pstrValue, _T("true")) == 0);
	else
	{
		CTextUI::SetAttribute(pstrName, pstrValue);
	}
}

void CPiUIText::PaintText( HDC hDC )
{
	UINT uTextStyleCalc = m_uTextStyle;
	if (m_bAutoSize)
	{
		uTextStyleCalc |= DT_CALCRECT;
	}

	if( m_sText.IsEmpty() ) {
		m_nLinks = 0;
		return;
	}
 
	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	if( m_sText.IsEmpty() ) return;

	m_nLinks = _countof(m_rcLinks);
	RECT rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;
	if( IsEnabled() ) {
		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
			m_rcLinks, m_sLinks, m_nLinks, m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
			m_iFont, m_uTextStyle);
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwTextColor, \
			m_iFont, uTextStyleCalc);
	}
	else {
		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
			m_rcLinks, m_sLinks, m_nLinks, m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
			m_iFont, m_uTextStyle);
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor, \
			m_iFont, uTextStyleCalc);
	}

	if (m_bAutoSize)
	{
		m_rcItem.right = rc.right + m_rcTextPadding.right;
		m_rcItem.bottom = rc.bottom + m_rcTextPadding.bottom;
		SetPos(m_rcItem, false);
	}
}

SIZE CPiUIText::EstimateSize( SIZE szAvailable )
{
	SIZE sz = __super::EstimateSize(szAvailable);
	RECT rt = {GetFixedXY().cx, GetFixedXY().cy};
	rt.right = rt.left + sz.cx;
	rt.bottom = rt.top + sz.cy;
	SetPos(rt, false);
	return sz;
}

