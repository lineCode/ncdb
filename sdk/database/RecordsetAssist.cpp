#include "StdAfx.h"
#include "RecordsetAssist.h"
#include "doString.h"

CRecordsetAssist::CRecordsetAssist(_RecordsetPtr& rs /*= NULL*/)
:
    m_recordset(rs)
{
    
}

CRecordsetAssist::~CRecordsetAssist(void)
{
}


void CRecordsetAssist::SetRecordset(_RecordsetPtr& rs)
{
    m_recordset = rs;
    return ;
}

wstring CRecordsetAssist::GetStringW(const string& strField)
{
    _variant_t vrtRet = m_recordset->GetCollect(strField.c_str());
    if (vrtRet.vt == VT_NULL)
    {
        return L"";
    }
    else
    {
        return vrtRet.bstrVal;
    }

}

string CRecordsetAssist::GetString(const string& strField)
{
    return ws2s(GetStringW(strField));
}

LLONG CRecordsetAssist::GetInt(const string& strField)
{
    _variant_t vrtRet = m_recordset->GetCollect(strField.c_str());
    if (vrtRet.vt == VT_NULL)
    {
        return 0;
    }
    else
    {
        return vrtRet.llVal;
    }
}