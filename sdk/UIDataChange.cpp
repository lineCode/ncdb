#include "StdAfx.h"
#include "UIDataChange.h"
#include "doString.h"
#include "AutoType.h"
#include "functional.h"


CUIDataChange::CUIDataChange(void)
{
}


CUIDataChange::~CUIDataChange(void)
{
}


float CUIDataChange::GetNumAmount( StringCRef strTemp )
{
	if (strTemp.empty())
	{
		return 0;
	}
	tstring strR = strTemp;
	StrReplace(strR, _T(","), _T(""));
	//Ö»±£ÁôÊý×Ö
	//strR = strR.substr(0, strR.find(_T(".")));
	EraseNoDigit(strR);
	return CAutoType(strR);
}

tstring CUIDataChange::TermStr( StringCRef strData )
{
	if (strData.empty())
	{
		return strData;
	}
	tstring strDist(strData);
	/*StrReplace(strData, _T("\n"), _T(""));
	StrReplace(strData, _T("\r"), _T(""));*/
	//TermStrLeft(strData);
	TermStrLeft(strDist, _T(" \t\r\n"));
	//TermStrRight(strData);
	//TermStrRight(strData, _T("	"));
	TermStrRight(strDist, _T(" \t\r\n"));
	return strDist;
}

double CUIDataChange::PercentSignToDecimal( StringCRef strTemp )
{
	return (double)CAutoType(strTemp.substr(0, strTemp.length() - 1)) / 100.0;
}

double CUIDataChange::PercentToDecimal( StringCRef strTemp )
{
	return (double)CAutoType(strTemp) / 100.0;

}
