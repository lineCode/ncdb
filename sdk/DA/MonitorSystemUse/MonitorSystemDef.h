#pragma once
#include <windows.h>
//用户消息定义
#define UM_MONITOR_DATA         WM_USER + 100

typedef HWND    WndID;
typedef float     UseStatus;

enum ENUM_MONITOR_DATA_TYPE
{
    mdt_cpu = 0,
    mdt_memory
};

struct MONITOR_CPU_DATA
{
    float       fPercent;
};

struct MONITOR_MEMORY_DATA
{
    DWORDLONG       nBytesTotal;
    DWORDLONG       nBytesFree; //可用内存
};