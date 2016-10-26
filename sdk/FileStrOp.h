#pragma once
#include <string>
#include <sys/stat.h>
#include "piTypeDef.h"
#include <shellapi.h>
using std::string;
using std::wstring;
class CFileStrOp
{
public:
	
	CFileStrOp(void);
	~CFileStrOp(void);
public:
	enum TIME_TYPE{TYPE_CREATE, TYPE_ACCESS, TYPE_MODIFY};
	//功能：把路径设置成 以 "\"结尾的字符串 e.g: "C:\23" -> "C:\23\"
	//
	static string AddDirEndingChar(const string& strPath);
	
	static wstring AddDirEndingChar(const wstring& strPath);

	//////////////////////////////////////////////////////////////////////////
	//功能：获取指定文件名的后缀,不包括 .   e.g:  a.txt ->  txt
	static string GetPostFix(const string& fileName);
	static string GetDotSuffix(const string& fileName);
    
    /************************************************************************/
    /*  fun: 创建多层目录    eg: c:\1\2\3    , 运行该函数后, 能保证该目录存在, 除非出错
    /************************************************************************/
	static bool CreateDeepDir(tcpchar pDir);

    //判断指定文件/目录是否存在
	static bool IsExist(tcpchar pFile);
    /************************************************************************/
    /*  fun: 获取文件名的下一部分文件名， 用于分割文件 e.g: a.txt -> a_1.txt, a_1.txt -> a_2.txt
    /************************************************************************/
    static string CFileStrOp::GetNextFileName(const char* pFile);
	//////////////////////////////////////////////////////////////////////////
	//功能：获得文件名后缀名的位置(如果有)
	//参数：文件名(包括路径)
	//备注：当 . 的位置大于 最后一个\ 的位置，文件名才算有后缀   c:\34.3\Test.txt
	static string::size_type GetPostFixPos(const string& fileName);

	//////////////////////////////////////////////////////////////////////////
	//功能：获得文件的目录文件名
	//参数：文件名(包括路径)
	static string ExtractFileDir(const string& fileName);
    /************************************************************************/
    /*  fun: Get Current execute  file 's Dir of the current process
        remark: contain the '\' end
    /************************************************************************/
    static tstring GetCurrentDir();
    
    /************************************************************************/
    /*  fun: Get Current execute  file 's Title, eg: c:\2.txt -> 2
    /************************************************************************/
    static string CFileStrOp::GetCurrentExeTitle();
	//////////////////////////////////////////////////////////////////////////
	//功能：获得文件名
	//参数：绝对路径文件名
	static string ExtractFileName(const string& fileName);

	//////////////////////////////////////////////////////////////////////////
	//功能：获得文件的标题  e.g:   c:\文件.txt  ->  文件
	//参数：绝对路径文件名
	static string ExtractFileTitle(const string& fileName);

	/*------------------------------------------------------------
			fun:	文件大小智能转换成相应单位的字符串,如 1024 => "1k"
			ret:	szUnit:指定字符串格式, 按顺序指定小数点位数和后缀, 用"^"区分每一段
						. 如: "0# byte^0# KB^0# MB^0# GB"
							2# byte 表示 2位小数 数值后+" byte"后缀
			param: 
			remark:	
	------------------------------------------------------------*/
	static tstring FileSizeSmartSwitch(LLONG dwSize, tcpchar szUnit = NULL);

	/************************************************************************/
	/*	fun: 获得文件/夹图标                                                                     */
	/************************************************************************/
	static SHFILEINFOA GetFileIcon(const CHAR* path);



	/***********************************************************************
		文件信息相关
	***********************************************************************/
	static struct _stat GetFileStatusInfo(const char* pFile);

	/**************
	* 获取文件大小，如果为文件夹则大小为0
	*/
	static DWORD GetFileSize(tcpchar szFile);

	//不支持fat32磁盘
	static SYSTEMTIME GetFileCreateTime(const char* pFile);
	static SYSTEMTIME GetFileAccessTime(const char* pFile);
	static SYSTEMTIME GetFileModifyTime(const char* pFile);

	static string GetFileCreateTimeStr(const char* pFile);
	static tm GetFileTime(const char* pFile, TIME_TYPE type);
protected:

private:
	
};
