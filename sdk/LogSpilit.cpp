#include "StdAfx.h"
#include "LogSpilit.h"
#include "SingleLog.h"
#include "LogOpr.h"
#include "PiTypeDef.h"
#include "FileStrOp.h"
#include "functional.h"
#include <assert.h>
#include "define_gnl.h"
#include "PiTime.h"
#include "StrCoding.h"
Pi_NameSpace_Using
CLogSpilit::CLogSpilit(void)
{
}

CLogSpilit::~CLogSpilit(void)
{
}

bool CLogSpilit::Init()
{
    return CFileOpr::Init();
}

bool CLogSpilit::UnInit()
{
    return CFileOpr::UnInit();
}

bool CLogSpilit::OpenTodayLog(tcpchar szPath)
{
	m_strDate = CPiTime::GetLocalDate();
    return CLogOpr::OpenTodayLog(szPath);
}


void CLogSpilit::PartLogFile()
{
    //一个文件最多保存一天的数据
    /*if(m_iParam.bSplitEveryDay && m_strDate != CPiTime::GetLocalDate())
    {
        string strDir = CFileStrOp::ExtractFileDir(CLogOpr::GetFileName());//得到之前的log目录
        if(!CLogSpilit::OpenTodayLog(strDir.c_str()))
        {
            //log 错误描述
            CLogOpr::WriteLogWithDate(CLogOpr::GetErrDesc());
        }
    }
    else if (CLogOpr::GetFileSize() >= m_iParam.nMaxSize)    //限制大小
    {
        string strFileFullPath = CLogOpr::GetFileName();
        string strNewTitle = CFileStrOp::GetNextFileName(strFileFullPath.c_str());

        if(!CLogOpr::Open(CFileStrOp::ExtractFileDir(strFileFullPath) + strNewTitle, "a", _SH_DENYWR))
        {
            assert(INFO_ERROR "新日志文件打开失败");
        }
        else
        {
            this->PartLogFile();
        }
    }*/
}


DWORD CLogSpilit::WriteLog(tcpchar data)
{
    assert(data && "写日志的字符串是空值");

    //TODO:分文件//PartLogFile();
    DWORD dwRet = this->WriteLogWithDate(data);
    return dwRet;
}

DWORD CLogSpilit::WriteLogPure(tcpchar data)
{
    //TODO:分文件//PartLogFile();
    DWORD dwRet = CLogOpr::WriteLog(data);
    return dwRet;
}

DWORD CLogSpilit::WriteLogPure(StringCRef data)
{
    return CLogSpilit::WriteLogPure(data.c_str());
}
DWORD CLogSpilit::WriteLog(StringCRef data)
{
    return WriteLog(data.c_str());
}


bool CLogSpilit::SetParam( const INIT_PARAM& iParam )
{
    m_iParam = iParam;
    return true;
}
