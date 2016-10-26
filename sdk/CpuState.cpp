#include "StdAfx.h"
#include "CpuState.h"
#include <Windows.h>
#include <time.h>
#include <iostream>
#include "HardwareQuery.h"
#include "Math/mathUnit.h"
using namespace std;
Pi_NameSpace_Using



CCpuState::CCpuState(void)
{
    m_hProcess      = NULL;
    m_fCpuUseSelfProcessPercent = NULL;
    memset(m_timeOldArr, 0, sizeof(m_timeOldArr[0]) * NUMBER_TIME_DATA_TYPE);
}


CCpuState::~CCpuState(void)
{
    UnInit();
}

bool CCpuState::Init()
{
    m_pHardwareQuery = auto_ptr<CHardwareQuery>(new CHardwareQuery);
    m_pHardwareQuery->QueryInfo();

    m_hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());
    CalcProcessSpendTimeDiff();
    CalcCpuIdleDiff();
    return true;
}

bool CCpuState::UnInit()
{
    if (m_hProcess)
    {
        ::CloseHandle(m_hProcess);
        m_hProcess = NULL;
    }   
    return true;
}

__int64 CCpuState::FileTimeToInt64( const FILETIME& time )
{    
    ULARGE_INTEGER tt;
    tt.LowPart = time.dwLowDateTime;
    tt.HighPart = time.dwHighDateTime;
    return tt.QuadPart;
}

float CCpuState::GetCurrentProcessSpend()
{
    /*TODO: 由于卡时间接口在各自获取的接口里调用, 会导致第二个开始的接口 时间被第一个接口给更新了, 
            可以把该接口放在外部统一调用, 或者分别在各自接口里保存时间点
    */

    time_t nExpend = CalcProcessSpendTimeDiff();
    time_t nSpend = CalcTimePass(m_timeOldArr[pos_process]);

    
    //cout<<"经过的时间:"<<nSpend<<endl;

    if (nSpend > 0)// 时间间隔至少为该值才计算, 否则没多大意义
    {
        m_fCpuUseSelfProcessPercent = INT_TO_FLOAT(NANOSECOND100_TO_NANOSECOND(nExpend)) 
            / MILLISECOND_TO_NANOSECOND( nSpend) * 100.0;
    }
    
    return m_fCpuUseSelfProcessPercent /m_pHardwareQuery->GetCpuThreadCountTotal();
}

time_t CCpuState::CalcTimePass( time_t& nOld )
{
    //总经过时间
    DWORD dwTime = GetTickCount();
    DWORD tSpend = dwTime - static_cast<DWORD>(nOld);
    nOld = dwTime;
    return tSpend;
}

time_t CCpuState::CalcProcessSpendTimeTotal()
{
    FILETIME ftCreate   = {0};
    FILETIME ftExit     = {0};
    FILETIME ftKernel   = {0};
    FILETIME ftUser     = {0};

    if(!::GetProcessTimes(m_hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser))
    {
        return 0;
    }

    //假设进程消耗cpu时间 = 内核时间 + 用户时间
    //该进程总消耗时间

    __int64 nExpend = FileTimeToInt64(ftKernel) + FileTimeToInt64(ftUser);
    return nExpend;
}

time_t CCpuState::CalcProcessSpendTimeDiff()
{
    time_t nTotal = CalcProcessSpendTimeTotal();
    cout<<"进程消耗时间:" <<nTotal<<endl;
    time_t nDiff = nTotal - m_timeOldArr[pos_process_spend_pre];
    m_timeOldArr[pos_process_spend_pre] = nTotal;
    return nDiff;
}

float CCpuState::GetCurrentCpuIdlePercent()
{
    time_t nIdleDiff = CalcCpuIdleDiff();

    //算出经过时间, 空闲时间/经过时间 = cpu空闲百分比

    time_t nUserKernelNow = CalcCpuUserKernelTotal();
    time_t nUserKernelDiff = nUserKernelNow - m_timeOldArr[pos_cpu_userKernel_pre];
    m_timeOldArr[pos_cpu_userKernel_pre] = nUserKernelNow;

    //cout<<"空闲经过时间2: "<< NANOSECOND100_TO_MILLISECOND(nUserKernelDiff * 1.0)<<endl;

    //cpu使用率
    if (nUserKernelDiff)
    {
        m_fCpuUsePercent = (INT_TO_FLOAT(nUserKernelDiff) - nIdleDiff)/ nUserKernelDiff * 100;
    }
    cout<< "利用率:" << m_fCpuUsePercent;

    //cpu空闲率
    if(nUserKernelDiff > 0)
    {
        m_fCpuIdlePercent = INT_TO_FLOAT(NANOSECOND100_TO_NANOSECOND(nIdleDiff)) 
            / NANOSECOND100_TO_NANOSECOND(nUserKernelDiff) * 100;
    }
    return m_fCpuIdlePercent;
    
}

time_t CCpuState::CalcCpuIdleTotal()
{
    enum {err = 0};
    FILETIME ftIdle     = {0};
    FILETIME ftKernel   = {0};
    FILETIME ftUser     = {0};
    if(!::GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
    {
        return err;
    }
    //cout<<"idle"<<FileTimeToInt64(ftIdle) << " kernel+User" <<FileTimeToInt64(ftKernel)
        //+ FileTimeToInt64(ftUser) <<endl;
    __int64 nIdleTotal = FileTimeToInt64(ftIdle);

    return nIdleTotal;
}

time_t CCpuState::CalcCpuIdleDiff()
{
    //跟上次空闲对比 得出这段时间的空闲时间, 
    time_t nIdleTotal = CalcCpuIdleTotal();
    time_t nDiff = nIdleTotal - m_timeOldArr[pos_cpu_idle_pre];
    m_timeOldArr[pos_cpu_idle_pre] = nIdleTotal;
    //cout<<"idle经过时间: "<<NANOSECOND100_TO_MILLISECOND(nDiff)<<endl;
    return nDiff;
}

time_t CCpuState::CalcCpuIdleDiff2()
{
    //跟上次空闲对比 得出这段时间的空闲时间, 
    //static time_t nCpuIdleOld = 0;

    time_t nIdleTotal = CalcCpuIdleTotal();
    time_t nDiff = nIdleTotal - m_timeOldArr[pos_cpu2_idle_pre];
    m_timeOldArr[pos_cpu2_idle_pre] = nIdleTotal;
    //cout<<"idle经过时间2: "<<NANOSECOND100_TO_MILLISECOND(nDiff)<<endl;
    return nDiff;
}

float CCpuState::GetCurrentCpuIdlePercentBad()
{
    time_t nIdleDiff = CalcCpuIdleDiff2();

    //算出经过时间, 空闲时间/经过时间 = cpu空闲百分比
    time_t nSpendPass = CalcTimePass(m_timeOldArr[pos_cpu]);

    static time_t nKernelPre = 0;
    static time_t nUserPre = 0;

    //cout<<"空闲经过时间: "<< nSpendPass<<endl;;
    float nPercent = 0;
    if(nSpendPass > 0)
    {
        nPercent = 1.0 * NANOSECOND100_TO_NANOSECOND(nIdleDiff) / MILLISECOND_TO_NANOSECOND(nSpendPass) * 100;
    }
    return nPercent / m_pHardwareQuery->GetCpuThreadCountTotal();
}

time_t CCpuState::CalcCpuUserKernelTotal()
{
    FILETIME ftIdle     = {0};
    FILETIME ftKernel   = {0};
    FILETIME ftUser     = {0};
    if(!::GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
    {
        return 0;
    }
    //cout<<"-2idle:"<<FileTimeToInt64(ftIdle) << " kernel+User:" <<FileTimeToInt64(ftKernel)
    //+ FileTimeToInt64(ftUser) <<endl;
    return FileTimeToInt64(ftKernel) + FileTimeToInt64(ftUser);;
}

float CCpuState::GetCurrentCpuUsePercent()
{
    return NUM_PERCENT_MAX - GetCurrentCpuIdlePercent();   //计算使用率
}
