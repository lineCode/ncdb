#pragma once
/************************************************************************
    fun:    事件信号对象
    remark:
************************************************************************/
#include "PiTypeDef.h"
#include <Windows.h>
class CEventLight
{
public:
	CEventLight(void);
	~CEventLight(void);

public:
	bool Init(tcpchar cName = NULL);
	bool UnInit();
	bool SetEventSign(bool bRet = true);
    
    /************************************************************************
        fun:    获取锁, 直到等待时间超时则返回
        param: 
        remark: true:正常超时; false://有外部影响事件
    ************************************************************************/
	bool GetLock(UNINT nWaitMax = INFINITE);
	bool Open(const char* cName);
	/************************************************************************
        fun:    设置等待事件超时, 单位毫秒
        param: 
        remark: 
    ************************************************************************/
	bool SetWaitTimeout( UNINT nWaitMaxMS = INFINITE);
    static bool IsExist( tcpchar cName );
private:
	HANDLE			m_hEventSync;
    UNINT           m_nWaitMax; //最大等待时间, 单位ms
    DWORD           m_dwErr;
    bool            m_bWaitRet; //等待结果, 用于通知外部等待成功或失败
};
