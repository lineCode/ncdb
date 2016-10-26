#include "StdAfx.h"
#include "MonitorCPU.h"
#include "CpuState.h"
CMonitorCPU::CMonitorCPU(void)
{
    ::memset(&m_data, 0, sizeof(MONITOR_CPU_DATA));
}


CMonitorCPU::~CMonitorCPU(void)
{
}

UseStatus CMonitorCPU::GetUseStatus()
{
    //获取
    UseStatus us = 0;
    m_data.fPercent = m_pCPU->GetCurrentCpuUsePercent();
    m_pMonitorResultData = &m_data;

    SetDataValid(true);
    return us;
}

bool CMonitorCPU::InitChild()
{
    m_pCPU = new CCpuState;
    m_pCPU->Init();

    //cpu 第一次获取的数据 由于时间问题是不准确的, 第二次开始的才是对的
    SetDataValid(false);
    SetType(mdt_cpu);
    return true;
    
}

