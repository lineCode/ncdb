#pragma once
#include <string>
#include "fileopr.h"
#include <sstream>
using namespace std;
#define LOG_TIME_SEPARATE       _T("  ")    //日志信息 时间和正文之间的分隔符

class CLogOpr :
	public CFileOpr
{
public:
	CLogOpr(void);
	~CLogOpr(void);

public:
	/***********************************************************************
		fun:打开今天的日志
		remark: the dir must existed
	/************************************************************************/
	
	bool OpenTodayLog(tcpchar DirPath = _T(""));
	bool   WriteLogWithDate(tcpchar data);
    bool   WriteLogWithDate(StringCRef data);
	bool WriteLog(tcpchar data);

	DWORD   WriteToTodayLog(const char* pData, DWORD dwSize);
	tstring GetTodayLogName();
private:
    tostringstream   m_oss;
};
