#include "StdAfx.h"
#include "PAOptDialog.h"
#include "PASetPage.h"
#include "OptionNotify.h"
#include "PAUserSetting.h"

CPAOptDialog::CPAOptDialog(void)
{
    m_nCurPageIdx = 0;
}


CPAOptDialog::~CPAOptDialog(void)
{
}

bool CPAOptDialog::Init()
{
    m_UserSetting = new CPAUserSetting;
    m_UserSetting->Init();

    
    //AddPage(pPage)
    AddPage(new CPAOptUIPageGeneral (m_pNotify, m_UserSetting));
    AddPage(new CPAOptUIPageApp     (m_pNotify, m_UserSetting));
    AddPage(new CPAOptUIPageUpdate  (m_pNotify, m_UserSetting));
    AddPage(new CPAOptUIPageFlow    (m_pNotify, m_UserSetting));

    
    return true;
}

bool CPAOptDialog::EnsureSetting()
{
    Apply();
    Cancel();
    return true;
}

bool CPAOptDialog::Cancel()
{
    //关闭设置窗口
    return true;
}

bool CPAOptDialog::Apply()
{
    m_pCurPage->SaveAllSetting();
    m_pCurPage->Notify();
    return true;
}


bool CPAOptDialog::ChangePage( CPAOptUIPageBase* pPage )
{
    pPage->ShowNewest();
    m_pCurPage = pPage;

    AdjustTplBtnPos();
    ShowTplButton();
    return true;
}

bool CPAOptDialog::ShowTplButton()
{
    //TODO: 显示3个模板按钮
    return true;

}

bool CPAOptDialog::AdjustTplBtnPos()
{
    return true;
}

bool CPAOptDialog::ShowDefaultPage()
{
    //TODO:显示第一个页面
    m_pCurPage = *(m_lstPage.begin());
    m_pCurPage->ShowNewest();

    AdjustTplBtnPos();
    ShowTplButton();
    
    return true;
    
}

bool CPAOptDialog::NextPage()
{
    ++m_nCurPageIdx;

    LST_PAGE::iterator it = m_lstPage.begin();
    advance(it, m_nCurPageIdx);
    CPAOptUIPageBase* pNext = *(it);
    
    ChangePage(pNext);
    return true;
}

bool CPAOptDialog::SetNotify( COptionNotify* pNotify )
{
    m_pNotify = pNotify;
    return true;
}

ENUM_OPTION_PAGE CPAOptDialog::GetCurrentPage()
{
    return m_pCurPage->GetPageFlag();
}

void CPAOptDialog::ModifySetting()
{
    m_pCurPage->ModifySetting();
}

void CPAOptDialog::SetUserSetting( CPAUserSetting* pUS )
{
    m_UserSetting = pUS;
}

bool CPAOptDialog::AddPage( CPAOptUIPageBase* pPage )
{
    if(!pPage->Init())
    {
        return false;
    }
    m_lstPage.push_back(pPage);
    
    return true;
}

bool CPAOptDialog::SaveAllSetting()
{
    //找到每个page, 对应起某个字段
//     CPAOptUIPageBase* pPage = NULL;
//     pPage->SaveAllSetting()
    return true;

}
