#include "StdAfx.h"
#include <iostream>
#include "DBEngine.h"
using namespace std;
#pragma comment(lib, "libmariadb.lib")
//////////////////////////////////////////////////////////////////////////
#define  ID_ACTIVE_BASE		100			//
//////////////////////////////////////////////////////////////////////////

CDBEngine* CDBEngine::m_pDBEngine = new CDBEngine();
CDBHelper CDBHelper::dbHelp;
//CSqlLog	 CDBEngine::sqlLog;

//#define SQL_LOG(...)		sqlLog.sqlLog.Log(__VA_ARGS__);
#define SQL_LOG_ERR(...)	sqlLog.sqlLog.ErrorLog(__VA_ARGS__);

//////////////////////////////////////////////////////////////////////////




CDBEngine::CDBEngine(void)
{
	//ms_pThis = this;
	InitPoolConf();
	ZeroMemory(m_CharSetName,sizeof(m_CharSetName));
	strcpy_s(m_CharSetName,"UTF8");
}

CDBEngine::~CDBEngine(void)
{
	for (size_t i=0; i<m_arrayStore.GetCount(); ++i)
	{
		MYSQL* pMysql = NULL;
		pMysql = m_arrayStore[i];
		Close(pMysql);
	}
	
	m_arrayStore.RemoveAll();
}

void CDBEngine::InitDB()
{
	CDBEngine *pDBEngine = CDBEngine::getInstance();
	pDBEngine->CreateConnect(CLIENT_MULTI_STATEMENTS);
	pDBEngine->SetCharacterSetName(pDBEngine->GetSetName());
}

bool CDBEngine::InitPoolConf()
{
	wchar_t * pwszCurrentFile;
	wchar_t wszValue[50];
	wchar_t wszSecion[20] = L"mysqlconnpool";
	//char   szFile[MAX_PATH];
	CString cstrPath = CGlobalOpt::GetCurPath();
	cstrPath += CString("config\\db_config.ini");
	pwszCurrentFile = cstrPath.GetBuffer(cstrPath.GetLength());
	unsigned int ulRet;
	m_stConn.nConnNum = GetPrivateProfileInt(wszSecion, L"conn_size", 0,pwszCurrentFile);
	if (m_stConn.nConnNum == 0) 
	{
		AfxMessageBox(_T("获取数据库的最大连接数出错！！"));
		return FALSE;
	}
	m_stConn.nPort = GetPrivateProfileInt(wszSecion, L"port", 0, pwszCurrentFile);
	if (m_stConn.nPort == 0) 
	{
		AfxMessageBox(_T("获取数据库的端口出错！！"));
		return FALSE;
	}
	ulRet = GetPrivateProfileString(wszSecion, L"username",  L"error", wszValue,sizeof(m_stConn.szUser)*2, pwszCurrentFile);
	strncpy_s(m_stConn.szUser, CGlobalOpt::WcharToChar(wszValue).c_str(),sizeof(m_stConn.szUser)-1);
	if (0==strcmp("error", m_stConn.szUser)) 
	{
		AfxMessageBox(_T("获取数据库的用户名出错！！"));
		return FALSE;
	}
	ulRet = GetPrivateProfileString(wszSecion, L"password", L"error", wszValue, sizeof(m_stConn.szPWD)*2, pwszCurrentFile);
	strncpy_s(m_stConn.szPWD, CGlobalOpt::WcharToChar(wszValue).c_str(),sizeof(m_stConn.szPWD)-1);
	if (0==strcmp("error", m_stConn.szPWD)) 
	{
		AfxMessageBox(_T("获取数据库的密码出错！！"));
		return FALSE;
	}
	ulRet = GetPrivateProfileString(wszSecion,  L"ip", L"error", wszValue, sizeof(m_stConn.szIp)*2, pwszCurrentFile);
	strncpy_s(m_stConn.szIp, CGlobalOpt::WcharToChar(wszValue).c_str(),sizeof(m_stConn.szIp)-1);
	if (0==strcmp("error", m_stConn.szIp)) 
	{
		AfxMessageBox(_T("获取数据库的IP出错！！"));
		return FALSE;
	}
	ulRet = GetPrivateProfileString(wszSecion, L"database", L"error",wszValue, sizeof(m_stConn.szDataBase)*2, pwszCurrentFile);
	strncpy_s(m_stConn.szDataBase, CGlobalOpt::WcharToChar(wszValue).c_str(),sizeof(m_stConn.szDataBase)-1);
	if (0==strcmp("error", m_stConn.szDataBase)) 
	{
		AfxMessageBox(_T("获取数据库的IP出错！！"));
		return FALSE;
	}
	return true;
}


int CDBEngine::Open(MYSQL*& pMysql,unsigned long nClientFlag /*= CLIENT_MULTI_STATEMENTS*/)
{
	CString cstrErr;
	if (pMysql != NULL)
	{
		mysql_close(pMysql);
		pMysql = NULL;
	}

	int nRet = 0;
	pMysql = mysql_init(NULL);
	if ( NULL == pMysql)
	{
		nRet = -1;
		cstrErr.Format(_T("[%s] error:%s"),_T("mysql_init"),_T("failed"));
		AfxMessageBox(cstrErr);
		exit(0);
	}

	my_bool bReconnect = true;
	nRet = mysql_options(pMysql,MYSQL_OPT_RECONNECT,&bReconnect);
	if (nRet != 0)
	{
		cstrErr.Format(_T("error:[%s][%s]"),L"mysql_options",CGlobalOpt::CharToWchar(mysql_error(pMysql)));
		mysql_close(pMysql);
		AfxMessageBox(cstrErr);
		pMysql = NULL;
		exit(0);
	}

	if (NULL == mysql_real_connect(pMysql
		,m_stConn.szIp
		,m_stConn.szUser
		,m_stConn.szPWD
		,m_stConn.szDataBase
		,m_stConn.nPort
		,NULL
		,nClientFlag))
	{
		nRet = -1;
		cstrErr.Format(_T("error:[%s][%s]"),L"mysql_real_connect",CGlobalOpt::CharToWchar(mysql_error(pMysql)));
		mysql_close(pMysql);
		AfxMessageBox(cstrErr);
		pMysql = NULL;
		exit(0);
	}

	nRet = mysql_select_db(pMysql,m_stConn.szDataBase);
	if (nRet != 0)
	{
		cstrErr.Format(_T("error:[%s][%s]"),L"mysql_select_db",CGlobalOpt::CharToWchar(mysql_error(pMysql)));
		mysql_close(pMysql);
		AfxMessageBox(cstrErr);
		pMysql = NULL;
		exit(0);
	}

	return nRet;
}

void CDBEngine::Close(MYSQL* pMysql)
{
	if (NULL != pMysql)
	{
		mysql_close(pMysql);
		pMysql = NULL;
	}
}

int CDBEngine::Ping(MYSQL* pMysql)
{
	int nRet = 0;
	if ( NULL == pMysql)
	{
		nRet = -1;
		goto PING_END;
	}

	nRet =mysql_ping(pMysql);
	if (nRet != 0)
	{
		SQL_LOG_ERR("[%s] error:%s\r\n","mysql_ping",mysql_error(pMysql));
		goto PING_END;
	}
	
	if ((nRet = mysql_set_character_set(pMysql,m_CharSetName))>0)
	{

		SQL_LOG_ERR("mysql_set_character_set error:%d %s\r\n",nRet,mysql_error(pMysql));
	}

PING_END:
	return nRet;
}

int CDBEngine::ExecSql(MYSQL* pMysql,const char *sql,CDBRowset* pRowset /*= NULL*/)
{
	if (pMysql == NULL)
	{
		return FALSE;
	}
	UINT uNum = 0;
	int nRedo = 2;
	do 
	{
		int nRet = mysql_real_query( pMysql, sql, strlen(sql) );
		if ( nRet )
		{
			SQL_LOG_ERR("error: Query(%s)\r\n", sql);
			SQL_LOG_ERR("mysql_real_query error:%s\r\n",mysql_error(pMysql));
			if ( (nRet = mysql_ping(pMysql)) > 0)
			{
				SQL_LOG_ERR("mysql_ping error:%d %s\r\n",nRet,mysql_error(pMysql));
			}

			if ((nRet = mysql_set_character_set(pMysql,m_CharSetName))>0)
			{
				SQL_LOG_ERR("mysql_set_character_set error:%d %s\r\n",nRet,mysql_error(pMysql));
			}

			--nRedo;
			continue;
		}
		else
		{
			do 
			{
				uNum = (UINT)mysql_field_count(pMysql);
				if (0==uNum)
				{
					uNum = (UINT)mysql_affected_rows(pMysql);
					//printf("%d rows affected\n",uNum);
				}
				else
				{
					MYSQL_RES*  pResult = mysql_store_result( pMysql );
					if (pResult)
					{
						pRowset->AddResult(pResult);
						if (pRowset != NULL && 1==pRowset->GetResultNumber())
						{
							pRowset->SeekResult(0);
						}
					}
					else
					{
						SQL_LOG_ERR("mysql_store_result error:%s\r\n",mysql_error(pMysql));
					}
				}
				if ( (nRet = mysql_next_result(pMysql) > 0))
				{
					SQL_LOG_ERR("mysql_next_result error:%s\r\n",mysql_error(pMysql));
				}
			} while (-1==nRet);
			//nRedo = -1;
			break;
		}
		//nRedo = 0;
	} while (nRedo > 0);

	if (nRedo>0)
	{
		return TRUE;
	}
	return FALSE;
}

CDBMySqlPtr CDBEngine::GetIdleConnect()
{
	INT32 nPos = GetConnectPos();
	return CDBMySqlPtr(*this,nPos);
}

INT32 CDBEngine::GetConnectPos(void)
{
	MYSQL* pMysqlTemp = NULL;
	INT32 nPos = -1;
	if ( !m_arrayIdle.IsEmpty())
	{
		size_t nEndPos = m_arrayIdle.GetCount()-1;
		pMysqlTemp = m_arrayIdle[nEndPos];
		m_arrayIdle.RemoveAt(nEndPos);
	}

	if (pMysqlTemp != NULL)
	{
		nPos = (int)m_arrayActive.Add(pMysqlTemp);
		nPos += ID_ACTIVE_BASE;
	}
	//printf("get connect:%p\n",pMysqlTemp);
	return nPos;
}

BOOL CDBEngine::PutActiveConnect(int nPos)
{
	nPos -= ID_ACTIVE_BASE;
	if (nPos < 0 || (size_t)nPos >= m_arrayActive.GetCount())
	{
		return FALSE;
	}

	MYSQL* pMysql = NULL;
	pMysql = m_arrayActive[nPos];
	m_arrayActive.RemoveAt(nPos);
	m_arrayIdle.Add(pMysql);
	//printf("put connect:%p\n",pMysql);
	return TRUE;
}

MYSQL* CDBEngine::GetActiveConnect(int nPos)
{
	nPos -= ID_ACTIVE_BASE;
	if (nPos < 0 || (size_t)nPos >= m_arrayActive.GetCount())
	{
		return NULL;
	}
	MYSQL* pMysql = NULL;
	pMysql = m_arrayActive[nPos];

	return pMysql;
}

int CDBEngine::CreateConnect(unsigned long nClientFlag /*= CLIENT_MULTI_STATEMENTS*/)
{
	//nMaxConnect = min(1,nMaxConnect);
	for (int i=0; i<m_stConn.nConnNum; ++i)
	{
		MYSQL* pMysql = NULL;
		//int nRet= Open(pMysql,pszHost,pszUser,pszPasswd,pszDB,nPort,nClientFlag);
		int nRet = Open(pMysql,nClientFlag);
		if (nRet != 0)
		{
			SQL_LOG_ERR("%s error:%d\r\n",__FUNCTION__,nRet);
		}
		printf("add connect:%p\n",pMysql);
		m_arrayStore.Add(pMysql);
		m_arrayIdle.Add(pMysql);
	}

	return 0; 
}

BOOL CDBEngine::SetCharacterSetName(const char *csname)
{
	for (UINT32 i=0;i<m_arrayStore.GetCount();i++)
	{
		MYSQL* pMysql = NULL;
		pMysql = m_arrayStore.GetAt(i);
		int nRet = mysql_set_character_set(pMysql,csname);
		if (nRet > 0)
		{
			SQL_LOG_ERR("mysql_set_character_set error:%d %s\r\n",nRet,mysql_error(pMysql));
		}
	}
	memcpy(m_CharSetName,csname,strlen(csname));
	return TRUE;
}

/*
int CDBEngine::StartTrans(MYSQL* pMysql)
{
	if ( pMysql != NULL)
	{
		if(!mysql_real_query(pMysql, "START TRANSACTION",
			(unsigned long)strlen("START TRANSACTION") ) )
		{
			return 0;
		}
	}

	return -1;
}
*/

/*
int CDBEngine::CommitTrans(MYSQL* pMysql)
{
	if ( pMysql != NULL)
	{
		if(!mysql_real_query(pMysql, "COMMIT",
			(unsigned long)strlen("COMMIT") ) )
		{
			return 0;
		}
	}

	return -1;
}
*/

/*
int CDBEngine::RollbackTrans(MYSQL* pMysql)
{
	if ( pMysql != NULL)
	{
		if(!mysql_real_query(pMysql, "ROLLBACK",
			(unsigned long)strlen("ROLLBACK") ) )
		{
			return 0;
		}
	}

	return -1;
}
*/






