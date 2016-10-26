#pragma once
#include "PASettingDef.h"
#include <list>
using namespace std;

class CPAOptUIPageBase;
class COptionNotify;
class CPAUserSetting;

typedef list<CPAOptUIPageBase*> LST_PAGE;

class CPAOptDialog
{
public:
    CPAOptDialog(void);
    ~CPAOptDialog(void);
public:
    bool Init();
    bool ShowDefaultPage();
    bool ChangePage(CPAOptUIPageBase* pPage);
    bool NextPage();
    void SetUserSetting(CPAUserSetting* pUS);
    bool SetNotify(COptionNotify*   pNotify);

    bool EnsureSetting();
    bool Cancel();
    bool Apply();
    bool ShowTplButton();
    ENUM_OPTION_PAGE GetCurrentPage();
    bool AdjustTplBtnPos();
    void ModifySetting();
    bool AddPage( CPAOptUIPageBase* pPage );
    bool SaveAllSetting();
private:
    LST_PAGE                        m_lstPage;
    int                             m_nCurPageIdx;
    int                             m_nTotalPage;
    CPAOptUIPageBase*               m_pCurPage;
    COptionNotify*                  m_pNotify;
    CPAUserSetting*                 m_UserSetting;
};

