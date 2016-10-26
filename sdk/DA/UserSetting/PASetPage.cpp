#include "StdAfx.h"
#include "PASetPage.h"
#include "PAOptionUI.h"
#include "OptionNotify.h"
#include "PAUserSetting.h"
#include "define_gnl.h"

#define FOR_EACH_ITER_SDUI(Iter) FOR_EACH_ITER(LST_OPT_UI, m_lstOptUI, Iter)

CPAOptUIPageBase::CPAOptUIPageBase( COptionNotify* pNotify, CPAUserSetting* pUserSet)
{
    m_pOptNotify = pNotify;
    m_pSettingCol = pUserSet;
    m_PageFlag = op_general;
}


CPAOptUIPageBase::~CPAOptUIPageBase(void)
{
}

bool CPAOptUIPageBase::Init()
{
    InitSetItem();
    //ShowNewest();
    //InitLayout();
    
    return true;
}

bool CPAOptUIPageBase::Notify()
{
    return m_pOptNotify->Notify(m_PageFlag);
}

bool CPAOptUIPageBase::Show()
{
    FOR_EACH_ITER_SDUI(it)
    {
        (*it)->Show();
    }
    
    return true;
}

bool CPAOptUIPageBase::ShowNewest()
{
    FOR_EACH_ITER_SDUI(it)
    {
        (*it)->ReadData();
    }

    FOR_EACH_ITER_SDUI(it)
    {
        (*it)->Show();
    }
    return true;
}

bool CPAOptUIPageBase::SaveAllSetting()
{
    SaveAllConfig();
    FOR_EACH_ITER_SDUI(it)
    {
        (*it)->SetConfig(); //保存数据, 但不写入文件
    }
    
    m_pSettingCol->SaveDB();    //最后一次性写入,提高效率
    return true;
}

ENUM_OPTION_PAGE CPAOptUIPageBase::GetPageFlag()
{
    return m_PageFlag;
}

void CPAOptUIPageBase::ModifySetting()
{
    LST_OPT_UI::iterator it = m_lstOptUI.begin();
    (*it)->SaveSetting();
}


bool CPAOptUIPageBase::AddOpt( CPAOptUIBase* pOptUI )
{
    if(!pOptUI->InitData())
    {
        return false;
    }
    m_lstOptUI.push_back(pOptUI);
    return true;
}

void CPAOptUIPageBase::SaveAllConfig()
{
    
}

bool CPAOptUIPageBase::ReadAllSetting()
{
    return true;

}

bool CPAOptUIPageBase::WriteAllSetting( void* pData )
{

    return true;
}




/************************************************************************
    通用设置页面
************************************************************************/

CPAOptUIPageGeneral::CPAOptUIPageGeneral( COptionNotify* pNotify , CPAUserSetting* pUserSet )
    :CPAOptUIPageBase(pNotify, pUserSet)
{

}

bool CPAOptUIPageGeneral::InitSetItem()
{
    AddOpt(new CPAOptUILaunchAsBoot(m_pSettingCol));
    AddOpt(new CPAOptUICloseApp(m_pSettingCol));
    AddOpt(new CPAOptUITask(m_pSettingCol));

    m_PageFlag = op_general;
    return true;
}

bool CPAOptUIPageGeneral::ReadAllSetting(void* pData)
{
    //取出所有的参数, 
    //把数据 一一对应赋值到结构体里
    CSettingGeneralParam gParam;
    m_SetParam;
    int nType = 0;
    switch(nType)
    {
    case 0:
        //选项跟字段对应起来
        CPAOptUIBase* pUI = NULL;
        pUI->ReadData();
        pUI->GetData();
        //gParam.m_bAutoOpen;
        break;
    }
    CSettingGeneralParam* pParam =  (CSettingGeneralParam*)pData;
    return true;

}

bool CPAOptUIPageGeneral::WriteAllSetting( void* pData )
{
    //取出所有的参数, 
    //把数据 一一对应赋值到结构体里
    

    int nType = 0;
    switch(nType)
    {
    case 0:
        //选项跟字段对应起来

        //CPAOptUILaunchAsBoot.SetData(pData->m_bAutoOpen);
        break;
    }
    return true;
}

/************************************************************************
    应用设置页面
************************************************************************/

CPAOptUIPageApp::CPAOptUIPageApp( COptionNotify* pNotify , CPAUserSetting* pUserSet )
    :CPAOptUIPageBase(pNotify, pUserSet)
{

}

bool CPAOptUIPageApp::InitSetItem()
{
    AddOpt(new CPAOptUIAppSaveOpt(m_pSettingCol));
    AddOpt(new CPAOptUIAppSavePath(m_pSettingCol));

    AddOpt(new CPAOptUIDriverSaveOpt(m_pSettingCol));
    AddOpt(new CPAOptUIDriverSavePath(m_pSettingCol));

    AddOpt(new CPAOptUIDesktopRecordPath(m_pSettingCol));
    AddOpt(new CPAOptUIWorksSavePath(m_pSettingCol));
    AddOpt(new CPAOptUIFontLibSavePath(m_pSettingCol));

    m_PageFlag = op_app;
    return true;
}




/************************************************************************
    升级设置页面
************************************************************************/

CPAOptUIPageUpdate::CPAOptUIPageUpdate( COptionNotify* pNotify , CPAUserSetting* pUserSet )
    :CPAOptUIPageBase(pNotify, pUserSet)
{

}

bool CPAOptUIPageUpdate::InitSetItem()
{
    AddOpt(new CPAOptUIUpdateType(m_pSettingCol));
    m_PageFlag = op_update;
    return true;
}



/************************************************************************
    浮窗设置页面
************************************************************************/

CPAOptUIPageFlow::CPAOptUIPageFlow( COptionNotify* pNotify , CPAUserSetting* pUserSet )
    :CPAOptUIPageBase(pNotify, pUserSet)
{

}

bool CPAOptUIPageFlow::InitSetItem()
{
    AddOpt(new CPAOptUIFlowWndAsBoot(m_pSettingCol));
    AddOpt(new CPAOptUIFlowTopWnd(m_pSettingCol));

    m_PageFlag = op_flow;
    return true;
}
