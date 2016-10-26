#pragma once
#include "PiTypeDef.h"
class CSysFilePathMgr
{
public:
	static tstring GetProfileDocumentPath();			// 获得“我的文档”路径
 	static tstring GetSysTempFolder();					// 获得系统TEMP目录路径
//	static tstring GetSysTempFolder();					// 获得系统TEMP目录路径
	static tstring GetSysFontFolder();					// 获得系统Font目录路径
	static tstring GetUserAppData();					//获取APPDATA目录
	static tstring GetDesktop();					//获取APPDATA目录
protected:
	static tstring ExpandString(tstring strOriginal, HMODULE hModule = NULL);

	static tstring GetSysSpecialFolder(int csidl);				// 获取系统特殊文件夹的路径如：系统目录，桌面等
};