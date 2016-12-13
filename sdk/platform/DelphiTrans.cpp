#include "stdafx.h"
#include "DelphiTrans.h"
#include "PiString.h"
Pi_NameSpace_Using

CDelphiTrans::CDelphiTrans()
{
}


CDelphiTrans::~CDelphiTrans()
{
}

tstring CDelphiTrans::FilterToMFC(tcpchar szFilter)
{
	if (!szFilter || !*szFilter)
	{
		return _T("All Files (*.*)|*.*||");
	}
	CPiString str;
	if (szFilter && *szFilter)
	{
		//delphi格式转成MFC
		str = szFilter;
		//str.Replace(_T(";"), _T("|"));
		//str.Replace(_T(","), _T(";"));
		str.append(_T("||"));
	}
	return str;
}
