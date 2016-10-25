#pragma once
#include "PASettingDef.h"
#include <list>
#include "SettingParam.h"
using namespace std;



class CPAOptUIBase;
class CPAXmlUserConfig;
class CPAUserSetting;
class COptionNotify;

typedef list<CPAOptUIBase*> LST_OPT_UI;

class CPAOptUIPageBase
{
public:
    CPAOptUIPageBase(COptionNotify* pNotify, CPAUserSetting* pUserSet );
    ~CPAOptUIPageBase(void);
public:
    bool Init();
    bool Notify();
    bool SaveAllSetting();
    bool ShowNewest();
    bool Show();
    void ModifySetting();
    bool AddOpt( CPAOptUIBase* pOptUI );
    ENUM_OPTION_PAGE GetPageFlag();
    virtual bool ReadAllSetting();
    virtual bool WriteAllSetting(void* pData);
protected:
    //virtual bool InitLayout() = 0;
    virtual bool InitSetItem() = 0;
    void SaveAllConfig();
protected:
    CPAUserSetting*         m_pSettingCol;
    ENUM_OPTION_PAGE        m_PageFlag;
private:
    LST_OPT_UI              m_lstOptUI;
    COptionNotify*          m_pOptNotify;

};



/************************************************************************
    设置页面1, 常规设置
/************************************************************************/
class CPAOptUIPageGeneral
    :public CPAOptUIPageBase
{
public:
    CPAOptUIPageGeneral(COptionNotify* pNotify, CPAUserSetting* pUserSet );
    ~CPAOptUIPageGeneral(void);
public:
    virtual bool ReadAllSetting(void* pData);
    virtual bool WriteAllSetting(void* pData );
protected:
    virtual bool InitSetItem();
private:
    CSettingGeneralParam        m_SetParam;
//     CDASetUIBase*           m_dsLaunchAsBoot;
//     CDASetUIBase*           m_dsCloseApp;
};




/************************************************************************
    设置页面2, 应用设置
/************************************************************************/
class CPAOptUIPageApp
    :public CPAOptUIPageBase
{
public:
    CPAOptUIPageApp(COptionNotify* pNotify, CPAUserSetting* pUserSet );
    ~CPAOptUIPageApp(void);
public:

protected:
    virtual bool InitSetItem();
private:
    CSettingAppParam        m_paramApp;
};



/************************************************************************
    设置页面3, 升级设置页面
/************************************************************************/
class CPAOptUIPageUpdate
    :public CPAOptUIPageBase
{
public:
    CPAOptUIPageUpdate(COptionNotify* pNotify, CPAUserSetting* pUserSet );
    ~CPAOptUIPageUpdate(void);
public:

protected:
    virtual bool InitSetItem();
private:
};



/************************************************************************
    设置页面4, 浮窗设置页面
/************************************************************************/
class CPAOptUIPageFlow
    :public CPAOptUIPageBase
{
public:
    CPAOptUIPageFlow(COptionNotify* pNotify, CPAUserSetting* pUserSet );
    ~CPAOptUIPageFlow(void);
public:

protected:
    virtual bool InitSetItem();
private:
};