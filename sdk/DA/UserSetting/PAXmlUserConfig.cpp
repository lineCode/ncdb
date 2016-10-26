#include "StdAfx.h"
#include "PAXmlUserConfig.h"
#include "Markup.h"
#include "XmlPathMgr.h"

CPAXmlUserConfig::CPAXmlUserConfig(void)
{

}


CPAXmlUserConfig::~CPAXmlUserConfig(void)
{
}

bool CPAXmlUserConfig::ReadWriteData( UserConfigData& ucData, ENUM_DATA_DUTY da, tstring& strData )
{
    bool bRet = false;
    switch(da)
    {
    case dn_launch_as_boot:
        {
            bRet = ReadWriteLaunchAsBoot(ucData, strData);
        }
        break;
    case dn_close_app:
        {
            bRet = ReadWriteCloseApp(ucData, strData);
        }
        break;
    case dn_task:
        {
            bRet = ReadWriteTask(ucData, strData);
        }
        break;
    case dn_app_save_setting:
        {
            bRet = ReadWriteSoftSaveSetting(ucData, strData);
        }
        break;
    case dn_app_save_dir:
        {
            bRet = ReadWriteSoftSavePath(ucData, strData);
        }
        break;
    case dn_driver_save_setting:
        {
            bRet = ReadWriteDriverSaveSetting(ucData, strData);
        }
        break;
    case dn_driver_save_dir:
        {
            bRet = ReadWriteDriverSavePath(ucData, strData);
        }
        break;
    case dn_desktop_record_path:
        {
            bRet = ReadWriteDesktopRecordPath(ucData, strData);
        }
        break;
    case dn_works_save_path:
        {
            bRet = ReadWriteWorksSavePath(ucData, strData);
        }
        break;
    case dn_fontLib_save_path:
        {
            bRet = ReadWriteFontLibSavePath(ucData, strData);
        }
        break;
    case dn_update_type:
        {
            bRet = ReadWriteUpdateType(ucData, strData);
        }
        break;
    case dn_flow_auto_open:
        {
            bRet = ReadWriteFlowAutoOpen(ucData, strData);
        }
        break;
    case dn_flow_top_wnd:
        {
            bRet = ReadWriteFlowTopWnd(ucData, strData);
        }
        break;

    }
    //把数据覆盖写入到xml
    return bRet;
}

bool CPAXmlUserConfig::DoMapXmlDataToMem()
{
    m_pXmlUtil->ResetPos();
    DealBoolRet(m_pXmlUtil->FindElem());
    //m_pXmlUtil->FindElem();

    DealBoolRet(m_pXmlUtil->IntoElem());

    DealBoolRet(m_pXmlUtil->FindElem(STRING_XML_UC_ELEM_COMMON));
    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_AUTO_OPEN));
    m_ConfigData.appAutoLaunch = m_pXmlUtil->GetChildData();

    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_EXIT_TYPE));
    m_ConfigData.exitType = m_pXmlUtil->GetChildData();

    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_TASK));
    m_ConfigData.task = m_pXmlUtil->GetChildData();

    //应用设置
    DealBoolRet(m_pXmlUtil->FindElem(STRING_XML_UC_ELEM_APP));
    DealBoolRet(m_pXmlUtil->IntoElem());         //app子层
    DealBoolRet(m_pXmlUtil->FindElem(STRING_XML_UC_ELEM_SOFT));

    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_SOFT_PACKAGESAVE));
    m_ConfigData.softPackageSave = m_pXmlUtil->GetChildData();

    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_SOFT_PACKAGEPATH));
    m_ConfigData.softPackagePath = m_pXmlUtil->GetChildData();


    DealBoolRet(m_pXmlUtil->FindElem(STRING_XML_UC_ELEM_DRIVER)); //video

    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_DRIVER_PACKAGESAVE));
    m_ConfigData.dPackageSave = m_pXmlUtil->GetChildData();

    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_DRIVER_PACKAGEPATH));
    m_ConfigData.dPackagePath = m_pXmlUtil->GetChildData();


    DealBoolRet(m_pXmlUtil->FindElem(STRING_XML_UC_ELEM_VIDEO));
    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_VIDEO_PATH));
    m_ConfigData.videoPATH = m_pXmlUtil->GetChildData();

    DealBoolRet(m_pXmlUtil->FindElem(STRING_XML_UC_ELEM_WORKSSAVE));
    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_WORKS_PATH));
    m_ConfigData.worksPath = m_pXmlUtil->GetChildData();

    DealBoolRet(m_pXmlUtil->FindElem(STRING_XML_UC_ELEM_FONTLIB));
    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_FONT_PATH));
    m_ConfigData.fontPath = m_pXmlUtil->GetChildData();

    //升级设置
    m_pXmlUtil->OutOfElem(); 
    DealBoolRet(m_pXmlUtil->FindElem(STRING_XML_UC_ELEM_UPDATE));

    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_UPATETYPE));
    m_ConfigData.updateType = m_pXmlUtil->GetChildData();

    //浮窗设置
    DealBoolRet(m_pXmlUtil->FindElem(STRING_XML_UC_ELEM_FLOWWND));
    DealBoolRet(m_pXmlUtil->IntoElem());
    DealBoolRet(m_pXmlUtil->FindElem(STRING_XML_UC_ELEM_FLOW_SHOW));
    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_FLOW_AUTOOPEN));
    m_ConfigData.flowAutoOpen = m_pXmlUtil->GetChildData();

    DealBoolRet(m_pXmlUtil->FindChildElem(STRING_XML_UC_ELEM_FLOW_TOPMOST));
    m_ConfigData.flowTopMost = m_pXmlUtil->GetChildData();
    return true;
}

bool CPAXmlUserConfig::CreateXmlData()
{
    m_pXmlUtil->SetDoc(NULL);
    m_pXmlUtil->SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));

    //m_pXmlUtil->ResetPos();
    m_pXmlUtil->AddElem(_T("root"));
    m_pXmlUtil->IntoElem();
    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_COMMON);
    m_pXmlUtil->IntoElem();
    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_AUTO_OPEN, m_ConfigData.appAutoLaunch.c_str());
    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_EXIT_TYPE, m_ConfigData.exitType.c_str());
    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_TASK, m_ConfigData.task.c_str());

    m_pXmlUtil->OutOfElem();
    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_APP);
    m_pXmlUtil->IntoElem();

    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_SOFT);
    m_pXmlUtil->AddChildElem(STRING_XML_UC_ELEM_SOFT_PACKAGESAVE, m_ConfigData.softPackageSave.c_str());
    m_pXmlUtil->AddChildElem(STRING_XML_UC_ELEM_SOFT_PACKAGEPATH, m_ConfigData.softPackagePath.c_str());

    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_DRIVER);
    m_pXmlUtil->AddChildElem(STRING_XML_UC_ELEM_DRIVER_PACKAGESAVE, m_ConfigData.dPackageSave.c_str());
    m_pXmlUtil->AddChildElem(STRING_XML_UC_ELEM_DRIVER_PACKAGEPATH, m_ConfigData.dPackagePath.c_str());

    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_VIDEO);
    m_pXmlUtil->AddChildElem(STRING_XML_UC_ELEM_VIDEO_PATH, m_ConfigData.videoPATH.c_str());

    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_WORKSSAVE);
    m_pXmlUtil->AddChildElem(STRING_XML_UC_ELEM_WORKS_PATH, m_ConfigData.worksPath.c_str());

    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_FONTLIB);
    m_pXmlUtil->AddChildElem(STRING_XML_UC_ELEM_FONT_PATH, m_ConfigData.fontPath.c_str());

    m_pXmlUtil->OutOfElem();
    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_UPDATE);
    m_pXmlUtil->AddChildElem(STRING_XML_UC_ELEM_UPATETYPE, m_ConfigData.updateType.c_str());


    m_pXmlUtil->AddElem(STRING_XML_UC_ELEM_FLOWWND);
    m_pXmlUtil->AddChildElem(STRING_XML_UC_ELEM_FLOW_SHOW);
    m_pXmlUtil->IntoElem();
    m_pXmlUtil->AddChildElem(STRING_XML_UC_ELEM_FLOW_AUTOOPEN, m_ConfigData.flowAutoOpen.c_str());
    m_pXmlUtil->AddChildElem(STRING_XML_UC_ELEM_FLOW_TOPMOST, m_ConfigData.flowTopMost.c_str());
    return true;
}

bool CPAXmlUserConfig::InitDefaultXmlStruct()
{
    m_ConfigData.appAutoLaunch      = STRING_UC_VALUE_SOFT_AUTO_OPEN_ENABLE;
    m_ConfigData.dPackagePath       = STRING_UC_VALUE_D_PACKAGEPATH;
    m_ConfigData.dPackageSave       = STRING_UC_VALUE_DRIVER_PACKAGESAVE_SAVE;
    m_ConfigData.exitType           = STRING_UC_VALUE_EXIT_TYPE_OTHER;
    m_ConfigData.fontPath           = STRING_UC_VALUE_F_PATH;
    m_ConfigData.softPackagePath    = STRING_UC_VALUE_PACKAGEPATH;
    m_ConfigData.softPackageSave    = STRING_UC_VALUE_PACKAGESAVE_DELETE;
    m_ConfigData.task               = STRING_UC_VALUE_TASK_AS_DRAWER;
    m_ConfigData.videoPATH          = STRING_UC_VALUE_VIDEO_PATH;
    m_ConfigData.worksPath          = STRING_UC_VALUE_W_PATH;
    m_ConfigData.updateType         = STRING_UC_VALUE_UPATETYPE_AUTO;
    m_ConfigData.flowAutoOpen       = STRING_UC_VALUE_FLOW_AUTOOPEN_YES;
    m_ConfigData.flowTopMost        = STRING_UC_VALUE_FLOW_TOPMOST_YES;
    return true;
}

bool CPAXmlUserConfig::DoReadWriteData( ENUM_DATA_DUTY da, tstring& strData )
{
    if(!ReadWriteData(m_ConfigData, da, strData))
    {
        return false;
    }
    return true;

}

bool CPAXmlUserConfig::DoReadWriteCache( ENUM_DATA_DUTY da, tstring& strData )
{
    if(!ReadWriteData(m_ConfigDataCache, da, strData))
    {
        return false;
    }
    return true;
}

tstring CPAXmlUserConfig::GetXmlFullPath()
{
    tstring strPath;

    CXmlPathMgr xmlPathMng;
    strPath = xmlPathMng.GetUserSettingXmlPath();
//     strPath = GetSysTempFolder();
//     strPath += _T("\\") STRING_PATH_XML_DIR;
//     strPath += STRING_NAME_XML;
    //strPath = _T("D:\\test\\DA\\userConfig.xml");
    return strPath;
}
