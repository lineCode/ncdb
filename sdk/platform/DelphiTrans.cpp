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

tcpchar CDelphiTrans::FilterToMFC(tcpchar szFilter)
{
	static tchar szFilterCommon[] = _T("All Files (*.*)|*.*||");
	static tcpchar pFilter = nullptr;
	if (pFilter)
	{
		delete[] pFilter;
	}
	/************************************************************************
	win32 格式:	All\0*.*\0图片\0*.jpg;*.png\0\0\0
	\0分割显示和类型， 2个一组， 分号分割多个多虑类型， 结尾3个\0

	delphi格式: 所有文件|*.*;图片|*.jpg,*.png;文档|*.doc,*.ppt
	竖线分割显示和类型， 分号分割每组，逗号分开多个过滤类型


	************************************************************************/
	//竖线 -> \0,   分号  -> \0,  逗号->分号,  结尾增加两个\0
	if (!szFilter || !*szFilter)
	{
		return szFilterCommon;
	}
	CPiString str;
	if (szFilter && *szFilter)
	{
		//delphi格式转成MFC
		str = szFilter;
		str.append(_T("||"));

		void* p = new TCHAR[str.size() + 1];
		memcpy_s(p, str.size() * sizeof(TCHAR), str.c_str(), str.size() * sizeof(TCHAR));
		pFilter = (tcpchar)p;
	}
	return pFilter;
}

tcpchar CDelphiTrans::FilterToWin32(tcpchar szFilter)
{
	static tchar szFilterCommon[] = _T("All\0*.*\0图片\0*.jpg;*.png\0\0\0");
	static tcpchar pFilter = nullptr;
	if (pFilter)
	{
		delete[] pFilter;
	}
	/************************************************************************
			win32 格式:	All\0*.*\0图片\0*.jpg;*.png\0\0\0
						\0分割显示和类型， 2个一组， 分号分割多个多虑类型， 结尾3个\0

			delphi格式: 所有文件|*.*;图片|*.jpg,*.png;文档|*.doc,*.ppt
						竖线分割显示和类型， 分号分割每组，逗号分开多个过滤类型


	************************************************************************/
	//竖线 -> \0,   分号  -> \0,  逗号->分号,  结尾增加两个\0
	if (!szFilter || !*szFilter)
	{
		return szFilterCommon;
	}
	CPiString str;
	if (szFilter && *szFilter)
	{
		//delphi格式转成MFC
		str = szFilter;
		str.ReplaceZero(_T("|"));
		str.ReplaceZero(_T(";"));
		str.Replace(_T(","), _T(";"));
		str.AppendZero(2);
		
		void* p = new TCHAR[str.size() + 1];
		memcpy_s(p, str.size() * sizeof(TCHAR), str.c_str(), str.size() * sizeof(TCHAR));
		pFilter = (tcpchar)p;
	}
	return pFilter;
}
