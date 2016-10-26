#include "StdAfx.h"
#include "DataBase.h"
#include <assert.h>
#include <sstream>
#include <iomanip>
#include "doString.h"
#include "Lock.h"
#include "functional.h"
#include "define_gnl.h"

namespace ns_pi_common {

const DWORD NUM_ReCnCircle      = 10;

using namespace std;

typedef CDataBase::DB_EXECUTE_STATE DB_EXECUTE_STATE;

CDataBase::CDataBase(void)
{
    m_bReCnNeed             = true;
}

CDataBase::~CDataBase(void)
{
    UnInit();
}

/*
void CDataBase::SetParam(const DB_Param& dbParam)
{
    m_bReCnNeed         = dbParam.bReCn;
    m_plErrorCodeReCn  = new long[dbParam.byCodeCount];
    memcpy(m_plErrorCodeReCn, dbParam.plErrorCode, dbParam.byCodeCount * sizeof(*dbParam.plErrorCode));

    m_byCodeCount       = dbParam.byCodeCount;
}*/

//初始化参数
bool CDataBase::Init( void )
{
    HRESULT hRet = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    if (hRet == S_FALSE)
    {
        //assert(_T("CoInitialize fail") == 0);
        //return FALSE;
        int a = 0;
    }
    else if(hRet == RPC_E_CHANGED_MODE)
    {
        int a = 0;
    }

	if(! _Init() )
	{
		return false;
	}

	//CLock l;
	Create_Auto_Ptr(m_pLock, CLock);
    //InitializeCriticalSection(&csDB);
	AddReCnCode();
    return true;
}


//清理资源
bool CDataBase::UnInit(void)
{
	CRAIILock raii(m_pLock->Lock());

	if(!_UnInit())
	{
		return false;
	}

    CoUninitialize();
    return true;
}

bool CDataBase::Close()
{
    //防止重复关闭, 抛出异常
	CRAIILock raii(m_pLock->Lock());
    if(m_pConnection->State != adStateClosed)
    {
        m_pConnection->Close();
    }
    return true;
}

bool CDataBase::AddReCnCode()
{
	m_lstReCn.push_back(CDBReConnect(0x80004005, _T("")));
	return true;
}

bool CDataBase::_Init()
{
	//创建Connection对象
	//if(FAILED(m_pConnection.CreateInstance(__uuidof(Connection))))
	if(FAILED(m_pConnection.CreateInstance(_T("ADODB.Connection"))))
	{ 
		return false;
	}

	if(FAILED(m_pCommand.CreateInstance(_T("ADODB.Command"))))
	{
		return false;
	}
	return true;
}

bool CDataBase::_UnInit()
{
	m_pCommand.Release();
	if (m_pConnection)
	{
		if (m_pConnection->State != adStateClosed)
		{
			m_pConnection->Close();//断开数据库连接
		}
	}
	m_pConnection.Release();
	//m_pConnection = NULL;
	return true;
}

DB_EXECUTE_STATE CDataBase::ConnectToDB( const tstring& strTemp )
{
    HRESULT hr = 0;

	CRAIILock raii(m_pLock->Lock());
    try
    {
        tstring strCn = strTemp;
        if (strCn.empty() )
        {
            strCn   = m_strCnStr;
        }
		//m_pConnection->CommandTimeout = 2;
		m_pConnection->ConnectionTimeout = 3;

		hr = m_pConnection->Open(strCn.c_str(), _T(""), _T(""),NULL);///连接数据库 
		
       // hr = m_pConnection->Open(_bstr_t(_T("Provider=SQLNCLI10;Server=117.25.155.115,9433\\MSSQLSERVER;Database=P2PWindow;Uid=banker; Pwd=banker8888;"))\
			, _bstr_t(_T("")), _bstr_t(_T("")), adModeUnknown);///连接数据库 

        if(FAILED(hr))
        {
            return DB_ERR_UNKNOW;
        }
        else
        {
            m_pCommand->ActiveConnection = m_pConnection;
            m_strCnStr = strCn;
        }
    }
    catch(_com_error e)
    {
        DealDBError(e);
        m_strErr = _T("connect DB fail! ") + m_strErr;
        /*if (m_pConnection->State != adStateClosed)
        {
            m_pConnection->Close();
        }*/
        return DB_ERR_UNKNOW;
    } 
    return DB_OK;
}

DB_EXECUTE_STATE CDataBase::ReConnect()
{
	CRAIILock raii(m_pLock->Lock());
    if(!_UnInit())
    {
        assert(0 && _T("反初始化失败"));
        return DB_ERR_UNKNOW;
    }
    Sleep(4000);
    if(!_Init())
    {
        assert(0 && _T("初始化失败"));
        return DB_ERR_UNKNOW;
    }
    //Sleep(2000);
    return ConnectToDB(m_strCnStr);
}

UNINT GetReConnectWait()
{
	return NUM_ReCnCircle * 1000;
}

void CDataBase::SaveErrorInfo(StringCRef strError)
{
	if(m_FuncSaveError)
	{
		(*m_FuncSaveError)(strError);
	}
}

DB_EXECUTE_STATE CDataBase::ExecuteSelf()
{
	CRAIILock raii(m_pLock->Lock());

    try
    {
        m_pCommand->CommandText = (_bstr_t)m_strLastSql.c_str();
		m_pCommand->CommandTimeout = 3;
        m_pRs = m_pCommand->Execute(NULL, NULL, adCmdText);
    }
    catch (_com_error e)
    {
        DealDBError(e);
		SaveErrorInfo(m_strErr);
        //m_strErr = _T("ExecuteSql fail,") + m_strErr;
        
        if (m_bReCnNeed 
			&& CanAttemptReCn())
        {
			SaveErrorInfo(_T("执行失败, 尝试重连"));
            while(1)
            {
                Sleep(GetReConnectWait());
                if(DB_OK == this->ReConnect())
                {
					SaveErrorInfo(_T("重连成功, 继续执行"));
					return ExecuteSelf();
                }
				SaveErrorInfo(_T("重连失败, 再次重连"));
            }
            return DB_OK;
        }

        return DB_ERR_UNKNOW;
    }
    
    return DB_OK;
}

//返回查询结果集
DB_EXECUTE_STATE CDataBase::ExecuteSql(const tstring& strSQL)
{
	CRAIILock raii(m_pLock->Lock());
    if (m_pConnection->State == adStateClosed)
    {
        assert(0 && _T("m_pConnection State is close!"));
        //return (FALSE);
    }

    //_CommandPtr			tmpCommand;

    //EnterCriticalSection(&csDB);
    //tmpCommand = m_pCommand;
    m_strLastSql = strSQL;
    //LeaveCriticalSection(&csDB);

    return ExecuteSelf();
}

void CDataBase::DealDBError(_com_error e)
{
    wostringstream oss;
	
    m_lErrCode  = e.Error();
	if (m_lErrCode == E_POINTER)
	{
		return ;
	}
    m_strErrDesc = e.Description();
    oss<<_T("code:0X")<<hex<<uppercase<<setw(8)<<m_lErrCode<<_T(" desc:<")<<m_strErrDesc<< _T(">sql:<")<< m_strLastSql<<_T(">");
    m_strErr = oss.str();
}

/*
DB_EXECUTE_STATE CDataBase::ExecuteSql(const wstring&strSQL)
{
    return ExecuteSql(ws2s(strSQL).c_str());
}*/

DB_EXECUTE_STATE CDataBase::Select(const tstring& strSQL)
{
    return this->ExecuteSql(strSQL);
}

_RecordsetPtr CDataBase::GetRecordset()
{
	CRAIILock raii(m_pLock->Lock());
    return m_pRs;
}

const _RecordsetPtr& CDataBase::GetRecordsetRf()
{
	CRAIILock raii(m_pLock->Lock());
    return m_pRs;
}

long CDataBase::GetErrCode()
{
	CRAIILock raii(m_pLock->Lock());
    return m_lErrCode;
}

//事务批量执行SQL语句，返回布尔值
bool CDataBase::ExecuteRoutineSQL(std::vector<tstring> vtrSQL)
{
    if(vtrSQL.empty())
    {
		return(false);
	}

	CRAIILock raii(m_pLock->Lock());
    if (m_pConnection->State == adStateClosed)
    {
        return (false);
    }

    bool bReturn = false;
    _variant_t RecordsAffected;

    try
    {
        m_pConnection->BeginTrans();
        for(std::vector<tstring>::iterator itrSQL = vtrSQL.begin();itrSQL != vtrSQL.end();++itrSQL)
        {
            if((*itrSQL).length())
            {
                bReturn = true;
                //bstrSQL = (*itrSQL).AllocSysString();
                m_pConnection->Execute(itrSQL->c_str(),&RecordsAffected,adCmdText); 
                //::SysFreeString(bstrSQL);
            }
        }
        m_pConnection->CommitTrans();
    }
    catch(_com_error e)
    { 	
        m_pConnection->RollbackTrans();
        DealDBError(e);
        m_strErr = _T("mutil task routine fail,") + m_strErr;
        bReturn = false;      
    } 

    return (bReturn);
}


tstring CDataBase::GetErrorString()
{
	CRAIILock raii(m_pLock->Lock());
    return m_strErr;
}

bool CDataBase::CanAttemptReCn()
{
	if (m_lErrCode != 0x80004005)	//是否为数据库链路连接方面的错误
	{
		return false;
	}

	FOR_EACH_ITER(LST_RE_CONNECT,m_lstReCn, it)
	{
		if(it->m_nCode == m_lErrCode
			&& 
			(it->m_strErrDesc.empty() || IsValidStrPos(m_strErrDesc.find(it->m_strErrDesc)))
			)
		{
			return true;
		}
	}
	return false;
    //根据错误代码来判断是否可以尝试进行重连
    /*for(UNINT i = 0; i < m_byCodeCount; ++i)
    {   
        if(m_lErrCode == m_plErrorCodeReCn[i])
        {
            return true;
        }
    }
    return false;
	*/
}

};