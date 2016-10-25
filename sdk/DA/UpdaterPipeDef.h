#pragma once

#define STRING_PIPE_PREFIX          _T("\\\\.\\pipe\\")
//消息类型
enum ENUM_PIPE_DATA_TYPE
{
    dt_update_progress = 0, //更新进度
    dt_state_notify,        
    dt_error,        

    //请求
    dt_notify_exit,            //请求宿主退出
    dt_notify_down_complete,    //通知宿主下载完成, 是否更新
    dt_notify_upgrade,           //是否要更新
    dt_notify_version_newest,   //版本最新无需更新

    //反馈
    dt_update_now,          //立即更新
    dt_update_after,        //下次更新

    dt_upgrade_now,         //升级
    dt_upgrade_after,       //下次升级
};

enum ENUM_UPDATER_ERROR_TYPE
{
    u_error_internet = 0,
    u_error_update,

};

struct tagUPDATE_ERROR
{
    const ENUM_PIPE_DATA_TYPE dType;
    ENUM_UPDATER_ERROR_TYPE nErrType;
    tagUPDATE_ERROR()
        :dType(dt_error)
    {
    }
};

struct tagUPDATE_PERCENT
{
    const ENUM_PIPE_DATA_TYPE dType;
    float nPercent;
    int nSpeed;     // 1kb/s
    tagUPDATE_PERCENT()
        :dType(dt_update_progress)
    {
    }
};

//错误, 流程状态通知
struct tagSTATE_NOTIFY
{
    const ENUM_PIPE_DATA_TYPE dType ;
    int nState;
    tagSTATE_NOTIFY()
        :dType(dt_state_notify)
    {

    }
};