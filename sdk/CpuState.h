#pragma once
#include <Windows.h>
#include <memory>
#define NUMBER_TIME_DATA_TYPE           7           //有几种需要单独保存时间数据的情况
class CHardwareQuery;
using namespace std;

class CCpuState
{
    enum TimeDataPos
    {
        pos_process = 0,
        pos_process_spend_pre,      //进程上次占用的时间
        pos_cpu,
        pos_cpu_idle_pre,       //cpu上次空闲的时间数, //两种算法
        pos_cpu2_idle_pre,    
        pos_tick_count_pre,     //上次计算的时钟主频时间点
        pos_cpu_userKernel_pre,     //上次的 sum(user+kernel) 的时间
    };
public:
    CCpuState(void);
    ~CCpuState(void);

public:
    /************************************************************************
        fun: 初始化操作
        parent: 
        remark: 
    ************************************************************************/
    bool Init();
    bool UnInit();
    /************************************************************************
        fun: 获取当前进程占用cpu 的百分比
        remark: 第一次调用会返回0. 占用率为两次时间间隔内的Cpu占用率,  取决于调用该接口的频率
    ************************************************************************/
    
    float GetCurrentProcessSpend();

    
     /************************************************************************
        fun:    获取当前cpu 空闲的百分比
        parent: 
        remark: 
    ************************************************************************/
    float GetCurrentCpuIdlePercent();
    float GetCurrentCpuUsePercent();

     /************************************************************************
        fun:    获取cpu空闲百分比
        parent: 
        remark: 结果为上次调用该接口到目前这段时间内算出的结果
    ************************************************************************/
    float GetCurrentCpuIdlePercentBad();
private:
     /************************************************************************
        fun:    获取距离上次的时间片段内, cpu的空闲时间数
        parent: 
        remark: 
    ************************************************************************/
    time_t CalcCpuIdleDiff();

    //另一个数据块
    time_t CalcCpuIdleDiff2();
    __int64 FileTimeToInt64(const FILETIME& time);
    time_t CalcProcessSpendTimeTotal();

     /************************************************************************
        fun:    计算进程当前占用时间和上次的差 并更新最新占用时间
        parent: 
        remark: 
    ************************************************************************/
    time_t CalcProcessSpendTimeDiff();
    time_t CalcCpuIdleTotal();
    time_t CalcCpuUserKernelTotal();
    /************************************************************************
        fun: 返回和上次的时间差 并记录下当前时间
        parent: 
        remark: 
    ************************************************************************/
    time_t CalcTimePass(time_t& nOld);
private:
    time_t          m_timeOldArr[NUMBER_TIME_DATA_TYPE];
    HANDLE          m_hProcess;

    float           m_fCpuUsePercent;               //最近一次计算的cpu使用率
    float           m_fCpuUseSelfProcessPercent;    //最近一次计算的 本进程的cpu使用率
    float           m_fCpuIdlePercent;               //最近一次计算的cpu空闲率

    auto_ptr<CHardwareQuery>    m_pHardwareQuery;
};

