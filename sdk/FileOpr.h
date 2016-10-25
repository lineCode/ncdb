#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>
#include "PiTypeDef.h"
using std::string;
const UNLONG MAX_READ_SIZE = 1024 * 1024 * (UNLONG)5;//1M

enum ENUM_FILE_POS
{
    pos_begin = 0,
    pos_end,
};
class CFileOpr
{
public:
	CFileOpr(void);
	virtual ~CFileOpr(void);

public:
	bool	Init();
	bool	UnInit();

	/************************************************************************
        fun:	用指定模式打开指定文件,原先打开的会被关闭掉，	
		param:  mode:"r"- 只读 , "w"-写， 现有文件会被清空，"wb"- 二进制写

        ret:    
        remark: 如果打开失败,原先的文件还能继续操作， 不会受到影响
	/************************************************************************/
	//bool	Open(PCSTR filePath, PCSTR cMode = "rb", int shflag = _SH_DENYWR);
    bool    Open(tstring& strFile, tcpchar cMode = _T("rb"), int shflag = _SH_DENYWR);
    bool    Open(tcpchar filePath, tcpchar cMode = _T("rb"), int shflag = _SH_DENYWR);
	bool Read( void*& pBuf, UNLONG MaxSize, UNLONG* nReadReal = NULL);
	bool ReadBuf( void* pBuf, UNLONG MaxSize, UNLONG* nReadReal = NULL);
    bool    Write(const void* data, UNLONG dwSize);
	bool	Flush();
	bool	Close();
	char*	GetFileData();
    UNLONG	GetFileSize();
	static UNLONG	GetFileSize(tcpchar szPath);
    tstring GetFilePath();

    //错误描述
    void SetErrDesc(tcpchar szDesc );
    tstring GetErrDesc();
    static bool RemoveData( tcpchar szFile, DWORD nEraseSize, int nStartPos);
    static bool RemoveData( tcpchar szFile, DWORD nEraseSize, ENUM_FILE_POS nPos);
    /*设置偏移, 
        nOrigin:
            SEEK_SET:起始位置
            SEEK_END: 文件结尾
            SEEK_CUR: 当前位置
    */
    bool SetPos( UNLONG nRightStart, int nOrigin  = SEEK_SET);
    bool DeleteFile();
    static bool DeleteFile(tcpchar szPath);
    //重命名, 目标名字不可存在
    //可移动文件, 但不可移动目录
    bool Rename(tcpchar szNewName);
    static bool Rename(tcpchar szOldName, tcpchar szNewName);

	static bool IsModuleBit64(tcpchar szPath);
	static bool Save( void* pData, int nSize, tcpchar szFile );
	bool OpenWrite( tcpchar szFile);
	bool IsOpen();
private:
	FILE* m_pFile;
	char* m_pFileData;
    tstring m_strFilePath;//当前操作文件的绝对路径

	UNLONG m_dwFileSize;
    CRITICAL_SECTION	m_cs;
	tstring              m_strErrDesc;//错误描述
};
