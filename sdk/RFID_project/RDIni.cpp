#include "StdAfx.h"
#include "RDIni.h"
#include "SingleLog.h"

static CLogOpr* g_pSingleLog;

CRDIni::CRDIni(const wchar_t* filename)
:CIni(filename)
{
	g_pSingleLog = CSingleLog::GetObj();
}

CRDIni::~CRDIni(void)
{
}

wchar_t* CRDIni::ReadParam(wchar_t* section, wchar_t* key)
{
    UNLONG dwRet = 0;
    memset(m_buf.m_pwBuf,0, BUF_SIZE * 2);
    dwRet = GetPrivateProfileStringW(section, key, m_cDefault, m_buf.m_pwBuf, 
        BUF_SIZE, m_filenameW);	
    if (dwRet <= 0)
    {
        UNLONG dwInt = GetLastError();
        g_pSingleLog->WriteLog("读取配置文件数据失败");
        return L"";
    }
    return m_buf.m_pwBuf;
}