#include "StdAfx.h"
#include "PAOptionUI.h"



/************************************************************************
    关闭应用程序的处理
/************************************************************************/


CPAOptUICloseApp::CPAOptUICloseApp( CPAUserSetting* pSC )
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}


CPAOptUICloseApp::~CPAOptUICloseApp(void)
{
}


bool CPAOptUICloseApp::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cCloseApp");
    return true;
}


bool CPAOptUICloseApp::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUICloseApp::CreateOptData()
{
    return new COptDataEnum(NULL, enDuty);
}



/************************************************************************
    是否随系统启动
/************************************************************************/


CPAOptUILaunchAsBoot::CPAOptUILaunchAsBoot( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}


CPAOptUILaunchAsBoot::~CPAOptUILaunchAsBoot(void)
{
}


bool CPAOptUILaunchAsBoot::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cLaunchAsBoot");
    return true;
}


bool CPAOptUILaunchAsBoot::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUILaunchAsBoot::CreateOptData()
{
    return new COptDataEnum(NULL, enDuty);
}





/************************************************************************
    是否保存应用安装包
/************************************************************************/

CPAOptUITask::CPAOptUITask( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}


CPAOptUITask::~CPAOptUITask(void)
{
}

bool CPAOptUITask::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cTask");
    return true;
}


bool CPAOptUITask::Show()
{
    tstring strData = GetData();
    TRACE(strData .c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUITask::CreateOptData()
{
    return new COptDataEnum(NULL, enDuty);
}



CPAOptUIAppSaveOpt::CPAOptUIAppSaveOpt( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}


CPAOptUIAppSaveOpt::~CPAOptUIAppSaveOpt(void)
{
}


bool CPAOptUIAppSaveOpt::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cAppSaveOpt");
    return true;
}


bool CPAOptUIAppSaveOpt::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUIAppSaveOpt::CreateOptData()
{
    return new COptDataEnum(NULL, enDuty);

}



/************************************************************************
    是否保存应用安装包
/************************************************************************/

CPAOptUIAppSavePath::CPAOptUIAppSavePath( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}


CPAOptUIAppSavePath::~CPAOptUIAppSavePath(void)
{
}


bool CPAOptUIAppSavePath::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cAppSavePath");
    return true;
}


bool CPAOptUIAppSavePath::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUIAppSavePath::CreateOptData()
{
    return new COptDataPath(NULL, enDuty);
}



/************************************************************************
    任务, 是否跟随绘画软件启动
/************************************************************************/

/************************************************************************
    驱动保存设置,
/************************************************************************/

CPAOptUIDriverSaveOpt::CPAOptUIDriverSaveOpt( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}


CPAOptUIDriverSaveOpt::~CPAOptUIDriverSaveOpt(void)
{
}

bool CPAOptUIDriverSaveOpt::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cDriverSaveOpt");
    return true;
}


bool CPAOptUIDriverSaveOpt::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUIDriverSaveOpt::CreateOptData()
{
    return new COptDataEnum(NULL, enDuty);
}




/************************************************************************
    驱动保存目录,
/************************************************************************/

CPAOptUIDriverSavePath::CPAOptUIDriverSavePath( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}

CPAOptUIDriverSavePath::~CPAOptUIDriverSavePath(void)
{
}

bool CPAOptUIDriverSavePath::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cDriverSavePath");
    return true;
}

bool CPAOptUIDriverSavePath::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUIDriverSavePath::CreateOptData()
{
    return new COptDataPath(NULL, enDuty);
}



/************************************************************************
    桌面录制目录,
/************************************************************************/

CPAOptUIDesktopRecordPath::CPAOptUIDesktopRecordPath( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}

CPAOptUIDesktopRecordPath::~CPAOptUIDesktopRecordPath(void)
{
}

bool CPAOptUIDesktopRecordPath::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cDesktopRecordPath");
    return true;
}

bool CPAOptUIDesktopRecordPath::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUIDesktopRecordPath::CreateOptData()
{
    return new COptDataPath(NULL, enDuty);
}


/************************************************************************
    作品保存目录,
/************************************************************************/

CPAOptUIWorksSavePath::CPAOptUIWorksSavePath( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}

CPAOptUIWorksSavePath::~CPAOptUIWorksSavePath(void)
{
}

bool CPAOptUIWorksSavePath::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cWorksSavePath");
    return true;
}

bool CPAOptUIWorksSavePath::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUIWorksSavePath::CreateOptData()
{
    return new COptDataPath(NULL, enDuty);
}



/************************************************************************
    作品保存目录,
/************************************************************************/

CPAOptUIFontLibSavePath::CPAOptUIFontLibSavePath( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}

CPAOptUIFontLibSavePath::~CPAOptUIFontLibSavePath(void)
{
}

bool CPAOptUIFontLibSavePath::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cFontLibSavePath");
    return true;
}

bool CPAOptUIFontLibSavePath::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUIFontLibSavePath::CreateOptData()
{
    return new COptDataPath(NULL, enDuty);
}




/************************************************************************
    升级方式
/************************************************************************/

CPAOptUIUpdateType::CPAOptUIUpdateType( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}

CPAOptUIUpdateType::~CPAOptUIUpdateType(void)
{
}

bool CPAOptUIUpdateType::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cUpdateType");
    return true;
}

bool CPAOptUIUpdateType::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUIUpdateType::CreateOptData()
{
    return new COptDataEnum(NULL, enDuty);
}




/************************************************************************
    软件启动时开启浮窗
/************************************************************************/

CPAOptUIFlowWndAsBoot::CPAOptUIFlowWndAsBoot( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}

CPAOptUIFlowWndAsBoot::~CPAOptUIFlowWndAsBoot(void)
{
}

bool CPAOptUIFlowWndAsBoot::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cFlowWndAsBoot");
    return true;
}

bool CPAOptUIFlowWndAsBoot::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUIFlowWndAsBoot::CreateOptData()
{
    return new COptDataEnum(NULL, enDuty);
}



/************************************************************************
    软件启动时开启浮窗
/************************************************************************/

CPAOptUIFlowTopWnd::CPAOptUIFlowTopWnd( CPAUserSetting* pSC ) 
    :CPAOptUIBase(pSC)
{
    SetDataDuty(enDuty);
}

CPAOptUIFlowTopWnd::~CPAOptUIFlowTopWnd(void)
{
}

bool CPAOptUIFlowTopWnd::DoGetSaevData( tstring& strData )
{
    //根据界面, 获取设置项的字符串
    strData = _T("cFlowTopWnd");
    return true;
}

bool CPAOptUIFlowTopWnd::Show()
{
    tstring strData = GetData();
    TRACE(strData.c_str());
    TRACE(_T("\n"));
    return true;

}

CDataSettingBase* CPAOptUIFlowTopWnd::CreateOptData()
{
    return new COptDataEnum(NULL, enDuty);
}