#pragma once
#include "PAOptionUIBase.h"

/************************************************************************
    关闭应用程序的处理
/************************************************************************/

class CPAOptUICloseApp :
    public CPAOptUIBase
{
    static const ENUM_DATA_DUTY enDuty = dn_close_app;
public:
    CPAOptUICloseApp(CPAUserSetting* pSC );
    ~CPAOptUICloseApp(void);
public:
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};




/************************************************************************
    是否随系统启动
/************************************************************************/

class CPAOptUILaunchAsBoot :
    public CPAOptUIBase
{
public:
    static const ENUM_DATA_DUTY enDuty = dn_launch_as_boot;
    CPAOptUILaunchAsBoot(CPAUserSetting* pSC );
    ~CPAOptUILaunchAsBoot(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};




/************************************************************************
    是否保存应用安装包
/************************************************************************/

class CPAOptUIAppSaveOpt :
    public CPAOptUIBase
{
    static const ENUM_DATA_DUTY enDuty = dn_app_save_setting;
public:
    CPAOptUIAppSaveOpt(CPAUserSetting* pSC );
    ~CPAOptUIAppSaveOpt(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};


/************************************************************************
    是否保存应用安装包
/************************************************************************/

class CPAOptUIAppSavePath :
    public CPAOptUIBase
{
    static const ENUM_DATA_DUTY enDuty = dn_app_save_dir;

public:
    CPAOptUIAppSavePath(CPAUserSetting* pSC );
    ~CPAOptUIAppSavePath(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};



/************************************************************************
    任务, 是否跟随绘画软件启动
/************************************************************************/

class CPAOptUITask :
    public CPAOptUIBase
{
    static const ENUM_DATA_DUTY enDuty = dn_task;

public:
    CPAOptUITask(CPAUserSetting* pSC );
    ~CPAOptUITask(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};



/************************************************************************
    驱动保存设置,
/************************************************************************/

class CPAOptUIDriverSaveOpt :
    public CPAOptUIBase
{
    static const ENUM_DATA_DUTY enDuty = dn_driver_save_setting;

public:
    CPAOptUIDriverSaveOpt(CPAUserSetting* pSC );
    ~CPAOptUIDriverSaveOpt(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};


/************************************************************************
    驱动保存目录,
/************************************************************************/

class CPAOptUIDriverSavePath :
    public CPAOptUIBase
{
    static const ENUM_DATA_DUTY enDuty = dn_driver_save_dir;

public:
    CPAOptUIDriverSavePath(CPAUserSetting* pSC );
    ~CPAOptUIDriverSavePath(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};



/************************************************************************
    桌面录制目录,
/************************************************************************/

class CPAOptUIDesktopRecordPath :
    public CPAOptUIBase
{
public:
    static const ENUM_DATA_DUTY enDuty = dn_desktop_record_path;

    CPAOptUIDesktopRecordPath(CPAUserSetting* pSC );
    ~CPAOptUIDesktopRecordPath(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};


/************************************************************************
    作品保存目录,
/************************************************************************/

class CPAOptUIWorksSavePath:
    public CPAOptUIBase
{
public:
    static const ENUM_DATA_DUTY enDuty = dn_works_save_path;

    CPAOptUIWorksSavePath(CPAUserSetting* pSC );
    ~CPAOptUIWorksSavePath(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};



/************************************************************************
    字库保存目录,
/************************************************************************/

class CPAOptUIFontLibSavePath:
    public CPAOptUIBase
{
public:
    static const ENUM_DATA_DUTY enDuty = dn_fontLib_save_path;

    CPAOptUIFontLibSavePath(CPAUserSetting* pSC );
    ~CPAOptUIFontLibSavePath(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};



/************************************************************************
    升级方式
/************************************************************************/

class CPAOptUIUpdateType:
    public CPAOptUIBase
{
public:
    static const ENUM_DATA_DUTY enDuty = dn_update_type;

    CPAOptUIUpdateType(CPAUserSetting* pSC );
    ~CPAOptUIUpdateType(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};



/************************************************************************
    软件启动时开启浮窗
/************************************************************************/

class CPAOptUIFlowWndAsBoot:
    public CPAOptUIBase
{
public:
    static const ENUM_DATA_DUTY enDuty = dn_flow_auto_open;

    CPAOptUIFlowWndAsBoot(CPAUserSetting* pSC );
    ~CPAOptUIFlowWndAsBoot(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};




/************************************************************************
    始终保持在其他窗口前端
/************************************************************************/

class CPAOptUIFlowTopWnd:
    public CPAOptUIBase
{
public:
    static const ENUM_DATA_DUTY enDuty = dn_flow_top_wnd;

    CPAOptUIFlowTopWnd(CPAUserSetting* pSC );
    ~CPAOptUIFlowTopWnd(void);
public:
    virtual bool DoGetSaevData(tstring& strData );
    virtual bool Show();
    static CDataSettingBase* CreateOptData();
};