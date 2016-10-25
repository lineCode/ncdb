#include "StdAfx.h"
#include "SysFilePathMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/************************************************************************/
/* 
VC 获取系统特殊文件夹的路径如：系统目录，桌面等

qtom:
TCHAR MyDir[_MAX_PATH];  
SHGetSpecialFolderPath(this->GetSafeHwnd(),MyDir,CSIDL_APPDATA,0);
AfxMessageBox(MyDir);
其中第三个参数CSIDL_APPDATA可以为以下的值：

CSIDL_BITBUCKET 回收站
CSIDL_CONTROLS 控制面板
CSIDL_DESKTOP Windows 桌面Desktop
CSIDL_DESKTOPDIRECTORY Desktop的目录
CSIDL_DRIVES 我的电脑
CSIDL_FONTS 字体目录
CSIDL_NETHOOD 网上邻居
CSIDL_NETWORK 网上邻居虚拟目录
CSIDL_PERSONAL 我的文档
CSIDL_PRINTERS 打印机
CSIDL_PROGRAMS 程序组
CSIDL_RECENT 最近打开的文档
CSIDL_SENDTO “发送到”菜单项
CSIDL_STARTMENU 任务条启动菜单项
CSIDL_STARTUP 启动目录
CSIDL_TEMPLATES 文档模板
*/
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
//

tstring CSysFilePathMgr::GetProfileDocumentPath()
{
// 	tstring strProfileDocumentPath;
// 
// 	TCHAR szDocument[MAX_PATH] = {0};
// 	memset(szDocument, 0, sizeof(szDocument));
// 
// 	LPITEMIDLIST pidl = NULL;
// 	SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl);
// 	if (pidl && SHGetPathFromIDList(pidl, szDocument))
// 	{
// 		strProfileDocumentPath = szDocument;
// 	}
// 
// 	return strProfileDocumentPath;
	return GetSysSpecialFolder(CSIDL_PERSONAL);
}

tstring CSysFilePathMgr::GetSysFontFolder()
{
	return GetSysSpecialFolder(CSIDL_FONTS);
}

tstring CSysFilePathMgr::GetSysSpecialFolder(int csidl)
{
	tstring strProfileDocumentPath;

	TCHAR szDocument[MAX_PATH] = {0};
	memset(szDocument, 0, sizeof(szDocument));

	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, csidl, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		strProfileDocumentPath = szDocument;
	}

	return strProfileDocumentPath;
}

tstring CSysFilePathMgr::GetSysTempFolder()
{
// 	tstring strExpand = _T("%TEMP%");
// 
// 	tstring strTempFld;
// 	ExpandEnvironmentStrings(strExpand, strTempFld.GetBuffer(MAX_PATH), MAX_PATH);
// 
//	return strTempFld;

	tstring strTempFld;

	tstring strExpand(_T("%TEMP%"));
	TCHAR szBuffer[MAX_PATH];
	ExpandEnvironmentStrings(strExpand.c_str(), szBuffer, MAX_PATH);
	strTempFld = szBuffer;

	return strTempFld;
}

tstring CSysFilePathMgr::ExpandString(tstring strOriginal, HMODULE hModule)
{
	const int macro_prefix_length = 2;
	const int macro_subfix_length = 1;

	tstring expanded;
	for (;;)
	{
		int start_pos(strOriginal.find(_T("$(")));
		if (-1 == start_pos)
		{
			expanded = strOriginal;
			break;
		}

		int end_pos(strOriginal.find(_T(")"), start_pos));
		if (-1 == end_pos)
		{
			expanded = strOriginal;
			break;;
		}

		tstring macro(strOriginal.substr(start_pos+macro_prefix_length, end_pos-start_pos-macro_prefix_length));
		expanded = strOriginal.substr(0, start_pos);

		/*if (0 == macro.Compare(_T("ProgramDir")))
		{
		}*/
	}

	return _T("");
}

tstring CSysFilePathMgr::GetUserAppData()
{
	return GetSysSpecialFolder(CSIDL_APPDATA);
}

tstring CSysFilePathMgr::GetDesktop()
{
	return GetSysSpecialFolder(CSIDL_DESKTOP);
}
