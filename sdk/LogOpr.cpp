#include "stdafx.h" 
#include "LogOpr.h"
#include "functional.h"
#include <fstream>

#include <assert.h>
#include "FileStrOp.h"
#include "PathLight.h"
#include "PiTime.h"
#include "StrCoding.h"


using namespace std; 



CLogOpr::CLogOpr(void)
{

}

CLogOpr::~CLogOpr(void)
{
	this->Flush();
}


/************************************************************************/
/* 打开今天的日志文件，归属到从                                                                     */
/************************************************************************/
bool CLogOpr::OpenTodayLog( tcpchar DirPath /*= _T("")*/ )
{
    assert(CFileStrOp::CreateDeepDir(DirPath));//一般不会创建失败吧
		
	tstring logFileName = CPathLight::AddSub(DirPath, GetTodayLogName().c_str());
	//logFileName += GetTodayLogName();
	logFileName += _T(".txt");
    if(!this->Open(logFileName.c_str(), _T("ab"), _SH_DENYWR))
	{
		return false;
	}
	return true;
}

bool CLogOpr::WriteLogWithDate(tcpchar data)
{
	DealBoolRet(IsOpen());
	tstring strNow = CPiTime::GetLocalDateTime();
    //char cSeparateBuf[50] = {"  "};
    m_oss.str(_T(""));
    m_oss<<strNow<<LOG_TIME_SEPARATE<<data;
    tstring strTemp = m_oss.str();
    return WriteLog(strTemp.c_str());
}

bool CLogOpr::WriteLog( tcpchar data )
{
    m_oss.str(_T(""));
    //m_oss<<data<<_T("\r\n");
	m_oss<<data<<_T("\r\n");

    string strTemp = t2s(m_oss.str());
    this->Write(strTemp.c_str(), strTemp.length() /* no need \0 */);/*是否需要写\0*/
    return Flush();
}

bool CLogOpr::WriteLogWithDate(StringCRef data)
{
    return WriteLogWithDate(data.c_str());
}

DWORD CLogOpr::WriteToTodayLog(const char* pData, DWORD dwSize)
{
	//把数据追加写入d
	return this->Write(pData, dwSize);
}

tstring CLogOpr::GetTodayLogName()
{
	return CPiTime::GetLocalDate();
}

