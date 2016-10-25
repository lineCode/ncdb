/*
1. 如果由于网络问题， 如更改ip或网关, 数据库操作会出现 
    1. 包写入程序失败
    2. 失去联系 
    的错误， 解决方法是 如果出现这些错误， 则重新连接
*/
#pragma once
//#define MSADO15 "c:\msado15.dll"
#define MSADO15 "c:\program files\common files\system\ado\msado15.dll"
//#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
#import MSADO15 no_namespace rename("EOF","adoEOF")
//using namespace ADODB;
//class _RecordsetPtr;
//class _CommandPtr;
//#include <string>
#include <vector>
#include "ResCommon.h"
#include "PiTypeDef.h"
using namespace std;
#define _WIN32_DCOM
#define ERROR_LENGTH 500
class CLock;

namespace ns_pi_common
{
	typedef void (*FunSaveErrorInfo)(StringCRef );
class CDBReConnect
{
public:
    CDBReConnect(int nCodeR, tstring strErr)
	{
		m_strErrDesc = strErr;
		m_nCode = nCodeR;
	};
public:
    tstring m_strErrDesc;
	int		m_nCode;
};

typedef list<CDBReConnect>	LST_RE_CONNECT;

struct DB_Param
{
    bool    bReCn;
    long*   plErrorCode;
    byte    byCodeCount;
};

class CDataBase
{
public:
    enum DB_EXECUTE_STATE{
        DB_OK = 0, 
        DB_ERR_NET, 
        //DB_ERR_ALRROW,  //允许的小错误，可以忽略
        DB_ERR_UNKNOW   //未知错误
    };

public:
    CDataBase(void);
    ~CDataBase(void);

public:
	bool Init(void);

	

	bool                    UnInit(void);


    //void                    SetParam(const DB_Param& dbParam);//设置一些参数
    

    bool                    Close();
	DB_EXECUTE_STATE ConnectToDB(const tstring& strTemp);
    DB_EXECUTE_STATE        ReConnect();

    void                    DealDBError(_com_error e);

    /************************************************************************/
    /*  fun: 执行Sql,语句为先前的语句
    /************************************************************************/
    DB_EXECUTE_STATE    ExecuteSelf();

    /************************************************************************/
    /*  fun: 执行:SQL, 只返回执行状态, 
    /************************************************************************/
    DB_EXECUTE_STATE    ExecuteSql(const tstring& strSQL);
    //DB_EXECUTE_STATE    ExecuteSql(const wstring& strSQL);

    /************************************************************************/
    /*  fun: 执行SQL, 返回执行结果, 查询结果保存在成员变量里
    /************************************************************************/
    DB_EXECUTE_STATE    Select(const tstring& strSQL);

    bool                ExecuteRoutineSQL(std::vector<tstring> vtrSQL);
    tstring              GetErrorString();

    _RecordsetPtr           GetRecordset();
    const _RecordsetPtr&    GetRecordsetRf();
    long                    GetErrCode();
    bool                    CanAttemptReCn();   //判断是否可以尝试重连数据库
	tstring LastSql() const { return m_strLastSql; }
	bool AddReCnCode();
	ns_pi_common::FunSaveErrorInfo FuncSaveError() const { return m_FuncSaveError; }
	void FuncSaveError(ns_pi_common::FunSaveErrorInfo val) { m_FuncSaveError = val; }
	void SaveErrorInfo(StringCRef strError);
protected:
	bool _Init();		//重连内部使用初始化接口
	bool _UnInit();

private:
    _CommandPtr         m_pCommand;	            //SQL命令
    _ConnectionPtr      m_pConnection;          //数据库连接
    _RecordsetPtr       m_pRs;                  //保存查询结果集
    bool                m_bReCnNeed;            //必要时候是否需要重连

    //CRITICAL_SECTION		csDB;
	auto_ptr<CLock>			m_pLock;
    tstring              m_strErr;               //保存错误信息
    long                m_lErrCode;             //保存错误代码
    tstring              m_strErrDesc;           //错误描述

    tstring              m_strCnStr;             //保存连接字符串
    tstring              m_strLastSql;           //保存最后一次的SQL语句

    /*long*               m_plErrorCodeReCn;     //保存 哪些错误代码可重连数据库
    byte                m_byCodeCount;          //可重连的错误代码的个数*/

	LST_RE_CONNECT		m_lstReCn;			//可重连的错误代码
	FunSaveErrorInfo		m_FuncSaveError;
	
};

}
