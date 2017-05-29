#pragma once
#include "PiTypeDef.h"
class CPathLight
{
public:
    CPathLight();
    CPathLight(tcpchar szPath);
	CPathLight(const tstring& strPath);
    //CPathLight(const CPathLight& Path);
    ~CPathLight(void);
public:
	tstring AddSubSession(tcpchar szTitle);
	tstring AddSubSession(const tstring& szTitle);
	bool IsFileExist();

	bool SetPath(const tstring& strPath);
    const tstring& GetPath();
    bool GoParent();
    tstring GetCurPos();
    /************************************************************************
        fun:    如果没有调用GoParent, 则结尾的部分(包括文件名)会被当成目录名被修改
        remark:
    ************************************************************************/
    bool ModifyCurDirName( const tstring& strNameNew );
    bool SyncCache();
    bool ModifyFileName( const tstring& strNewName );
	tstring GetExt();
	operator const tstring&() const;
    operator tcpchar();

	CPathLight operator +(tcpchar szPath);
	CPathLight operator +(StringCRef szPath);
	CPathLight operator +(int nValue);
	CPathLight& operator +=(tcpchar szPath);
	CPathLight& operator +=(int nValue);
	CPathLight& operator +=(StringCRef szPath);
	CPathLight& operator =(tcpchar szPath);
	bool RenameTitle( StringCRef nameNew );
	bool RenameName( StringCRef nameNew );
	/*------------------------------------------------------------
			fun:	获取文件的md5
			ret:	
			param: 
			remark:	文件大小不能超过200M
	------------------------------------------------------------*/
	tstring GetMD5();
	tstring GetParentPathFromPath(bool bSlash = false);
	tstring GetFileName();


	static tstring GetSelfModulePath();
	static tstring GetSelfModuleFolder();

    //判断文件是否存在
    static tstring AddSub(tcpchar szDir, tcpchar szFile);    
    static bool IsFileExist(tcpchar szPath);

/************************************************************************
        fun:    去掉结尾的文件名 和斜杠
        param: 
        remark: 两种结构
                1. 如果尾部有斜杠,则去掉, 结束调用
                2. 去文件名(包括有无后缀), 再去斜杠
                3. 只剩盘符, 保留尾部斜杠
************************************************************************/
    static tstring GetParentPathFromPath( tcpchar szPath , bool bSlash = false);
    static tstring GetParentDirName(tcpchar szPath);
    //获取最后一段的文件名, 如果尾部是空格, 先去掉, 在获取
    static tstring GetDirName( tcpchar szPath );
    
	/************************************************************************
            fun: 获取文件后缀, 带.点字符
            param: 
            remark: 
    ************************************************************************/
	static tstring GetExt(StringCRef strPath);
	/************************************************************************
            fun: 获取文件后缀, 不带点字符
            param: 
            remark: 
    ************************************************************************/
	static tstring GetSuffixName(StringCRef strPath);
	tstring SetSuffixName(tcpchar szSuffix);

	static tstring GetFileTitleFromPath( tcpchar szPath );

    /************************************************************************
        文件枚举                                                                          
    ************************************************************************/    
    typedef bool (*EnumerateFunc) (tcpchar lpFileOrPath, void* pUserData );
    /************************************************************************
            fun: 
            param: 参数3:bool, true->查找文件, false-> 查找目录
            remark: 
    ************************************************************************/
	
	static bool FileEnumeration(tcpchar lpPath, bool bRecursion, bool bEnumFiles, bool bFindRecursionDir, EnumerateFunc pFunc, void* pUserData);
	static bool FileEnumeration(tcpchar lpPath, bool bRecursion, bool bEnumFiles, LST_STRING& lstS);
	static LST_STRING FileEnumeration(tcpchar lpPath, bool bRecursion, bool bEnumFiles);
    static bool CB_EnumerateFunc(tcpchar lpFileOrPath, void* pUserData );

    static bool IsValidPath( tcpchar szPath );
    static tstring GetExpandDir(tcpchar szVariant);
	static tstring GetFileNameFromPath( tcpchar szPath );
	static LLONG GetFileSize( tcpchar szPath );
	static tstring GetDirFromPath( tcpchar szPath );

	static bool CreateMultipleDirectory(tcpchar strFileFolder);	// 创建多级目录
	void AddSuffix(tcpchar szSuffix);
	bool IsDirExist();
private:
    tstring             m_strPath;
    tstring             m_strPos;   //内部保存临时位置
};

/***************** 宏项目问题 ***************/