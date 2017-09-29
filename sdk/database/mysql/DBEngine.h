#pragma once
#include "include/mysql.h"
#include "DBStruct.h"
#include <atlcoll.h>
#include <assert.h>
#include "DBRowset.h"
#include "SqlLog.h"

//#pragma comment(lib,"../../LoginServer/mysql/lib/libmariadb.lib")


typedef CAtlArray<MYSQL* >  CConnectArray; 

class CDBMySqlPtr;

class CDBEngine
{
private:
	CDBEngine(void);

public:
	~CDBEngine(void);

public:
 
	static CDBEngine* getInstance()  
	{  
		return m_pDBEngine;
	}

	//	读取config.ini数据，来初始化mysql连接
	bool InitPoolConf();

public:
	//	创建与MYSQL的连接
	void InitDB();

	//创建连接池
	int CreateConnect(unsigned long nClientFlag = CLIENT_MULTI_STATEMENTS);

	//打开单条连接
	int Open(MYSQL*& pMysql,unsigned long nClientFlag = CLIENT_MULTI_STATEMENTS);

	//关闭单条连接
	void Close(MYSQL* pMysql);

public:
	//查看连接 如果断开连接自动接上
	int Ping(MYSQL* pMysql);

	//执行SQL语句
	int ExecSql(MYSQL* pMysql,const char *sql,CDBRowset* pRowset);

	//设置字符集
	BOOL SetCharacterSetName(const char *csname);
	//获取当前特定MYSQL字符集
	const char* GetCharacterSetName(MYSQL* pMysql)
	{
		if (pMysql != NULL)
		{
			return mysql_character_set_name(pMysql);
		}

		return "";
	}

	//	获取字符集
	char *GetSetName()
	{
		return m_CharSetName;
	}
	//事务操作
public:
	/*
	int StartTrans(MYSQL* pMysql);
	int CommitTrans(MYSQL* pMysql);
	int RollbackTrans(MYSQL* pMysql);
	*/
public:
	INT32 GetConnectPos(void);
	CDBMySqlPtr GetIdleConnect(void);

	MYSQL* GetActiveConnect(int nPos);
	BOOL PutActiveConnect(int nPos);


	CSqlLog& GetSqlLog()
	{
		return sqlLog;
	}

	
private:
	CDBEngine(const CDBEngine &);
	CDBEngine& operator=(const CDBEngine& db);

private:
	CConnectArray				m_arrayStore;
	CConnectArray				m_arrayIdle;
	CConnectArray				m_arrayActive;
	static CDBEngine*			m_pDBEngine;

	char						m_CharSetName[256];
	STMysqlConn					m_stConn;

	CSqlLog				sqlLog;

};

//	该类用于释放CDBEngine的资源
class CDBHelper
{
public:
	~CDBHelper()
	{
		CDBEngine *pDBEngine = CDBEngine::getInstance();
		if(pDBEngine)
		{
			delete pDBEngine;
			pDBEngine = NULL;
		}
	}

private:
	static CDBHelper dbHelp;
};


//////////////////////////////////////////////////////////////////////////
//自动连接者
//////////////////////////////////////////////////////////////////////////

class CDBMySqlPtr
{
public:
	friend class CDBEngine;

private:
	CDBMySqlPtr(CDBEngine & rEngine,INT32 nPos)  
		:m_nPos(nPos)
		,m_pDB(NULL)
		,m_pEngine(NULL)
	{
		//从池里找出空闲的连接
		m_pDB = rEngine.GetActiveConnect(nPos);
		m_pEngine = &rEngine;
	}
public:
	~CDBMySqlPtr(void)
	{
		if (m_pEngine != NULL)
		{
			//用完了放回池里
			m_pEngine->PutActiveConnect(m_nPos);
		}
	}

	/*
	CDBMySqlPtr& operator=(CDBMySqlPtr &rhs)
	{
		m_pDB			= rhs.m_pDB;
		m_pEngine		= rhs.m_pEngine;
		rhs.m_pEngine = NULL;
		return *this;
	}
	*/
public:

	MYSQL* operator->(void)
	{
		assert(m_pDB != NULL);
		return m_pDB;
	}

	operator MYSQL*()
	{
		assert(m_pDB != NULL);
		return m_pDB;
	}

	operator bool()
	{
		return (NULL != m_pDB);
	}

	MYSQL *GetDB(void)
	{
		return m_pDB;
	}

private:
	INT32			m_nPos;
	MYSQL*			m_pDB;
	CDBEngine*		m_pEngine;

};
