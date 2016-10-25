#pragma once

class CXmlPathMgr
{
public:
	static CString GetUserSettingFolder(BOOL bCreate = TRUE);				// 用户配置目录
	static CString GetUserSettingXmlPath();				// 用户配置XML

	static CString GetUserCollectXmlFolder(BOOL bCreate = TRUE);			// 用户收集目录

	static CString GetUpdateFolder(BOOL bCreate = TRUE);	// 程序更新配置目录
	static CString GetUpdateXmlPath();					// 程序更新配置XML

    static CString GetSoftSettingFolder(BOOL bCreate = TRUE);	// 软件配置目录
    static CString GetSoftSettingXmlPath();					// 软件配置XML

    static CString GetVideoXmlFolder(BOOL bCreate = TRUE);					// 录像配置目录
    static CString GetVideoXmlPath();										// 录像配置XML

	static CString GetUserDataFolder(BOOL bCreate = TRUE);					// 获取用户相关数据目录
	static CString GetUserHeadPath();										// 获取用户头像路径

	static CString GetDriverFolder(LPCTSTR vid,LPCTSTR pid,BOOL bCreate = FALSE);// 获取驱动目录

	static CString GetDownloadTempFolder(BOOL bCreate = TRUE);				// %TEMP%/Unicorn/PaintAid/Download
	static CString GetUploadTempFolder(BOOL bCreate = TRUE);				// %TEMP%/Unicorn/PaintAid/Upload
};