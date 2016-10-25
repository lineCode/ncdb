#pragma once
#include <list>
#include "PiTypeDef.h"
#include "PASettingDef.h"
using namespace std;
class CPAXmlUserConfig;
class CDataSettingBase;

typedef list<CDataSettingBase*>     LST_DATA_SETTING;
/************************************************************************
    保存所有设置的数据
/************************************************************************/
class CPAUserSetting
{
public:
    CPAUserSetting(void);
    ~CPAUserSetting(void);
public:
    bool Init();
    bool SaveDB();
    CDataSettingBase* GetSetting( ENUM_DATA_DUTY dType);

private:
    bool CreateAllDS();
    void AddOptData( CDataSettingBase* pDsBase );
    bool OpenDB();
    bool NoRepeatDataSetting();
    void SetXmlPath( tcpchar szPath );
private:
    CPAXmlUserConfig*               m_pDB;

//     CDataSettingBase*            m_dsLaunchAsBoot;
//     CDataSettingBase*            m_dsCloseApp;
    LST_DATA_SETTING                m_lstDS;
    tstring                         m_strXmlPath;
};


