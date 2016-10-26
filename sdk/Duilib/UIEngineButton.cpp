#include "StdAfx.h"
#include "UIEngineButton.h"
#include "Duilib\DuiHeader.h"


CEngineButton::CEngineButton(void)
{
}

CEngineButton::~CEngineButton(void)
{
}



void CEngineButton::DoEvent( TEventUI& event )
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

void CEngineButton::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
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
		CButtonUI::SetAttribute(pstrName, pstrValue);
	}

}

ENUM_ENGINE CEngineButton::GetButtomSimulatorType()
{
	return m_eSimulatorType;
}

LPCTSTR CEngineButton::GetClass() const
{
	return _T("CEngineButton");
}
