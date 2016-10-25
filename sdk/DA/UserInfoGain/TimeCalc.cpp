#include "StdAfx.h"
#include "TimeCalc.h"


CTimeCalc::CTimeCalc(void)
{
    m_bValid        = false;
    m_tTimePrev     = 0;
}


CTimeCalc::~CTimeCalc(void)
{
}

bool CTimeCalc::SetOriginTime( TimeType t )
{
    m_tTimePrev = t;
    SetValid();
    return true;

}

CTimeCalc::TimeType 
    CTimeCalc::GetDiff( time_t tTimeMonitor )
{
    return tTimeMonitor - m_tTimePrev;
}

bool CTimeCalc::IsValid()
{   
    return m_bValid;

}

bool CTimeCalc::SetInValid()
{
    m_bValid = false;
    return true;
}

// bool CTimeCalc::AddTime()
// {
// 
//     return true;
// }

bool CTimeCalc::SetValid()
{
    m_bValid = true;
    return true;
}

void CTimeCalc::SetState( TimeType tTime, bool bValid )
{
    m_tTimePrev = tTime;
    m_bValid = bValid;
}

