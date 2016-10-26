#pragma once

/************************************************************************
    fun:    时间计算类, 内部维护 时间和 是否有效的标识. 
    remark: 可用于在 需要 计算两个数值之间差异, 并且有开关可关闭/启动的情况
************************************************************************/
class CTimeCalc
{
    typedef time_t TimeType;
public:
    CTimeCalc(void);
    ~CTimeCalc(void);
public:
    bool SetInValid();
    //bool AddTime();
    bool SetOriginTime(TimeType t);
    bool SetValid();
    TimeType GetDiff( time_t tTimeMonitor );
    bool IsValid();
    void SetState( TimeType tTime, bool bValid );
private:
    bool                    m_bValid;   
    TimeType                m_tTimePrev;
};

