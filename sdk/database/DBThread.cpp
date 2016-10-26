#include "StdAfx.h"
#include "DBThread.h"
#include "database/DataBase.h"
#include "StrCoding.h"
#include "PiODBC.h"
#include "RAIILock.h"
#include "Lock.h"
#include "Signal.h"
#include "functional.h"
#include "GLog.h"


CDBThread::CDBThread(void)
{
}


CDBThread::~CDBThread(void)
{
	UnInit();
}

bool CDBThread::Init()
{
	if (m_strSqlConnect.empty())
	{
		return false;
	}

	CThreadLight::SetThreadName(_T("CDBThread"));
	CThreadLight::Init();

	Create_Auto_Ptr(m_pDB, CDataBase);
	Create_Auto_Ptr(m_Sign, CSignal);
	Create_Auto_Ptr(m_pLockSql, CLock);
	Create_Auto_Ptr(m_pLockDB, CLock);

	if( ! m_pDB->Init()
		|| m_pDB->ConnectToDB(w2sSmart(m_strSqlConnect)) != ns_pi_common::CDataBase::DB_OK)
	{
		LOG(INFO) << "error, db cn fail";
		return false;
		//AfxMessageBox(_T("connect fail"));
	}

	//TODO:
	m_Sign->Init(0, 1000);
	CThreadLight::Run();

	return true;
}

bool CDBThread::AddQuery( CSql& sql )
{
	return AddSql(sql);
}

bool CDBThread::Query( CSql& sql )
{
	CRAIILock raii(m_pLockDB->Lock());
	if( m_pDB->Select(w2s(sql.m_strSql)) != CDataBase::DB_OK)
	{
		return false;
	}
	sql.m_pRs = m_pDB->GetRecordsetRf();
	return true;

}

bool CDBThread::AddExecute( CSql& sql )
{
	//TODO:放入队列
	return AddSql(sql);
}

bool CDBThread::Execute( CSql& sql )
{
	CRAIILock raii(m_pLockDB->Lock());
	if( m_pDB->ExecuteSql(sql.m_strSql) != CDataBase::DB_OK)
	{
		return false;
	}
	return true;

}

bool CDBThread::AddSql( CSql& sql )
{
	CRAIILock raii(m_pLockSql->Lock());
	m_lstSqlNeedRun.push_back(new CSql(sql));
	return m_Sign->Add();
}

DWORD CDBThread::DoWork()
{
	UNLONG nRunCount = 0;
	while(1)
	{
		//TODO:等待有队列的信号量
		LOG(INFO) << "wait";
		if(! m_Sign->WaitSign() )
		{
			LOG(INFO) << "dbThread will exit";
			break;
		}
		/*if (CThreadLight::IsWillStopThread())
		{
			break;
		}*/

		CRAIILock raii(m_pLockSql->Lock());
		CSql* pSql = m_lstSqlNeedRun.front();
		CSql sql = *pSql;
		ClearMem(pSql);

		m_lstSqlNeedRun.pop_front();

		CRAIILock raii2(m_pLockDB->Lock());
		LOG(INFO) << "will execute";

		if( m_pDB->ExecuteSql(sql.m_strSql) != CDataBase::DB_OK)
		{
			LOG(INFO) << "ERROR: db thread execute fail: sql count:"<< m_pDB->GetErrorString();
			//OutInfo(_T("ERROR: db thread execute fail: sql count:")\
				, s2w(m_pDB->GetErrorString()).c_str());
		}
		++nRunCount;
		//LOG(INFO) << "after execute";

	}
	
	return true;
}

bool CDBThread::UnInit()
{
	//CThreadLight::StopThread()
	m_Sign->SetSign(false);
	CThreadLight::UnInit();
	return true;
}

bool CDBThread::SetConnectSql( tcpchar szSql )
{
	m_strSqlConnect = szSql;
	return true;
}
