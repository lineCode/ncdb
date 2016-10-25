#pragma once
#include <list>
#include "SettingParam.h"
#include "PASettingDef.h"
using namespace std;
class CPAXmlUserConfig;
class CSettingDataMng
{
public:
    CSettingDataMng(void);
    ~CSettingDataMng(void);
public:
    bool Init();
    bool SaveAllConfig();

    bool ReadGeneralConfig(CSettingGeneralParam& gP);
    bool WriteGeneralConfig(CSettingGeneralParam& gP);

    bool ReadConfigApp(CSettingAppParam& sParam );
    bool WriteConfigApp(CSettingAppParam& sParam );

    bool ReadConfigUpdate( CSettingUpdateParam& sParam );
    bool WriteConfigUpdate(CSettingUpdateParam& sParam  );

    bool ReadConfigFlow( CSettingMonitorParam& sParam );
    bool WriteConfigFlow(CSettingMonitorParam& sParam );

    bool ReadAllConfig(CSettingTotalParam& sParam);
    bool WriteAllConfig(CSettingTotalParam& sParam);

    auto_ptr<CPAXmlUserConfig>                   m_pXml;
};

