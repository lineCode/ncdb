#include "StdAfx.h"
#include "MDHandle.h"

/****************************** sai进程检测结果处理类 *******************************/

CMDHandleSai::CMDHandleSai(void)
{
}


CMDHandleSai::~CMDHandleSai(void)
{
}


bool CMDHandleSai::InitDataDuty()
{
    AddFieldDuty(f_count, dnh_sai_use_count);
    return true;

}

bool CMDHandleSai::IncreaseTimesDuration( TimeType t )
{
    //不计算使用时间
    return true;
}

MonitorTask CMDHandleSai::GetMonitorTask()
{
    
    //return _T("sai.exe");
    return _T("gvim.exe");//TODO:test

}

bool CMDHandleSai::HandleTimeDuration( bool , TimeType)
{
    /* 不计算使用时间*/
    return true;
}




/****************************** PA进程检测结果处理类 *******************************/

CMDHandlePA::CMDHandlePA(void)
{
}


CMDHandlePA::~CMDHandlePA(void)
{
}

bool CMDHandlePA::InitDataDuty()
{
    AddFieldDuty(f_count,   dnh_da_use_count);
    AddFieldDuty(f_time,    dnh_da_use_time);
    //SetDataDuty(f_count, dnh_sai_use_count);
    return true;

}

MonitorTask CMDHandlePA::GetMonitorTask()
{
    //return _T("pa.exe");
    return _T("QQ.exe");//TODO:TEST
}







/****************************** ps进程检测结果处理类 *******************************/

CMDHandlePS::CMDHandlePS(void)
{
}


CMDHandlePS::~CMDHandlePS(void)
{
}


bool CMDHandlePS::InitDataDuty()
{
    AddFieldDuty(f_count, dnh_ps_use_Count);
    return true;

}

MonitorTask CMDHandlePS::GetMonitorTask()
{
    
    //return _T("photoshop.exe");
    return _T("explorer.exe");//TODO:test

}

bool CMDHandlePS::HandleTimeDuration( bool , TimeType)
{
    /* 不计算使用时间*/
    return true;
}



/********************************** PA数据处理 *************************************/


CMDHPAssist::CMDHPAssist(void)
{
}


CMDHPAssist::~CMDHPAssist(void)
{
}

bool CMDHPAssist::HandleCount()
{
    assert(GetDataDuty() != dnh_unKnown);

    this->IncreaseCount();
    return true;

}

bool CMDHPAssist::SetCountType( ENUM_DATA_DUTY_UH dd)
{
    SetDataDuty(dd);
    return true;

}

bool CMDHPAssist::InitDataDuty()
{
    SetDataDuty(dnh_unKnown);   //该类处理多种职责的数据, 需要外部SetCountType设置职责类型
    return true;
}

MonitorTask CMDHPAssist::GetMonitorTask()
{
    assert(0 && "虚接口, 为了编译通过, 外部不会调用");
    MonitorTask taskSai = _T("xxx.exe");
    return taskSai;
}



