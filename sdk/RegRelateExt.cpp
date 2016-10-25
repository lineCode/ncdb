// RegRelateExt.cpp: implementation of the CRegRelateExt class. 
// add by liwanliang 2015-9-10, used for auto open with file extension file
////////////////////////////////////////////////////////////////////// 

#include "stdafx.h" 
//#include "LibMaker.h" 
// #include "RelateAPP.h" 
#include "RegRelateExt.h" 

#ifdef _DEBUG 
#undef THIS_FILE 
static char THIS_FILE[]=__FILE__; 
#define new DEBUG_NEW 
#endif 

////////////////////////////////////////////////////////////////////// 
// Construction/Destruction 
////////////////////////////////////////////////////////////////////// 

CRegRelateExt::CRegRelateExt() 
{ 
	/*m_RegClass = _T("Unicorn mtd File"); 
	m_strArchiveDesc = _T("Unicorn Archive File");*/
} 

bool CRegRelateExt::Check( tcpchar szFileExt, tcpchar szExePath, tcpchar szParam )
{ 
	bool bRelate = false;
	if( !szFileExt || !*szFileExt
		|| !szExePath || !*szExePath) 
	{
		return bRelate; 
	}
	CPiString strExt(_T("."));
	strExt += szFileExt;

	CRegKey reg;
	if( reg.Open(HKEY_CLASSES_ROOT, strExt, KEY_READ) != ERROR_SUCCESS) 
	{
		return bRelate; 
	}
	DWORD ValueLen = MAX_PATH; 
	TCHAR Value[MAX_PATH]; 
	if(reg.QueryValue(Value, _T(""), &ValueLen) != ERROR_SUCCESS) 
	{
		return bRelate; 
	}
	reg.Close(); 
	CPiString strRegClass = Value; 

	if(reg.Open(HKEY_CLASSES_ROOT, strRegClass + _T("\\shell\\open\\command"), KEY_READ) != ERROR_SUCCESS) 
	{
		return bRelate; 
	}

	ValueLen = MAX_PATH; 
	if(reg.QueryValue(Value, _T(""), &ValueLen) != ERROR_SUCCESS) 
	{
		return bRelate; 
	}
	reg.Close();
	CPiString CmdLine(Value); 

	
	if (szParam)
	{
		bRelate = CmdLine.EqualNoCase(MakeShellCmd(szExePath, szParam).c_str());
	}
	else
	{
		bRelate = CmdLine.Exist(szExePath);
	}
	return bRelate; 
} 

bool CRegRelateExt::Delete( tcpchar FileExt )
{ 
	if(!FileExt) 
		return false; 

	CPiString strExt(_T("."));
	strExt += FileExt;

	CRegKey reg;

	if(reg.Open(HKEY_CLASSES_ROOT, strExt, KEY_READ) != ERROR_SUCCESS) 
		return false; 

	DWORD ValueLen = MAX_PATH; 
	TCHAR Value[MAX_PATH]; 
	if(reg.QueryValue(Value, _T(""), &ValueLen) != ERROR_SUCCESS) 
		return false; 
	reg.Close(); 

	reg.Attach(HKEY_CLASSES_ROOT); 
	if(reg.RecurseDeleteKey(Value) != ERROR_SUCCESS) 
		return false; 

	if(reg.RecurseDeleteKey(strExt) != ERROR_SUCCESS) 
		return false; 

	return true; 
} 

bool CRegRelateExt::SetSelf( tcpchar szFileExt, tcpchar szShellParam /*= NULL*/, tcpchar szIcon /*= NULL*/ )
{
	TCHAR bufferExe[400] = {0};
	GetModuleFileName(NULL, bufferExe, 400);
	return Set(szFileExt, bufferExe, szShellParam, szIcon);
}
bool CRegRelateExt::Set( tcpchar szFileExt, tcpchar szRelateExe, tcpchar szShellParam /*= NULL*/, tcpchar szIcon /*= NULL*/ )
{ 
	if(!szFileExt || !*szFileExt) 
		return true; 
	if (m_RegClass.empty()
		|| m_strArchiveDesc.empty())
	{
		return false;
	}
	Delete(szFileExt);
	
	/*if (Check(szFileExt, szRelateExe, szShellParam))
	{
		return true;
	}*/
	CRegKey reg;
	//m_RelateExe = ShellCmd; 
	CPiString strSuffix(_T("."));
	strSuffix += szFileExt;

	CPiString strIcon(szRelateExe);
	strIcon += _T(",0");
	if(szIcon)
	{
		strIcon = szIcon;
	}
	tcpchar szRegClass = m_RegClass.c_str();
	Delete(szFileExt); 

	if(reg.Create(HKEY_CLASSES_ROOT, strSuffix) != ERROR_SUCCESS) 
		return false; 
	if(reg.SetValue(HKEY_CLASSES_ROOT, strSuffix, m_RegClass.c_str()) != ERROR_SUCCESS) 
		return false; 
	if(reg.Create(HKEY_CLASSES_ROOT, m_RegClass.c_str()) != ERROR_SUCCESS) 
		return false; 
	if(reg.SetValue(HKEY_CLASSES_ROOT, m_RegClass.c_str(), m_strArchiveDesc.c_str()) != ERROR_SUCCESS) 
		return false; 
	if(reg.SetValue((DWORD)0x08, _T("BrowserFlags")) != ERROR_SUCCESS) 
		return false; 
	if(reg.SetValue((DWORD)0x00, _T("EditFlags")) != ERROR_SUCCESS) 
		return false;

	if(reg.Create(HKEY_CLASSES_ROOT, m_RegClass + _T("\\DefaultIcon")) != ERROR_SUCCESS) 
		return false; 
	if(reg.SetValue(HKEY_CLASSES_ROOT, m_RegClass + _T("\\DefaultIcon"), strIcon) != ERROR_SUCCESS) 
		return false; 
	if(reg.Create(HKEY_CLASSES_ROOT, m_RegClass + _T("\\shell")) != ERROR_SUCCESS) 
		return false; 
	if(reg.Create(HKEY_CLASSES_ROOT, m_RegClass + _T("\\shell\\open")) != ERROR_SUCCESS) 
		return false; 
	if(reg.Create(HKEY_CLASSES_ROOT, m_RegClass + _T("\\shell\\open\\command")) != ERROR_SUCCESS) 
		return false; 
	CPiString strShellCmd; 
	strShellCmd = MakeShellCmd(szRelateExe, szShellParam);
	
	if(reg.SetValue(HKEY_CLASSES_ROOT,m_RegClass + _T("\\shell\\open\\command"), strShellCmd) != ERROR_SUCCESS) 
		return false; 

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	return true; 
} 

CRegRelateExt::~CRegRelateExt() 
{ 

} 

void CRegRelateExt::SetParam( tcpchar szRegClassName, tcpchar szArchiveDesc )
{
	m_RegClass = szRegClassName;
	m_strArchiveDesc = szArchiveDesc;
}

bool CRegRelateExt::CheckSelf( tcpchar szFileExt, tcpchar szParam )
{
	TCHAR bufferExe[400] = {0};
	GetModuleFileName(NULL, bufferExe, 400);
	return Check(szFileExt, bufferExe, szParam);
}

tstring CRegRelateExt::MakeShellCmd( tcpchar szRelateExe, tcpchar szParam )
{
	CPiString strShellCmd;
	tstring strShellParam;
	if(szParam && *szParam)
	{
		strShellParam = szParam;
	}
	strShellParam += _T(" \"%1\"");

	strShellCmd.Format(_T("%s %s"), szRelateExe, strShellParam.c_str());
	return strShellCmd;
}
