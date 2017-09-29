#ifndef _SQL_LOG_H_
#define  _SQL_LOG_H_
#include "../CLASS/Mylog.h"
class CSqlLog
{
public:
	CSqlLog(void);
	~CSqlLog(void);

public:
	BOOL InitSQLFile();

	BOOL Sql_Log(...);

	BOOL Sql_Log_Err(...);

public:
	CMyLog sqlLog;
};
//#define LOGINFO(...)	theApp.GetLogError()->Log(__VA_ARGS__);
//#define LOGERROR(...)	theApp.GetLogError()->Log(__VA_ARGS__);printf("%s ",Timer());printf(__VA_ARGS__);


//#define SQL_LOG(...)		sqlLog.sqlLog.Log(__VA_ARGS__);
//#define SQL_LOG_ERR(...)	sqlLog.sqlLog.Log(__VA_ARGS__);printf("%s ",(LPCTSTR)CGlobalOpt::GetCurTime());printf(__VA_ARGS__);
#endif



