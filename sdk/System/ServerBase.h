#pragma once
/*------------------------------------------------------
	Fun:	服务控制器模块, 简化服务的创建
	remark:	步骤:
				1. 从本类派生一个对象, 实现接口DoTask执行服务的任务 在程序入口处(win32为main) 调用Start接口
				2. 可以设置m_nPeriodMils的值改变执行周期, 单位毫秒
	Authon: liwanliang, QQ:396257680
	Date:	2016/04/01
------------------------------------------------------*/
#include "PiTypeDef.h"
#include "SingleLog.h"
#include <Windows.h>
class CServerBase
{
public:
	CServerBase(void);
	virtual ~CServerBase(void);
protected:
	/*------------------------------------------------------------
			fun:	做一些耗时不久的初始化操作
			ret:	
			param: 
			remark:	
	------------------------------------------------------------*/
	virtual bool InitThread();
	/*------------------------------------------------------------
			fun:	执行服务任务, 周期执行该接口
			ret:	
			param: 
			remark:	
	------------------------------------------------------------*/
	virtual bool DoTask();
	/*------------------------------------------------------------
			fun:	服务即将退出时调用, 用来做退出反初始化操作
			ret:	
			param: 
			remark:	
	------------------------------------------------------------*/
	virtual void OnExit();

public:
	/*------------------------------------------------------------
			fun:	开始执行服务
			ret:	
			param: 
			remark:	调用线程会一直等待, 直到本进程内所有服务退出后才返回
	------------------------------------------------------------*/
	bool Start();
	
protected:
	bool Init();
	static VOID WINAPI ServiceMain(DWORD dwArgc,LPTSTR* lpszArgv);
	static VOID WINAPI ServerHandler(DWORD fdwControl);
protected:
	tstring				m_strServiceName;	//如果程序里只有1个服务, 则不用指定, 在外部创建服务时指定
	bool				m_bWriteLog;
	UNINT				m_nPeriodMils;		//定时执行任务的周期, 单位毫秒
};
