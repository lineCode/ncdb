#include "StdAfx.h"
#include "MDHDAssist.h"
#include "MonitorDataHandleBase.h"
#include <assert.h>
//#include "DBData.h"


CMDHDAssist::CMDHDAssist(void)
{
}


CMDHDAssist::~CMDHDAssist(void)
{
}

bool CMDHDAssist::HandleCount()
{
    assert(GetDataDuty() != dnh_unKnown);

    this->IncreaseCount();
    return true;

}

bool CMDHDAssist::SetCountType( ENUM_DATA_DUTY_UH dd)
{
    SetDataDuty(dd);
    return true;

}

bool CMDHDAssist::InitDataDuty()
{
    SetDataDuty(dnh_unKnown);   //该类处理多种职责的数据, 需要外部SetCountType设置职责类型
    return true;
}

