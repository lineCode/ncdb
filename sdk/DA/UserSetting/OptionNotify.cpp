#include "StdAfx.h"
#include "OptionNotify.h"
#include "define_gnl.h"

COptionNotify::COptionNotify(void)
{
}


COptionNotify::~COptionNotify(void)
{
}

bool COptionNotify::AddNotify( ENUM_OPTION_PAGE op, WndID wndID )
{
    //一个页面可以有多个监听者, 一个监听者可以监听多个页面
    
    LST_MONITOR::iterator itFind = m_lstMonitor.find(op);
    itFind->second.push_back(wndID);

    return true;
}

bool COptionNotify::Notify( ENUM_OPTION_PAGE op )
{
    LST_MONITOR::iterator itFind = m_lstMonitor.find(op);
    FOR_EACH_ITER(LST_WND, itFind->second, it)
    {
        WndID wndID = *it;
        //TODO:发送窗口消息,通知设置已更改
    }
    return true;
}

bool COptionNotify::Init()
{
    LST_WND lstWnd;
    m_lstMonitor.insert(pair<ENUM_OPTION_PAGE, LST_WND>(op_general, lstWnd));
    m_lstMonitor.insert(pair<ENUM_OPTION_PAGE, LST_WND>(op_app, lstWnd));
    m_lstMonitor.insert(pair<ENUM_OPTION_PAGE, LST_WND>(op_update, lstWnd));
    m_lstMonitor.insert(pair<ENUM_OPTION_PAGE, LST_WND>(op_flow, lstWnd));
    return true;
}
