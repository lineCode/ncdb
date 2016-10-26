
//===========================================================================================
//CADODataBase类的定义文件
//日期：2011-3-2
//作者：Seven
//===========================================================================================

#ifndef __DEALWITHDB_H__
#define __DEALWITHDB_H__

//#include <comdef.h>
//#include <atlbase.h>
#include <string>
using namespace std;
#pragma   warning(disable:4146)
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF"), rename("BOF","adoBOF")

class CADODataBase
{
public:
/*
	enum DBType
	{
		ORACLR_DB = 0,         //链接Oracle数据库
		SQL_SERVER_DB,         //链接SQL_Server数据库
		ACCESS_DB				//链接Access数据库
	};
*/
	_ConnectionPtr   m_pConnection;
	_RecordsetPtr    m_pRecordset;
	_CommandPtr      m_pCommand;
	string         m_strSQL;
	string         m_strConn;

public:
	CADODataBase();
	~CADODataBase();
	BOOL InitDB(void);
	BOOL ConnectionDB(void);
	void CloseDB(void);									//关闭断开数据库
	BOOL ADOUpdata(const string& strSQL);                  //更新操作,新建,删除,修改string strCommand
	_RecordsetPtr ADOSelect(string strSQL);          //返回查询结果集
	BOOL GetLink(void);
	BOOL ReSetCnState(void);
	wstring  GetErrorMsg(void);
	
	_variant_t GetSelectValue(string strSql, const char* pColumn);
	//_variant_t GetSelectValue(string strSql, const char* pColumn, _RecordsetPtr* pRs);
	double GetSelectValueNum(string strSql, const char* pColumn);
	string GetSelectValueStr(string strSql, const char* pColumn);
	
	//附加的，不通用。  **写日记到数据库中
	string	m_LogTable;			//日志表的名字，如果要写日志，必须要填写
	string m_strLogContent;			//m_strContent：日志内容
	void MakeDBLog(string strLogContent = "", int logType=0);	//logType：日志类型(0，常规日志；1，调试日志)
	void SetConnectStr(const char* username, const char* pw, const char* source);
protected:
	CRITICAL_SECTION		csDB;

public:
	/*string m_userName;
	string m_pw;
	string m_Source;*/
private:
	BOOL		bOpenDB;
	wstring		m_strErrorMsg;	//错误信息
//	DBType   m_dbtype;

};

#endif //__DEALWITHDB_H__