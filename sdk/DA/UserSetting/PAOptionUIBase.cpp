#include "StdAfx.h"
#include "PAOptionUI.h"
#include "PAUserSetting.h"

CPAOptUIBase::CPAOptUIBase( CPAUserSetting* pSC )
{
    m_pSettingCol   = pSC;
    m_SetData       = NULL;
    m_dataDuty      = dn_unKnown;
}


CPAOptUIBase::~CPAOptUIBase(void)
{
}


CDataSettingBase* CPAOptUIBase::GetSetting( ENUM_DATA_DUTY dDuty)
{
    return m_pSettingCol->GetSetting(dDuty);
}

bool CPAOptUIBase::SaveData( const tstring& strData )
{
    m_SetData->SetData(strData);
    if(!m_SetData->CheckDataValid())
    {
        return false;
    }
    return m_SetData->Write();
}

bool CPAOptUIBase::SetData(const tstring& strData)
{
    m_SetData->SetData(strData);
    if(!m_SetData->CheckDataValid())
    {
        return false;
    }
    m_SetData->WriteCache();
    return true;
}


bool CPAOptUIBase::ReadData()
{
    if(!m_SetData->Read())
    {
        return false;
    }
    m_strData = m_SetData->GetData();

    return true;
}

bool CPAOptUIBase::SetSetting( CDataSettingBase* pSetting )
{
    m_SetData = pSetting;
    return true;
}

bool CPAOptUIBase::SetSetting( ENUM_DATA_DUTY dd)
{
    m_SetData = GetSetting(dd);
    return true;
}

bool CPAOptUIBase::SaveSetting()
{
    tstring strData;
    DoGetSaevData(strData);
    return SaveData(strData);
}

bool CPAOptUIBase::SetConfig()
{
    tstring strData;
    DoGetSaevData(strData);
    return SetData(strData);
}




bool CPAOptUIBase::ShowDB()
{
    ReadData();
    Show();
    return true;
}

tstring CPAOptUIBase::GetData()
{
    return m_strData;
}

bool CPAOptUIBase::InitData()
{
    return SetSetting(m_dataDuty);
}

void CPAOptUIBase::SetDataDuty( ENUM_DATA_DUTY dd )
{
    m_dataDuty = dd;
}
