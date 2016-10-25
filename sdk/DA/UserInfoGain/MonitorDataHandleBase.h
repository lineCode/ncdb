#pragma once
#include "PiTypeDef.h"
#include "BackEndCollectDef.h"
#include "PAXmlUserHobbyDef.h"
#include <string>

//进程检测结果可处理类型
enum ENUM_FIELD
{
    f_count = 0,        //计数
    f_time              //时间计算
};

typedef map<ENUM_FIELD, ENUM_DATA_DUTY_UH> LST_FIELD_DUTY;

using namespace std;

class CPAXmlUserHobby;
class CTimeCalc;
typedef tstring MonitorTask;

class CMonitorDataHandleBase
{
public:
    CMonitorDataHandleBase(void);
    virtual ~CMonitorDataHandleBase(void);
public:
    
    bool Init();
    virtual bool HandleDetectResult( TimeType tTimeMonitor, bool bExist );
    virtual bool HandleCount( bool bExist );

    /************************************************************************
        fun:    逻辑上层处理: 增加时间
        param: 
        remark: 
    ************************************************************************/
    virtual bool HandleTimeDuration( bool bExist, TimeType tTimeMonitor );
    virtual MonitorTask GetMonitorTask() = 0;

    /************************************************************************
        fun:    实现增加指定的时间(s)到数据库里
        param: 
        remark: 
    ************************************************************************/
    virtual bool IncreaseTimesDuration(TimeType t );
    virtual bool InitDataDuty() = 0;
    virtual bool IncreaseCount();

    void SetDataDuty(ENUM_DATA_DUTY_UH duty);
    ENUM_DATA_DUTY_UH GetDataDuty();
    
    virtual tstring GetNewTimeStr( TimeType tAdd );
    bool AddFieldDuty( ENUM_FIELD enF, ENUM_DATA_DUTY_UH dd );

private:
    bool IsProcessNewCreate( bool bExist );
    void SaveDetectResult( bool bExist );
    bool BeginDealTimeDuration();
    bool BeginDealCount();

    bool SetDataDuty(ENUM_FIELD c);
    void TestTimeTransStr();
    
    bool IsCanIncreaseTime( bool bExist );
    void TestTimeCalc();
protected:
    CPAXmlUserHobby*                    m_pDB;
private:
    bool                        m_bExistPrev;
    ENUM_DATA_DUTY_UH           m_udDuty;
    auto_ptr<CTimeCalc>         m_pTimeCalc;
    LST_FIELD_DUTY              m_lstFieldDuty; //领域对应的职责
};

