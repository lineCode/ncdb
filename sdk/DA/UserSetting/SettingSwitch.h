#pragma once
#include "datasettingbase.h"
class CSettingSwitch :
    public CDataSettingBase
{
public:
    CSettingSwitch(CPAXmlUserConfig* pXml, ENUM_DATA_DUTY dt);
    ~CSettingSwitch(void);
public:
   virtual bool InitDataType();
   bool CheckDataValid( const tstring& m_strData );
};

