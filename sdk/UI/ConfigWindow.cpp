#include "StdAfx.h"
#include "ConfigWindow.h"
#include "PathLight.h"
#include "define_gnl.h"
#include "PiString.h"

tcpchar STRING_INI_SECTION_WINDOW				= _T("wnd");
tcpchar STRING_INI_DEFAULT_FILE_NAME			= _T("configWnd.ini");
tcpchar STRING_INI_DATA_SEPERATOR				= _T("|");

Pi_NameSpace_Using

CConfigWindow::CConfigWindow( tcpchar szConfigPath /*= NULL*/ )
{
	tstring strConfigPath(CPathLight(CPathLight::GetSelfModuleFolder()) + STRING_INI_DEFAULT_FILE_NAME);
	if (szConfigPath && *szConfigPath)
	{
		strConfigPath = szConfigPath;
	}
	FileName(strConfigPath.c_str());
	Init();
}

CConfigWindow::~CConfigWindow(void)
{
	
}
bool CConfigWindow::GetWindowList( LST_WINDOW& lstWnd )
{
	LST_STRING lstKey;
	lstKey = GetAllKey(STRING_INI_SECTION_WINDOW);
	tstring strWindowDesc;
	FOR_EACH_ITER(LST_STRING, lstKey, it)
	{
		strWindowDesc = ReadParamString(STRING_INI_SECTION_WINDOW, it->c_str());
		AnalyDesc(strWindowDesc, lstWnd);
	}
	return true;
}

bool CConfigWindow::AnalyDesc( const tstring& strWindowDesc, LST_WINDOW& lstWnd )
{
	if (strWindowDesc.empty())
	{
		return true;
	}
	// 以分号为分符
	ARR_STRING arrData;
	arrData = CPiString::SpilitStrToArray(strWindowDesc, STRING_INI_DATA_SEPERATOR);
	lstWnd.push_back(tagWindow(arrData[0], arrData[1]));
	return true;
}
