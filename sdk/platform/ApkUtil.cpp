#include "StdAfx.h"
#include "ApkUtil.h"
#include "System\PiKernel.h"
#include "StrCoding.h"
#include "PathLight.h"
#include "PiZip.h"

#define APM_AAPT CPathLight::GetSelfModuleFolder() + _T("Tool\\sy_aapt.exe")
CApkUtil::CApkUtil(void)
{
}

CApkUtil::~CApkUtil(void)
{
}

bool CApkUtil::GetApkInfo( tcpchar szApkPath, tagAPK_INFO& tagAI )
{
	tstring strApk(szApkPath);
	string sA;
	tstring strW = s2t(sA);

	tstring strCmd;
	tstringstream oss;
	oss << '\"' << APM_AAPT << '\"' << _T(" d badging \"") << szApkPath << '"';
	strCmd = oss.str();
	//strCmd = '\"' + APM_AAPT + '\"' + _T(" d badging \"") + szApkPath + '"';

	string strOut;
	//OutputDebugString(_T("开始获取apk信息"));
	if(!CPiKernel::ExecuteDos(strCmd.c_str(), strOut, 10000))
	{
		return false;
	}
	//OutputDebugString(_T("获取apk信息完成"));

	tstring strTemp(CStrCoding::Utf8ToUnicode(strOut.c_str()));
	//OutInfo(_T("--lwl src:"), strTemp);
	tstring pkgName = _T("package: name='");
	tstring verCode = _T("versionCode='");
	tstring version = _T("versionName='");
	tstring fileame = _T("application: label='");
	tstring logo = _T("icon='");
	tstring activity = _T("launchable-activity: name='");
	int posBegin = strTemp.find(_T(""));

	if (posBegin >=0)
	{
		posBegin = posBegin + pkgName.length();
		int posend = strTemp.find('\'', posBegin);
		tagAI.packname = strTemp.substr(posBegin, posend-posBegin);
	}
	posBegin = strTemp.find(verCode);
	{
		posBegin = posBegin + verCode.length();
		int posend = strTemp.find('\'', posBegin);
		tagAI.nCode = _ttoi(strTemp.substr(posBegin,posend-posBegin).c_str());
	}
	posBegin = strTemp.find(version);
	if (posBegin >=0)
	{
		posBegin = posBegin + version.length();
		int posend = strTemp.find('\'', posBegin);
		tagAI.version = strTemp.substr(posBegin,posend-posBegin);
	}
	posBegin = strTemp.find(fileame);
	{
		posBegin = posBegin + fileame.length();
		int posend = strTemp.find('\'', posBegin);
		tagAI.filename = strTemp.substr(posBegin,posend-posBegin);
	}
	posBegin = strTemp.find(logo);
	{
		posBegin = posBegin + logo.length();
		int posend = strTemp.find('\'', posBegin);
		tagAI.logoPath = strTemp.substr(posBegin,posend-posBegin);
	}
	posBegin = strTemp.find(activity);
	{
		posBegin = posBegin + activity.length();
		int posend = strTemp.find('\'', posBegin);
		tagAI.acvivity = strTemp.substr(posBegin,posend-posBegin);
	}
	return true;
}

bool CApkUtil::GetApkInfoAndPic( tcpchar szApkPath, tagAPK_INFO& tagAI, tcpchar szPicDir )
{
	if(!GetApkInfo(szApkPath, tagAI))
	{
		return false;
	}

	if(!CPiZip::UnZip(szApkPath, szPicDir, tagAI.logoPath.c_str()))
	{
		return false;
	}
	CPathLight path(tstring(szPicDir) + _T("\\") + CPathLight::GetFileNameFromPath(tagAI.logoPath.c_str()));
	path.RenameTitle(tagAI.packname);
	tagAI.logoPath = (tcpchar)path;
	
	return true;
}
