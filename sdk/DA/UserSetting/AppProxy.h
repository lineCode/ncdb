#pragma once

class CPAUserSetting;

class CAppProxy
{
public:
    CAppProxy(void);
    ~CAppProxy(void);
public:
    bool Init();
    bool ShowSettingDlg();
private:
    CPAUserSetting*         m_pUserSetting;
};

