#include "stdafx.h"
#include "SingleLog.h"
#include "LogOpr.h"
#include "PiTypeDef.h"
#include "FileStrOp.h"
#include "Lock.h"
#include "functional.h"
#include <assert.h>
#include "define_gnl.h"

Pi_NameSpace_Using
CLock*   CSingleLog::m_pLock = NULL;
CLogSpilit* CSingleLog::m_pstSingleLog;
string CSingleLog::m_strDate = "";

const UNLONG MAX_FILE_SIZE_LIMIT = 1024 * 1024  * 30;//30M
//const UNLONG MAX_FILE_SIZE_LIMIT = 1024 * 2;

CSingleLog::~CSingleLog(void)
{
	
}



void CSingleLog::Init()
{
	if (m_pstSingleLog)
	{
		return ;
	}
	m_pstSingleLog  = GetObj();
    m_pLock         = new CLock;
}

void CSingleLog::UnInit()
{
    DeleteObj();
    m_pstSingleLog = NULL;

    if (CSingleLog::m_pLock)
    {
        delete m_pLock;
        CSingleLog::m_pLock = NULL;
    }
    
    
}

bool CSingleLog::OpenTodayLog(tcpchar szPath)
{    
	DealBoolRet(m_pLock);
	return m_pstSingleLog->OpenTodayLog(szPath);
}


// void CSingleLog::PartLogFile()
// {
// 
//     //一个文件最多保存一天的数据
//     if(m_strDate != GetLocalDate())
//     {
//         string strDir = CFileStrOp::ExtractFileDir(m_pstSingleLog->GetFileName());//得到之前的log目录
//         if(!CSingleLog::OpenTodayLog(strDir.c_str()))
//         {
//             //log 错误描述
//             m_pstSingleLog->WriteLogWithDate(m_pstSingleLog->GetErrDesc());
//         }
//     }
//     else if (m_pstSingleLog->GetFileSize() >= MAX_FILE_SIZE_LIMIT)    //限制大小
//     {
//         string strFileFullPath = m_pstSingleLog->GetFileName();
//         string strNewTitle = CFileStrOp::GetNextFileName(strFileFullPath.c_str());
// 
//         if(!m_pstSingleLog->Open(CFileStrOp::ExtractFileDir(strFileFullPath) + strNewTitle, "a", _SH_DENYWR))
//         {
//             assert(INFO_ERROR "新日志文件打开失败");
//         }
//         else
//         {
//         }
//     }
// }
// 
DWORD CSingleLog::WriteLog(tcpchar data)
{
	DealBoolRet(m_pLock && m_pstSingleLog->IsOpen());
    assert(tstring(_T("无效指针")) != data && "写日志时, 数据为 \"无效指针\" ");

    assert(data && "写日志的字符串是空值");

    m_pLock->Lock();
	DWORD dwRet = m_pstSingleLog->WriteLog(data);
    m_pLock->UnLock();
	return dwRet;
}

DWORD CSingleLog::WriteLog(StringCRef data)
{
    return CSingleLog::WriteLog(data.c_str());
}


DWORD CSingleLog::WriteLogPure(tcpchar data)
{
	DealBoolRet(m_pLock);
    m_pLock->Lock();
    DWORD dwRet = m_pstSingleLog->WriteLogPure(data);
    m_pLock->UnLock();
    return dwRet;
}

DWORD CSingleLog::WriteLogPure(StringCRef data)
{
    return CSingleLog::WriteLogPure(data.c_str());
}

void CSingleLog::SetParam( const INIT_PARAM& iParam)
{
    m_pstSingleLog->SetParam(iParam);
}



DWORD ns_PiPi::CSingleLog::WriteLog( int data )
{
	tchar szBuf[40] = {0};
	_itot_s(data, szBuf, _countof(szBuf), 10);
	return WriteLog(szBuf);
}
