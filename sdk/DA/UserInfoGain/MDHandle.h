#pragma once
#include "monitordatahandlebase.h"

/****************************** sai进程检测结果处理类 *******************************/

class CMDHandleSai :
    public CMonitorDataHandleBase
{
public:
    CMDHandleSai(void);
    ~CMDHandleSai(void);
public:
    virtual MonitorTask GetMonitorTask();
    virtual bool InitDataDuty();
    virtual bool IncreaseTimesDuration(TimeType t );
    //virtual bool HandleCount( bool bExist );
    virtual bool HandleTimeDuration( bool bExist, TimeType tTimeMonitor );
    //virtual bool HandleDetectResult( time_t tTimeMonitor, bool bExist );
};


/****************************** PA进程检测结果处理类 ******************************************************/

class CMDHandlePA :
    public CMonitorDataHandleBase
{
public:
    CMDHandlePA(void);
    ~CMDHandlePA(void);
public:
    virtual MonitorTask GetMonitorTask();
    virtual bool InitDataDuty();
};


/********************************** PA数据处理 *************************************/

class CMDHPAssist :
    public CMonitorDataHandleBase
{
public:
    CMDHPAssist(void);
    ~CMDHPAssist(void);
public:
    virtual bool HandleCount();
    virtual bool InitDataDuty();
    virtual MonitorTask GetMonitorTask();
    bool SetCountType(ENUM_DATA_DUTY_UH );
    
private:
};




/********************************** PS 数据处理 *************************************/

class CMDHandlePS :
    public CMonitorDataHandleBase
{
public:
    CMDHandlePS(void);
    ~CMDHandlePS(void);
public:
    virtual bool InitDataDuty();
    virtual bool HandleTimeDuration( bool bExist, TimeType tTimeMonitor );
    virtual MonitorTask GetMonitorTask();

    
private:
};