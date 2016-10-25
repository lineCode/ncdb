#include "StdAfx.h"
#include "AppProxy.h"

#include "PAUserSetting.h"
#include "PAOptDialog.h"
#include "OptionNotify.h"

CAppProxy::CAppProxy(void)
{
}


CAppProxy::~CAppProxy(void)
{
}

bool CAppProxy::Init()
{
   
    return true;
}

bool CAppProxy::ShowSettingDlg()
{
    //测试所有设置选项的读取 保存

    COptionNotify* pNotify = new COptionNotify;
    pNotify->Init();
    pNotify->AddNotify(op_general   , 0x100);
    pNotify->AddNotify(op_app       , 0x101);
    pNotify->AddNotify(op_update    , 0x102);
    pNotify->AddNotify(op_flow      , 0x103);

    CPAOptDialog dlgOpt;
    dlgOpt.SetNotify(pNotify);
    dlgOpt.Init();

    dlgOpt.ShowDefaultPage();
    dlgOpt.EnsureSetting();
    assert(op_general == dlgOpt.GetCurrentPage());

    dlgOpt.NextPage();
    dlgOpt.EnsureSetting();
    assert(op_app == dlgOpt.GetCurrentPage());

    dlgOpt.NextPage();
    dlgOpt.EnsureSetting();
    assert(op_update == dlgOpt.GetCurrentPage());

    dlgOpt.NextPage();
    dlgOpt.EnsureSetting();
    assert(op_flow == dlgOpt.GetCurrentPage());

    
    return true;
}
