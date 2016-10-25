
#include "stdafx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "XmlPathMgr.h"
#include "UCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
//
const CString SETTING_FOLDER		= _T("Config");
const CString SETTING_FILENAME		= _T("PA_Config.cfg");

const CString COLLECT_FOLDER		= _T("Collection");

const CString UPDATE_FOLDER			= _T("Update");
const CString UPDATE_FILENAME		= _T("PASoft_Update.ut");

const CString SOFT_SETTING_FOLDER	= _T("Config");
const CString SOFT_SETTING_FILENAME	= _T("PASoft_Config.cfg");

const CString VIDEO_FOLDER			= _T("AppData");
const CString VIDEO_FILENAME	    = _T("AppData.xml");

const CString USERDATA_FOLDER		= _T("UserData");
const CString USER_HEADJPG			= _T("head.jpg");

const CString DRIVER_FOLDER			= _T("Driver");

CString CXmlPathMgr::GetUserSettingFolder(BOOL bCreate/* = TRUE*/)
{
	CString strExeFld = CFolderUtil::GetPaUserMainFolder();
	CString strSettingFld = strExeFld + _T("\\") + SETTING_FOLDER;

	if (bCreate)
		CDirectoryUtil::CreateMultipleDirectory(strSettingFld);

	return strSettingFld;
}

CString CXmlPathMgr::GetUserSettingXmlPath()
{
	CString strXmlPath = GetUserSettingFolder() + _T("\\") + SETTING_FILENAME;
	return strXmlPath;
}

CString CXmlPathMgr::GetUserCollectXmlFolder(BOOL bCreate/* = TRUE*/)
{
	CString strPaUserMainFld = CFolderUtil::GetPaUserMainFolder();
	CString strCltFld = strPaUserMainFld + _T("\\") + COLLECT_FOLDER;

	if (bCreate)
		CDirectoryUtil::CreateMultipleDirectory(strCltFld);

	return strCltFld;
}

/*
CString CXmlPathMgr::GetUserCollectXmlPath()
{
    CString strDir = GetUserCollectXmlFolder() + _T("\\");
    CString strPath = 

    _stprintf_s(&strPath.at(0), MAX_PATH - 1, _T("%sDA_Collection_%s.clt"), strDir.GetBuffer()
        , GetCurrentMonthDBName().c_str());
}*/

CString CXmlPathMgr::GetUpdateFolder(BOOL bCreate/* = TRUE*/)
{
	CString strExeFld = CFolderUtil::GetExeFoloder();
	CString strUpdateFld = strExeFld + _T("\\") + UPDATE_FOLDER;

	if (bCreate)
		CDirectoryUtil::CreateMultipleDirectory(strUpdateFld);

	return strUpdateFld;
}

CString CXmlPathMgr::GetUpdateXmlPath()
{
	CString strUpdateXml = GetUpdateFolder() + _T("\\") + UPDATE_FILENAME;
	return strUpdateXml;
}

CString CXmlPathMgr::GetSoftSettingFolder( BOOL bCreate /*= TRUE*/ )
{

    CString strExeFld = CFolderUtil::GetPaUserMainFolder();
    CString strSettingFld = strExeFld + _T("\\") + SETTING_FOLDER;

    if (bCreate)
        CDirectoryUtil::CreateMultipleDirectory(strSettingFld);

    return strSettingFld;
}

CString CXmlPathMgr::GetSoftSettingXmlPath()
{
    return GetSoftSettingFolder() + _T("\\") +SOFT_SETTING_FILENAME;
}

CString CXmlPathMgr::GetVideoXmlFolder( BOOL bCreate /*= TRUE*/ )
{
    CString strPaUserMainFld = CFolderUtil::GetPaUserMainFolder();
    CString strFld = strPaUserMainFld + _T("\\") + VIDEO_FOLDER;
    if (bCreate)
        CDirectoryUtil::CreateMultipleDirectory(strFld);
    return strFld;
}

CString CXmlPathMgr::GetVideoXmlPath()
{
    return GetVideoXmlFolder() + _T("\\") + VIDEO_FILENAME;
}

CString CXmlPathMgr::GetUserDataFolder(BOOL bCreate/* = TRUE*/)
{
	CString strExeFld = CFolderUtil::GetPaUserMainFolder();
	CString strUserDataFld = strExeFld + _T("\\") + USERDATA_FOLDER;

	if (bCreate)
		CDirectoryUtil::CreateMultipleDirectory(strUserDataFld);

	return strUserDataFld;
}

CString CXmlPathMgr::GetUserHeadPath()
{
	CString strUseDataFld = GetUserDataFolder();
	CString strUserHeadPath = strUseDataFld + _T("\\") + USER_HEADJPG;

	return strUserHeadPath;
}

CString CXmlPathMgr::GetDriverFolder(LPCTSTR vid,LPCTSTR pid,BOOL bCreate)
{
	CString strTmpFld = CFolderUtil::GetPaUserMainFolder();
	CString strDriverFld = strTmpFld + _T("\\") + DRIVER_FOLDER;

	CString strVidPidFld = _T("VID_");
	strVidPidFld = strVidPidFld + vid + _T("&&PID_") + pid;

	strDriverFld = strDriverFld + _T("\\") + strVidPidFld;

	if(bCreate)
		CDirectoryUtil::CreateMultipleDirectory(strDriverFld);

	return strDriverFld;
}

CString CXmlPathMgr::GetDownloadTempFolder(BOOL bCreate/* = TRUE*/)
{
	CString strTempFld = CFolderUtil::GetPaTempFolder();
	CString strDownload = strTempFld + _T("\\Download");

	if(bCreate)
		CDirectoryUtil::CreateMultipleDirectory(strDownload);

	return strDownload;
}

CString CXmlPathMgr::GetUploadTempFolder(BOOL bCreate/* = TRUE*/)
{
	CString strTempFld = CFolderUtil::GetPaTempFolder();
	CString strUpload = strTempFld + _T("\\Upload");

	if(bCreate)
		CDirectoryUtil::CreateMultipleDirectory(strUpload);

	return strUpload;
}