#include "StdAfx.h"
#include "SettingSwitch.h"


CSettingSwitch::CSettingSwitch(CPAXmlUserConfig* pXml, ENUM_DATA_DUTY dt)
    :CDataSettingBase(pXml,dt)
{
}


CSettingSwitch::~CSettingSwitch(void)
{
}

bool CSettingSwitch::InitDataType()
{
    SetDataType(dn_launch_as_boot);
    return false;
}


bool CSettingSwitch::CheckDataValid( const tstring& m_strData )
{
    return true;
}
