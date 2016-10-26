#pragma once
#include "InetDef.h"
using namespace ns_down_file;

//下载中的任务信息,通过回调函数传出去
struct DowningInfoUpdate
{
    float fPercent;     //进度, 百分比, 小数点, 0.73
    UNINT nSpeed;           //速度, 单位时间里多少字节
    UNLONG nSizeDowned;     //已下载大小
    UNLONG nSizeTotal;      //总大小
};

struct DowningItemInfo
{
    /*ns_down_file::*/ItemID ID;
    UNLONG  dwDowned;        //已下载字节数
    UNLONG  dwDownedCircle; //上一个周期到现在下载的字节数
    UNLONG  dwTotal;        //文件总字节数
    CInternetHttp*        pInet;
    bool    bShow;      //是否显示
    DowningItemInfo()
    {
        bShow = true;
        dwDowned        = 0;
        ID              = 0;
        dwDownedCircle  = 0;
        dwTotal         = 0;
        pInet           = NULL;
    }
};

