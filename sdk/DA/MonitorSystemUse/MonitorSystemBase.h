#pragma once
#include "MonitorSystemDef.h"
class CMonitorSystemBase
{
public:
    CMonitorSystemBase(void);
    virtual ~CMonitorSystemBase(void);
public:
    virtual UseStatus GetUseStatus() = 0;

    bool Init();
    bool AddNotifyWnd(WndID wndID);
    bool Notify();
    bool IsDataValid();

    bool SetDataValid(bool bValid);

    /**************************************** 方法 ****************************************/
protected:
    void SetType(ENUM_MONITOR_DATA_TYPE dt);

private:
    virtual bool InitChild();
    /**************************************** 属性 ****************************************/
protected:
    //UseStatus               m_UseStatus;
    void*                   m_pMonitorResultData;
    bool                            m_bDataValid;   //数据是否有效, 无效则不发送出去

private:
    WndID       m_WndNotify;
    ENUM_MONITOR_DATA_TYPE          m_enDataType;
};

