/***********************************************************************
 Copyright (c), 2012, Unicorn Tech. Co., Ltd.

 File name   : Directory.h
 Description : 获取系统目录 
 Version     : 1.0
 Author      : xuemingzhi
 Date        : 2014/08/14
 Other       :
 -----------------------------------------------------------------------
 History 1 
      Date   : 
      Author : 
      Modification : 
***********************************************************************/


#include "StdAfx.h"
#include <tchar.h>
#include <shlobj.h>
#pragma comment(lib,"shell32.lib")
#include "Directorys.h"

KFilePath CDirectorys::GetSysPredefineDir(int csidl)
{
	TCHAR lpszDefaultDir[MAX_PATH];
	TCHAR szDocument[MAX_PATH]={0};   
	memset(lpszDefaultDir,0,_MAX_PATH);   

	LPITEMIDLIST pidl=NULL;   
	SHGetSpecialFolderLocation(NULL,   csidl,   &pidl);   
	if   (pidl   &&   SHGetPathFromIDList(pidl,   szDocument))   
	{   
		GetShortPathName(szDocument,lpszDefaultDir,_MAX_PATH);   
	}
	return lpszDefaultDir;
}

KFilePath CDirectorys::GetCSPMaterialDB()
{
	KFilePath fp = GetSysPredefineDir(CSIDL_PERSONAL);
	fp += _T("Unicorn\\UDMCommon\\MaterialDB");
	return fp;
}

KFilePath CDirectorys::GetCSPMaterialInstall()
{
	KFilePath fp = GetSysPredefineDir(CSIDL_PERSONAL);
	fp += _T("Unicorn\\UDMCommon\\Material\\PaintAidSetup");
	return fp;
}

KFilePath CDirectorys::GetCSPMTPreSetupDirMain()
{
    //TODO:主目录和
    KFilePath fp = CDirectorys::GetExpandDir(_T("%ProgramData%"));
    fp += _T("\\Unicorn\\UDM\\Common\\Material\\Install");
    return fp;
    //_T("C:\\ProgramData\\Unicorn\\UDM\\Common\\Material\\Install");
    //return _T("");
}

KFilePath CDirectorys::GetCSPMTPreSetupDirBackup()
{
    KFilePath fp = GetSysPredefineDir(CSIDL_PERSONAL);
    fp += _T("Unicorn\\UDMCommon\\Material\\Install");
    return fp;
}

KFilePath CDirectorys::GetExpandDir( LPCTSTR szSrc )
{
    STLString strTempFld;
    strTempFld.resize(MAX_PATH + 1);
    //TCHAR szBuffer[MAX_PATH];
    ExpandEnvironmentStrings(szSrc, &strTempFld.at(0), MAX_PATH);
    return strTempFld.c_str();
}
