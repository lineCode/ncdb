#pragma once
#include "monitordatahandlebase.h"
class CMDHDAssist :
    public CMonitorDataHandleBase
{
public:
    CMDHDAssist(void);
    ~CMDHDAssist(void);
public:
    bool SetCountType(ENUM_DATA_DUTY_UH );
    bool HandleCount();
    virtual bool InitDataDuty();
private:
    ENUM_DATA_DUTY_UH           m_dutyCount;    //计数的数据类型
};

