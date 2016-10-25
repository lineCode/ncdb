#pragma once
#include "PASettingDef.h"
#include <map>
#include <list>
using namespace std;
typedef int WndID;
typedef list<WndID> LST_WND;
typedef map<ENUM_OPTION_PAGE, LST_WND> LST_MONITOR; 

class COptionNotify
{
public:
    COptionNotify(void);
    ~COptionNotify(void);
public:
    bool Init();
    bool AddNotify(ENUM_OPTION_PAGE op, WndID wndID);
    bool Notify(ENUM_OPTION_PAGE op);
private:
    LST_MONITOR     m_lstMonitor;
};

