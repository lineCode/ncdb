#include "stdafx.h"
#include "SqlLog.h"


CSqlLog::CSqlLog(void)
{
	InitSQLFile();
}


CSqlLog::~CSqlLog(void)
{
}

BOOL CSqlLog::InitSQLFile()
{
	
	char szFile[MAX_SIZE] = "./SQLlog/";
	sqlLog.InitDir(szFile);
	sprintf_s(szFile,"%s%s%s",szFile, "sql_", sqlLog.GetLogFile().c_str());
	sqlLog.InitFile(szFile);
	
	return TRUE;
}

/*
BOOL CSqlLog::Sql_Log(...)
{
	sqlLog.Log(__VA_ARGS__);
}

BOOL CSqlLog::Sql_Log_Err(...)
{
	sqlLog.Log(__VA_ARGS__);
	printf("%s ",Timer());
	printf(__VA_ARGS__);
}
*/