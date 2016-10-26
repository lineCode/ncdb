#pragma once
#include "PiTypeDef.h"
#include <string>
#include <list>
#include "define_gnl.h"

/*******************************
    网络模块调试信息宏
*******************************/
#define MACRO_PI_INET_DEBUG_INFO

using std::string;
using std::list;

class CInternetHttp;
class CTrDownFile;

namespace ns_down_file
{
    typedef void (*CBDowningInfo)(void*, void*);
    typedef int ItemID;
    typedef CInternetHttp CDownItem;
}

enum TaskState
{
    ts_new_add = 0          ,
    ts_downing              ,
    ts_retry                ,
    ts_done                 ,
};

enum DownState
{
	ok_completion = 0,
    ok_downing,
	err_net,				//网络问题引起的错误
	err_local_file,			//本地文件操作
    err_file_create,        //创建文件
    err_file_write,         //写文件
    err_server_404,         //请求的网页404 错误
	err_server_403,         //请求的网页405 错误, 被服务器拒绝屏蔽了
    err_proxy,              //代理引起的错误
    err_user_cancel,        //用户取消下载
    err_user_stop,          //用户暂停下载
    err_get_file_size,      //获取文件大小失败
	err_server,
	err_too_big,	
	err_unKnown,
};


struct DownFileInfo
{
public:
    //using ns_down_file::ItemID;

	tstring strUrl;
	tstring strSavePath;
	DownState	dState;	//下载状态
    TaskState   taskState;  //任务状态
    UNINT       nServerCode;
    UNINT       nErrCode;   //错误代码
    bool        bUseServerName; //是否使用服务器上的名字作为本地保存文件名
    UNINT       nAddi;  //附加数据
    bool        bNeedBPDown;    //该任务是否采用断点续传
    ns_down_file::ItemID      itemID;     //每个任务的唯一标识

    DownFileInfo()
    {
        dState          = err_unKnown;
        taskState       = ts_new_add;
        bUseServerName  = false;
        itemID          = 0;
        nAddi           = NULL;
        nServerCode     = 0;
    	nErrCode		= 0;   //错误代码
        bNeedBPDown     = false;
    }
    
};

typedef list<DownFileInfo>	DownList;
typedef list<CTrDownFile*>	DownObjList;


//遍历容器的每个元素
#define FOR_EACH_DOWN_TASK(Cn, Iter)\
    FOR_EACH_ITER(DownList, Cn, Iter)
