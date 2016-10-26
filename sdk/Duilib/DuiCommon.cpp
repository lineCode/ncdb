#include "StdAfx.h"
#include "DuiCommon.h"
#include "PiTypeDef.h"
#include "UIlib.h"

using namespace DuiLib;
CDuiCommon::CDuiCommon(CPaintManagerUI* pPaintManager /*= NULL*/)
{
	m_pPaintManager = pPaintManager;
}

CDuiCommon::~CDuiCommon(void)
{
}

bool CDuiCommon::IsStaticControl( CControlUI* pControl )
{
	ARR_STRING arrControl;
	arrControl.push_back(_T("HorizontalLayoutUI"));
	arrControl.push_back(_T("VerticalLayoutUI"));
	arrControl.push_back(_T("LabelUI"));

	tstring strClass = pControl->GetClass();
	return std::find(arrControl.begin(), arrControl.end(), strClass) != arrControl.end();
}

bool CDuiCommon::IsControl( CControlUI* pSender, tcpchar szName )
{
	return pSender->GetName() == szName;
}

void CDuiCommon::SetVisible( LPCTSTR param1, bool bShow )
{
	CControlUI* p = m_pPaintManager->FindControl(param1);
	if (p)
	{
		p->SetVisible(bShow);
	}
}
