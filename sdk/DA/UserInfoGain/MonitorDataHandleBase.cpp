#include "StdAfx.h"
#include "MonitorDataHandleBase.h"
#include "PAXmlUserHobby.h"
#include "TimeCalc.h"
#include "functional.h"
#include "AutoType.h"
#include "ResCommon.h"

CMonitorDataHandleBase::CMonitorDataHandleBase(void)
{
    m_udDuty            = dnh_unKnown;
    m_pDB               = NULL;
    m_bExistPrev        = false;
}


CMonitorDataHandleBase::~CMonitorDataHandleBase(void)
{
}

bool CMonitorDataHandleBase::Init()
{
     TestTimeCalc();
     TestTimeTransStr();

    //return true;

    InitDataDuty();
    Create_Auto_Ptr(m_pTimeCalc, CTimeCalc);
    m_pDB = CPAXmlUserHobby::GetInstance();
    return true;

}

bool CMonitorDataHandleBase::HandleDetectResult( TimeType tTimeMonitor, bool bExist )
{
    //m_timeMonitor = tTimeMonitor;/* 设置为当前时间*/
    
    HandleCount(bExist);
    HandleTimeDuration(bExist, tTimeMonitor);

    SaveDetectResult(bExist);
    
    return true;    
}

bool CMonitorDataHandleBase::IncreaseCount()
{
    m_pDB->AddMath(m_udDuty, 1);
    return true;

}

bool CMonitorDataHandleBase::IncreaseTimesDuration( TimeType tAdd )
{
    CAutoType at(tAdd);
    TRACE((tcpchar)at);
    TRACE(_T("\n"));

    if(tAdd <= 0)
    {
        return true;
    }

    //转换下时间

    tstring strData = GetNewTimeStr(tAdd);

    m_pDB->WriteData(m_udDuty, strData);
    return true;

}

void CMonitorDataHandleBase::SetDataDuty(ENUM_DATA_DUTY_UH duty)
{
    m_udDuty = duty;
}

bool CMonitorDataHandleBase::SetDataDuty(ENUM_FIELD c)
{
    LST_FIELD_DUTY::iterator itFind = m_lstFieldDuty.find(c);
    if (itFind != m_lstFieldDuty.end())
    {
        SetDataDuty(itFind->second);
        return true;
    }
    return false;
}

tstring CMonitorDataHandleBase::GetNewTimeStr( TimeType tAdd )
{
    tstring strData;
    m_pDB->ReadData(m_udDuty, strData);
//     TRACE(strData.c_str());
//     TRACE(_T("\n"));

    TimeType tTime = 0;
    if(!TimeStrToSecondHighHour(strData.c_str(), &tTime))
    {
        tTime = 0;
    }

    //增加时间
    tTime += tAdd;

    strData = ::SecondToTimeStrHighHour(tTime);
    TRACE(strData.c_str());
    TRACE(_T("\n"));

    return strData;
}

bool CMonitorDataHandleBase::IsProcessNewCreate( bool bExist )
{
    /*根据之前的状态, 判断进程是不是被用户新启动了一次:
        进程新创建条件 = 这次存在 && 上次也存在
    */
    return (bExist 
        && ! m_bExistPrev);
 
}

bool CMonitorDataHandleBase::HandleCount( bool bExist )
{    
    if(!BeginDealCount())
    {
        return false;
    }
    if(IsProcessNewCreate(bExist))
    {
        IncreaseCount();
    }
    return true;
}

bool CMonitorDataHandleBase::HandleTimeDuration( bool bExist, TimeType tTimeMonitor )
{
    /************************************************************************
        使用时间计算逻辑: 通过一个计算CTimeCalc时间对象, 内部维护 时间和 是否有效的标识
            每次都会更新时间, 并设置是否有效
            检测出结果后, 先根据这次和CTimeCalc 是否有效(上次检测结果) 得出这次是否可以叠加时间
    ************************************************************************/
    if(!BeginDealTimeDuration())
    {
        return false;
    }
    if(IsCanIncreaseTime(bExist))
    {
        //增加使用时间
        //时间锚点重置为新的时间
        TimeType tPass = m_pTimeCalc->GetDiff(tTimeMonitor);
        IncreaseTimesDuration(tPass);
    }

    //设置时间为最新时间
    /*根据结果设置时间是否有效, 
        如果无效, 则上面设置的时间不起作用; 
        如果有效, 则时间起了作用;   减少外部判断逻辑
    */

    m_pTimeCalc->SetState(tTimeMonitor, bExist);
    
    return true;

}

void CMonitorDataHandleBase::SaveDetectResult( bool bExist )
{
    m_bExistPrev = bExist;
}

ENUM_DATA_DUTY_UH CMonitorDataHandleBase::GetDataDuty()
{
    return m_udDuty;
}

bool CMonitorDataHandleBase::BeginDealTimeDuration()
{
    return SetDataDuty(f_time);
}

bool CMonitorDataHandleBase::BeginDealCount()
{
    return SetDataDuty(f_count);
}

bool CMonitorDataHandleBase::AddFieldDuty( ENUM_FIELD enF, ENUM_DATA_DUTY_UH dd )
{
    m_lstFieldDuty.insert(pair<ENUM_FIELD, ENUM_DATA_DUTY_UH>(enF, dd));
    return true;
}

void CMonitorDataHandleBase::TestTimeTransStr()
{
    //test: 时间转字符串
    
    tstring strTemp;
    strTemp = _T("00:00:01");
    assert(strTemp == ::SecondToTimeStrHighHour(1));

    strTemp = _T("00:00:41");
    assert(strTemp == ::SecondToTimeStrHighHour(41));
    strTemp = _T("00:01:21");
    assert(strTemp == ::SecondToTimeStrHighHour(81));
    strTemp = _T("00:11:21");
    assert(strTemp == ::SecondToTimeStrHighHour(11 * 60 + 21));
    strTemp = _T("01:11:21");
    assert(strTemp == ::SecondToTimeStrHighHour( (60 + 11) * 60 + 21));

    strTemp = _T("11:11:21");
    assert(strTemp == ::SecondToTimeStrHighHour( (60 * 11 + 11) * 60 + 21));

    //test_end: 时间转字符串
}

bool CMonitorDataHandleBase::IsCanIncreaseTime( bool bExist )
{
    //存在, 上次存在
    return bExist && m_pTimeCalc->IsValid();
}

void CMonitorDataHandleBase::TestTimeCalc()
{
    CTimeCalc* pBackup = m_pTimeCalc.get();
    CTimeCalc tc;
    Create_Auto_Ptr(m_pTimeCalc, CTimeCalc);
    *m_pTimeCalc = tc;

    time_t tNow = 3;
    bool bExist = false;

    {//不存在, 上次不存在 = 不变
        tNow += 10;
        bExist = false;
        assert(false == IsCanIncreaseTime(bExist));
        m_pTimeCalc->SetState(tNow, bExist);

    }

    {//存在, 上次不存在 = 不变, 开始计时
        tNow += 10;
        bExist = true;
        assert(false == IsCanIncreaseTime(bExist)); 
        m_pTimeCalc->SetState(tNow, bExist);
        
    }
    
    {//存在, 上次存在 = 增加时间
        time_t tBegin = tNow;
        tNow += 10;
        bExist = true;
        assert(true == IsCanIncreaseTime(bExist)); 
        assert( 10 == m_pTimeCalc->GetDiff(tNow));
        m_pTimeCalc->SetState(tNow, bExist);

    }

    {//不存在, 上次存在 = 不变
        tNow += 10;
        bExist = false;
        assert(false == IsCanIncreaseTime(bExist)); 
        m_pTimeCalc->SetState(tNow, bExist);

    }


    if(IsCanIncreaseTime(bExist))
    {
        //增加使用时间
        //时间锚点重置为新的时间
        TimeType tPass = m_pTimeCalc->GetDiff(tNow);
        IncreaseTimesDuration(tPass);
    }

    m_pTimeCalc->SetState(tNow, bExist);

    //delete m_pTimeCalc;


    m_pTimeCalc.reset(pBackup);
}
