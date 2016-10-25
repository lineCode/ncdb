#include "StdAfx.h"
#include "UIEngineGifAnim.h"
#include "Duilib\DuiHeader.h"


CEngineGifAnim::CEngineGifAnim(void)
{
}

CEngineGifAnim::~CEngineGifAnim(void)
{
}



void CEngineGifAnim::DoEvent( TEventUI& event )
{
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSEENTER,m_eSimulatorType);
	}
	if( event.Type == UIEVENT_MOUSELEAVE )
	{
		m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSELEAVE,m_eSimulatorType);
	}
	return __super::DoEvent(event);
}

void CEngineGifAnim::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	 
	if( _tcscmp(pstrName, _T("enginetype")) == 0 ) 
	{
		if ( _tcscmp(pstrValue, _T("haozhuo")) == 0)
		{
			m_eSimulatorType = eg_bs;
		}
		else if ( _tcscmp(pstrValue, _T("tiantian")) == 0)
		{
			m_eSimulatorType = eg_tt;
		}
	}
	else
	{
		CGifAnimUI::SetAttribute(pstrName, pstrValue);
	}

}

ENUM_ENGINE CEngineGifAnim::GetButtomSimulatorType()
{
	return m_eSimulatorType;
}

LPCTSTR CEngineGifAnim::GetClass() const
{
	return _T("CEngineGifAnim");
}
