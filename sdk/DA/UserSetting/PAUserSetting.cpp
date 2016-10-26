#include "StdAfx.h"
#include "PAUserSetting.h"
#include "PAXmlUserConfig.h"
#include "DataSettingBase.h"
#include "PAOptionUI.h"
#include "define_gnl.h"
#include <set>
using namespace std;
CPAUserSetting::CPAUserSetting(void)
{
    m_pDB               = NULL;
    SetXmlPath(_T("D:\\test\\DA\\DA.xml"));
}


CPAUserSetting::~CPAUserSetting(void)
{
}

bool CPAUserSetting::OpenDB()
{
    return m_pDB->OpenDB();
}

bool CPAUserSetting::Init()
{
    
    m_pDB = new CPAXmlUserConfig;
    
    m_pDB->SetXmlPath(m_strXmlPath.c_str());

    return (m_pDB->Init() 
        && m_pDB->OpenDB() 
        && CreateAllDS());
    
}

bool CPAUserSetting::CreateAllDS()
{
    //CDataSettingBase* pDS = CDAOptUICloseApp::CreateOptData();

    AddOptData(CPAOptUICloseApp::CreateOptData());
    AddOptData(CPAOptUILaunchAsBoot::CreateOptData());
    AddOptData(CPAOptUIAppSaveOpt::CreateOptData());
    AddOptData(CPAOptUIAppSavePath::CreateOptData());
    AddOptData(CPAOptUITask::CreateOptData());
    AddOptData(CPAOptUIDriverSaveOpt::CreateOptData());
    AddOptData(CPAOptUIDriverSavePath::CreateOptData());
    AddOptData(CPAOptUIDesktopRecordPath::CreateOptData());
    AddOptData(CPAOptUIWorksSavePath::CreateOptData());
    AddOptData(CPAOptUIFontLibSavePath::CreateOptData());
    AddOptData(CPAOptUIUpdateType::CreateOptData());
    AddOptData(CPAOptUIFlowWndAsBoot::CreateOptData());
    AddOptData(CPAOptUIFlowTopWnd::CreateOptData());

    assert(NoRepeatDataSetting()/* UI和 UI数据集没有重复*/);
    return true;
}

CDataSettingBase* CPAUserSetting::GetSetting( ENUM_DATA_DUTY dType)
{
    CDataSettingBase* pSetting = NULL;
    FOR_EACH_ITER(LST_DATA_SETTING, m_lstDS, it)
    {
        if((*it)->GetDataDuty() == dType)
        {
            pSetting = *it;
            break;
        }
    }
    assert(pSetting);
    return pSetting;
}

void CPAUserSetting::AddOptData( CDataSettingBase* pDsBase )
{
    pDsBase->SetXml(m_pDB);
    
    m_lstDS.push_back(pDsBase);
}

bool CPAUserSetting::SaveDB()
{
    return m_pDB->SaveCache();
}

bool CPAUserSetting::NoRepeatDataSetting()
{
    bool bRet = true;
    typedef set<ENUM_DATA_DUTY> LST_DUTY;
    LST_DUTY setTemp;
    //pair<
    //LST_DUTY::iterator = setTemp.end();
    FOR_EACH_ITER(LST_DATA_SETTING, m_lstDS, it)
    {
        if(setTemp.insert((*it)->GetDataDuty()).second == false)
        {
            bRet = false;
            break;
        }
    }
    return bRet;
}

void CPAUserSetting::SetXmlPath( tcpchar szPath )
{
    m_strXmlPath = szPath;
}
