#include "StdAfx.h"
#include "MonitorSystemBase.h"
#include "PiTypeDef.h"

CMonitorSystemBase::CMonitorSystemBase(void)
{
    m_bDataValid            = false;
    //m_UseStatus             = 0;
    m_WndNotify             = NULL;
    m_pMonitorResultData    = NULL;
}


CMonitorSystemBase::~CMonitorSystemBase(void)
{
}



bool CMonitorSystemBase::Init()
{
    //初始化自身

    //初始化子类
    InitChild();
    return true;

}

bool CMonitorSystemBase::AddNotifyWnd( WndID wndID )
{
    m_WndNotify = wndID;
    return true;
}

bool CMonitorSystemBase::Notify()
{
    tstring strMsg;
    const int nBuf = 100;
    strMsg.resize(nBuf);
    //_stprintf_s(&strMsg.at(0), nBuf, _T("CMonitorSystemBase notify, type:%d, value:%f\n"), m_enDataType, m_UseStatus);
    //TRACE(strMsg.c_str());

    ::PostMessage(m_WndNotify, UM_MONITOR_DATA, m_enDataType, (LPARAM)m_pMonitorResultData);

    return true;

}

bool CMonitorSystemBase::IsDataValid()
{

    return m_bDataValid;

}

bool CMonitorSystemBase::InitChild()
{
    return true;
}

bool CMonitorSystemBase::SetDataValid( bool bValid )
{
    m_bDataValid = bValid;
    return true;

}

void CMonitorSystemBase::SetType( ENUM_MONITOR_DATA_TYPE dt )
{
    m_enDataType = dt;
}
