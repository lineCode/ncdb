#pragma once
#include "PiTypeDef.h"
#include "doString.h"
#include "database/DataBase.h"
//#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
//#import "c:\msado15.dll" no_namespace rename("EOF","adoEOF")

/*  
    date: 2012-9-23 11:38:45
    author: 李万良
    fun: CDataBase 数据库辅助类, 封装获取字段 字符串和数值 的接口,如果字段值没有值,则返回 "" 和 0
    remark: 传入的是一个记录集引用,
*/
class CRecordsetAssist
{
public:
    CRecordsetAssist(_RecordsetPtr& rs);
    ~CRecordsetAssist(void);

    /************************************************************************/
    /*  fun: 获取当前记录行指定字段的字符串值
        remark: 如果字段值是空的, 则返回 空字符串""
    /************************************************************************/
    void        SetRecordset(_RecordsetPtr&);
    string      GetString(const string& strField);
    wstring     GetStringW(const string& strField);
    /************************************************************************/
    /*  fun: 获取当前记录行指定字段的整数值
        remark: 如果字段值是空的, 则返回 0
        ret: 返回长整数型
    /************************************************************************/
    LLONG GetInt(const string& strField);
public:

private:
    _RecordsetPtr&   m_recordset;
};

