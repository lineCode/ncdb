#include "StdAfx.h"
#include "SettingDataMng.h"
#include "PAXmlUserConfig.h"
#include "ResCommon.h"

CSettingDataMng::CSettingDataMng(void)
{
}


CSettingDataMng::~CSettingDataMng(void)
{
}

bool CSettingDataMng::Init()
{
    Create_Auto_Ptr(m_pXml, CPAXmlUserConfig);
//     tstring strXmlPath = _T("D:\\test\\DA\\userConfig.xml");
//     m_pXml->SetXmlPath(strXmlPath.c_str());
    if( m_pXml->Init() 
        && m_pXml->OpenDB())
    {
        return true;
    }
    return false;

}

bool CSettingDataMng::ReadGeneralConfig( CSettingGeneralParam& gP )
{
    //获取指定页面的所有选项, 
    //每个字段 需要找到对应的设置对象, 调用接口写入文件
    CSettingGeneralParam gParamTemp;
    tstring strData;
    m_pXml->ReadData( dn_launch_as_boot, gP.m_bAutoOpen);
    m_pXml->ReadData( dn_close_app, gP.m_bExit);
    m_pXml->ReadData( dn_task, gP.m_bManualOpen);
    return true;
}

bool CSettingDataMng::WriteGeneralConfig( CSettingGeneralParam& gP )
{
    //获取指定页面的所有选项, 
    //每个字段 需要找到对应的设置对象, 调用接口写入文件
    CSettingGeneralParam gParamTemp;
    m_pXml->WriteData( dn_launch_as_boot, gP.m_bAutoOpen);
    m_pXml->WriteData( dn_close_app, gP.m_bExit);
    m_pXml->WriteData( dn_task, gP.m_bManualOpen);
    //gP.m_bAutoOpen
    return true;
}

bool CSettingDataMng::ReadConfigApp( CSettingAppParam& sParam)
{
    m_pXml->ReadData( dn_app_save_setting, sParam.m_bSoftPackageSave);
    m_pXml->ReadData( dn_app_save_dir, sParam.m_strSoftPackagePath);
    m_pXml->ReadData( dn_driver_save_setting, sParam.m_bDriverPackageSave);
    m_pXml->ReadData( dn_driver_save_dir, sParam.m_strDriverPackagePath);
    m_pXml->ReadData( dn_desktop_record_path, sParam.m_strVedioPath);
    m_pXml->ReadData( dn_works_save_path, sParam.m_strWorksSavePath);
    m_pXml->ReadData( dn_fontLib_save_path, sParam.m_strFontPath);
    return true;

}

bool CSettingDataMng::WriteConfigApp( CSettingAppParam& sParam )
{
    m_pXml->WriteData( dn_app_save_setting, sParam.m_bSoftPackageSave);
    m_pXml->WriteData( dn_app_save_dir, sParam.m_strSoftPackagePath);
    m_pXml->WriteData( dn_driver_save_setting, sParam.m_bDriverPackageSave);
    m_pXml->WriteData( dn_driver_save_dir, sParam.m_strDriverPackagePath);
    m_pXml->WriteData( dn_desktop_record_path, sParam.m_strVedioPath);
    m_pXml->WriteData( dn_works_save_path, sParam.m_strWorksSavePath);
    m_pXml->WriteData( dn_fontLib_save_path, sParam.m_strFontPath);
    return true;
}

bool CSettingDataMng::ReadConfigUpdate( CSettingUpdateParam& sParam )
{
    return m_pXml->ReadData( dn_update_type, sParam.m_bUpdateType);
    
}

bool CSettingDataMng::WriteConfigUpdate( CSettingUpdateParam& sParam )
{
    return m_pXml->WriteData( dn_update_type, sParam.m_bUpdateType);

}

bool CSettingDataMng::ReadConfigFlow( CSettingMonitorParam& sParam )
{
    if(
    m_pXml->ReadData( dn_flow_auto_open, sParam.m_bAutoOpen)
    && m_pXml->ReadData( dn_flow_top_wnd, sParam.m_bTopMost))
    {
        return true;
    }

    return false;;
}

bool CSettingDataMng::WriteConfigFlow( CSettingMonitorParam& sParam )
{
    if(
    m_pXml->WriteData( dn_flow_auto_open, sParam.m_bAutoOpen)
    && m_pXml->WriteData( dn_flow_top_wnd, sParam.m_bTopMost))
    {
        return true;
    }
    return false;

}

bool CSettingDataMng::ReadAllConfig( CSettingTotalParam& sParam )
{
    CSettingTotalParam totalParam;
    ReadConfigApp(sParam.m_paramApp);
    ReadGeneralConfig(sParam.m_paramGeneral);
    ReadConfigUpdate(sParam.m_paramUpdate);
    ReadConfigFlow(sParam.m_paramMonitor);
    return true;
}

bool CSettingDataMng::WriteAllConfig( CSettingTotalParam& sParam )
{
    WriteGeneralConfig(sParam.m_paramGeneral);
    WriteConfigApp(sParam.m_paramApp);
    WriteConfigUpdate(sParam.m_paramUpdate);
    WriteConfigFlow(sParam.m_paramMonitor);
    return true;

}
