#pragma once
#include "BYStatHeader.h"
Pi_M_NameSpace_Begin(ns_by_stat)
class DLL_CLASS IByStat
{
	/*typedef ns_by_stat::ENUM_PHONE_CONNECT_STATE ENUM_PHONE_CONNECT_STATE;
	typedef ns_by_stat::ENUM_UNINSTALL_REASON ENUM_UNINSTALL_REASON;
	typedef ns_by_stat::tagPARAM tagPARAM;*/
	
	
public:
	virtual ~IByStat(){};
public:
	virtual bool Init() = 0;
/************************************************************************
    fun:	启动时调用, 获取系统信息并发送给服务器
    param:  
    ret:    true:收集发送成功
    remark: 
/************************************************************************/
	virtual bool CollectAppLunchyInfo() = 0;
	virtual bool CollectAppLunchyInfo_Sync() = 0;
/************************************************************************
    fun:	软件关闭时调用, 获取系统信息和结束时间并发送给服务器
    param:  
    ret:    true:收集发送成功
    remark: 
/************************************************************************/
	virtual bool CollectAppExitInfo() = 0;
	virtual bool CollectAppExitInfo_Sync() = 0;

/************************************************************************
    fun:	发送崩溃信息给服务器
    param:  
    ret:    true:收集发送成功
    remark: 
/************************************************************************/
	virtual bool CollectAppExceptionInfo(tstring strExceptionDesc) = 0;
	virtual bool CollectAppExceptionInfo_Sync(tstring strExceptionDesc) = 0;

/************************************************************************
    fun:	统计手机连接状态
    param:  
    ret:    true:收集发送成功
    remark: 
/************************************************************************/
	virtual bool CollectPhoneConnectState(ENUM_PHONE_CONNECT_STATE enState) = 0;
	virtual bool CollectPhoneConnectState_Sync(ENUM_PHONE_CONNECT_STATE enState) = 0;

/************************************************************************
    fun:	统计安装
    param:  
    ret:    true:收集发送成功
    remark: 
/************************************************************************/
	virtual bool CollectInstall() = 0;
	virtual bool CollectInstall_Sync() = 0;

/************************************************************************
    fun:	统计卸载
    param:  
    ret:    true:收集发送成功
    remark: 
/************************************************************************/
	virtual bool CollectUnInstall() = 0;
	virtual bool CollectUnInstall_Sync() = 0;

/************************************************************************
    fun:	统计升级情况
    param:  
    ret:    true:收集发送成功
    remark: 
/************************************************************************/
	virtual bool CollectUpdate() = 0;
	virtual bool CollectUpdate_Sync() = 0;

/************************************************************************
    fun:	统计卸载中的反馈信息
    param:  
    ret:    true:发送成功
    remark: 
/************************************************************************/
	virtual bool CollectUnInstallFeedback(int nReason) = 0;
	virtual bool CollectUnInstallFeedback_Sync(int nReason) = 0;
	virtual bool CollectUnInstallFeedback(tstring strReason, tstring strQQ) = 0;
	virtual bool CollectUnInstallFeedback_Sync(tstring strReason, tstring strQQ) = 0;

/************************************************************************
    fun:	统计应用下载情况
    param:  
    ret:    true:发送成功
    remark: 
/************************************************************************/
	virtual bool CollectDownloadSoft(tstring strAppName) = 0;
	virtual bool CollectDownloadSoft_Sync(tstring strAppName) = 0;

/************************************************************************
    fun:	统计音乐下载情况
    param:  
    ret:    true:发送成功
    remark: 
/************************************************************************/
	virtual bool CollectDownloadMusic(tstring strAppName) = 0;
	virtual bool CollectDownloadMusic_Sync(tstring strAppName) = 0;

/************************************************************************
    fun:	统计壁纸下载情况
    param:  
    ret:    true:发送成功
    remark: 
/************************************************************************/
	virtual bool CollectDownloadWP(tstring strAppName) = 0;
	virtual bool CollectDownloadWP_Sync(tstring strAppName) = 0;

/************************************************************************
    fun:	发送自定义事件
    param:  
    ret:    true:发送成
    remark: 
/************************************************************************/
	virtual bool SendEvent(tstring szLabelID,  tstring szName) = 0;
	virtual bool SendEvent(tagSTAT_CUSTOM_EVENT tagEvent) = 0;
	virtual bool SendEventList(LST_EVENT lstEvent) = 0;


	virtual bool SendEvent_Sync(tcpchar szLabelID,  tcpchar szName) = 0;
	virtual bool SendEvent_Sync(const tagSTAT_CUSTOM_EVENT& tagEvent) = 0;
	virtual bool SendEventList_Sync( const LST_EVENT& lstEvent) = 0;

	virtual tagPARAM* Param()   = 0;
	virtual void Param(const tagPARAM& val) = 0;
};

Pi_M_NameSpace_End
