#pragma once
#include <windows.h>
#include "PiTypeDef.h"
/************************************************************************
/*  fun: 获取开机以来经过的毫秒数, 开机时间最大只能保存20天? ,有最大限制, 需要更改,参照MSDN
/************************************************************************/

class CTimeLight
{
public:
    CTimeLight(void);
    ~CTimeLight(void);
    bool    Init();
    void    Start();
    void    Stop();
    DWORD   GetPass();
    
    double  GetPassMS();
	DWORD Pass();
	DWORD PassMS();
	bool IsStart();
	static tstring GetCurrentTick();
	tstring GetPassStr();
private:
    LONGLONG       m_dwTimeBegin;
    LONGLONG       m_dwTimeEnd;
    LONGLONG      m_dwFrequencyPerSecond;
};
