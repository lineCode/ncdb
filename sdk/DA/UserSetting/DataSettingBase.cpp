#include "StdAfx.h"
#include "DataSettingBase.h"
#include <assert.h>
#include "PAXmlUserConfig.h"

CDataSettingBase::CDataSettingBase( CPAXmlUserConfig* pXml, ENUM_DATA_DUTY dt )
{
    m_xml = pXml;
    m_dataDuty = dt;
}


CDataSettingBase::~CDataSettingBase(void)
{
}

// bool CDataSettingBase::InitDataType()
// {
//     assert(0);
//     return false;
// }

bool CDataSettingBase::Init()
{
    return true;
}

void CDataSettingBase::SetDataType( ENUM_DATA_DUTY dn )
{
    m_dataDuty      = dn;
}

bool CDataSettingBase::Write()
{
    m_xml->WriteData(m_dataDuty, m_strData);
    return true;
}

bool CDataSettingBase::Read()
{
    return  m_xml->ReadData(m_dataDuty, m_strData);
}

bool CDataSettingBase::SetData( tstring strData )
{
    m_strData = strData;
    return true;
}

tstring CDataSettingBase::GetData()
{
    return m_strData;
}

bool CDataSettingBase::SetXml( CPAXmlUserConfig* pXml )
{
    m_xml = pXml;
    return true;
}

bool CDataSettingBase::WriteCache()
{
    m_xml->WriteCache(m_dataDuty, m_strData);
    return true;
}

ENUM_DATA_DUTY CDataSettingBase::GetDataDuty()
{
    return m_dataDuty;
}


/************************************************************************
        CSetEnumCloseApp
************************************************************************/

COptDataPath::COptDataPath( CPAXmlUserConfig* pXml, ENUM_DATA_DUTY dt)
    :CDataSettingBase(pXml,dt)
{
    
}

COptDataPath::~COptDataPath( void )
{

}

bool COptDataPath::CheckDataValid()
{
    bool bIsValidPath = true;
    return bIsValidPath;
}


COptDataEnum::COptDataEnum( CPAXmlUserConfig* pXml, ENUM_DATA_DUTY dt )
    :CDataSettingBase(pXml, dt)
{

}

COptDataEnum::~COptDataEnum( void )
{

}

bool COptDataEnum::CheckDataValid()
{
    bool bIsNum = true;
    return bIsNum;
}
