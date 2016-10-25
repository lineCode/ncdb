#pragma once
#include "PiTypeDef.h"
Pi_M_NameSpace_Begin(ns_by_stat)

#define STRING_LABELID_INSTALL		_T("install")
#define STRING_LABELID_UNINSTALL	_T("uninst")
#define STRING_LABELID_UPDATE		_T("update")
#define STRING_LABELID_DOWNLOAD_SOFT	_T("PCdownload")
#define STRING_LABELID_DOWNLOAD_MUSIC	_T("MusicDownload")
#define STRING_LABELID_DOWNLOAD_WP		_T("WPdownload")

enum ENUM_PHONE_CONNECT_STATE
{
	connect_usb_success					= 0,	//USB连接成功
	connect_usb_start					,		//USB开始连接
	connect_usb_start_no_check_port		,		//USB开始连接(不检测设备类型)
	connect_wifi_success				,		//WIFI连接成功
	connect_wifi_start					,		//WIFI开始连接
};

enum ENUM_UNINSTALL_REASON
{
	reason_cannot_connect		= 1,		//手机助手连接不上
	reason_no_androw			= 2 << 1	,	//我使用的不是安卓手机
	reason_download_app			= 2 << 2	,	//下载的应用有问题
	reason_bad_download_speed	= 2 << 3	,	//资源下载速度慢
	reason_res_lack				= 2 << 4	,	//找不到想要的资源
	reason_use_other			= 2 << 5	,	//已有其他手机管理软件
	reason_other				= 2 << 6	,	//其他原因+QQ号码
};

//自定义事件
struct tagSTAT_CUSTOM_EVENT
{
	tstring strLabelID;	
	tstring strName;	
public:
	tagSTAT_CUSTOM_EVENT(tcpchar szLabelId =_T(""), tcpchar szName =_T(""))
		:strLabelID(szLabelId)
		,strName(szName){}
};


struct tagEXCEPTION
{
	tstring strExceptionDesc;
};

//外部可以修改的参数结构
struct tagPARAM
{
	tstring	strServer;		//统计服务器, 如http://tj1.3310.com
	tstring	strAESKey;
	tstring	strAESIV;		//内部写好, 外部无需设置
	tstring	strProductID;	//产品ID

	tstring	strChannel;			// 渠道
	tstring strSoftVersion;		//软件版本
	tagPARAM()
		:strChannel(_T("0000"))
	{

	}
};

typedef list<tagSTAT_CUSTOM_EVENT>	LST_EVENT;
typedef map<ENUM_UNINSTALL_REASON, tstring>	MAP_REASON;
Pi_M_NameSpace_End
