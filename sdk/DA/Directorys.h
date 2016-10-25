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

#include "k_filepath.h"

/* 目录集合类
     支持快捷获取需要的目录，无需在做繁琐的组装工作
*/
class CDirectorys
{
public:
	// [Unicorn]:CSP自定义素材数据库存放目录
	static KFilePath GetCSPMaterialDB();
	// [Unicorn]:CSP自定义素材存放目录
	static KFilePath GetCSPMaterialInstall();   
    static KFilePath GetCSPMTPreSetupDirMain();
    static KFilePath GetCSPMTPreSetupDirBackup();
    static KFilePath GetExpandDir(LPCTSTR szSrc);	
public:
	/* 获取操作系统预定义目录

		CSIDL_BITBUCKET				回收站
		CSIDL_CONTROLS				控制面板
		CSIDL_DESKTOP				Windows 桌面Desktop
		CSIDL_DESKTOPDIRECTORY		Desktop的目录
		CSIDL_DRIVES				我的电脑
		CSIDL_FONTS					字体目录
		CSIDL_NETHOOD				网上邻居
		CSIDL_NETWORK				网上邻居虚拟目录
		CSIDL_PERSONAL				我的文档
		CSIDL_PRINTERS				打印机
		CSIDL_PROGRAMS				程序组
		CSIDL_RECENT				最近打开的文档
		CSIDL_SENDTO				“发送到”菜单项
		CSIDL_STARTMENU				任务条启动菜单项
		CSIDL_STARTUP				启动目录
		CSIDL_TEMPLATES				文档模板
	*/
	static KFilePath GetSysPredefineDir(int csidl);
};

