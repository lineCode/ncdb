/***********************************************************************
 Copyright (c), 2012, Unicorn Tech. Co., Ltd.

 File name   : k_filepath.h
 Description : 文件路径类
 Other       : 
 Version     : 1.0
 Author      : xuemingzhi
 Date        : 2014/06/23
 -----------------------------------------------------------------------
 History 1 
      Date   : 
      Author : 
      Modification : 
***********************************************************************/

#ifndef KERNEL_FILEPATH_H
#define KERNEL_FILEPATH_H

#include "k_STLString.h"
#include <WinBase.h>

/* 文件路径类
*/
class KFilePath
{
public:
	KFilePath(LPCTSTR inSzPath = NULL);
	KFilePath(ConstSTLStringRef inStrPath);
	~KFilePath();

public:
	// 静态-获取文件目录、名称、后缀名
	static STLString GetFileDirectory(ConstSTLStringRef inPath);
	static STLString GetFileName(ConstSTLStringRef inPath,bool inHasExtend);
	static STLString GetFileExtend(ConstSTLStringRef inPath);
	static bool      IsValidFileName(ConstSTLStringRef inFileName);
	// 静态-是否物理有效
	static bool      IsExist(ConstSTLStringRef inPath);
	static bool      IsDirectory(ConstSTLStringRef inPath);
	
public:
    LPCTSTR    cdat() { return path_.c_str(); }
	STLString& GetPath() { return path_; }
	STLString  GetFileDirectory();
	STLString  GetFileName(bool inHasExtend);
	STLString  GetFileExtend();
	bool       IsExist();
	bool       IsDirectory();
	STLString  GetMD5();

public:
	KFilePath& operator=(ConstSTLStringRef inPath);
	KFilePath  operator+(ConstSTLStringRef inPath);
	KFilePath& operator+=(ConstSTLStringRef inPath);
	operator STLString() { return GetPath(); }
    operator LPCTSTR() { return GetPath().c_str(); }

	// 快捷路径
public:
	static STLString GetCurrentDir(); // 获取当前模块目录
	static STLString GetTempDir(); // 获取temp目录

protected:
	static void _unified_format(STLString& inPath);
protected:
	STLString path_;
};

#endif // !KERNEL_FILEPATH_H