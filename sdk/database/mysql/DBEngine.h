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

	//	��ȡconfig.ini���ݣ�����ʼ��mysql����
	bool InitPoolConf();

public:
	//	������MYSQL������
	void InitDB();

	//�������ӳ�
	int CreateConnect(unsigned long nClientFlag = CLIENT_MULTI_STATEMENTS);

	//�򿪵�������
	int Open(MYSQL*& pMysql,unsigned long nClientFlag = CLIENT_MULTI_STATEMENTS);

	//�رյ�������
	void Close(MYSQL* pMysql);

public:
	//�鿴���� ����Ͽ������Զ�����
	int Ping(MYSQL* pMysql);

	//ִ��SQL���
	int ExecSql(MYSQL* pMysql,const char *sql,CDBRowset* pRowset);

	//�����ַ���
	BOOL SetCharacterSetName(const char *csname);
	//��ȡ��ǰ�ض�MYSQL�ַ���
	const char* GetCharacterSetName(MYSQL* pMysql)
	{
		if (pMysql != NULL)
		{
			return mysql_character_set_name(pMysql);
		}

		return "";
	}

	//	��ȡ�ַ���
	char *GetSetName()
	{
		return m_CharSetName;
	}
	//�������
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

//	���������ͷ�CDBEngine����Դ
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
//�Զ�������
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
		//�ӳ����ҳ����е�����
		m_pDB = rEngine.GetActiveConnect(nPos);
		m_pEngine = &rEngine;
	}
public:
	~CDBMySqlPtr(void)
	{
		if (m_pEngine != NULL)
		{
			//�����˷Żس���
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
