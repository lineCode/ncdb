#include "StdAfx.h"
#include "TimeLight.h"
#include <assert.h>
#include <comutil.h>
#pragma comment(lib, "Winmm.lib")

CTimeLight::CTimeLight(void)
{
    m_dwTimeBegin           = 0;
    m_dwTimeEnd             = 0;
    m_dwFrequencyPerSecond  = 0;
}

CTimeLight::~CTimeLight(void)
{
}

bool CTimeLight::Init()
{
    //支持64位的编译器中使用QuadPart 
    LARGE_INTEGER liTemp = {0,0};
    if(!QueryPerformanceFrequency(&liTemp))
    {
        return false;
    }
    m_dwFrequencyPerSecond = liTemp.QuadPart;
    return true;
}

void CTimeLight::Start()
{
    LARGE_INTEGER liTemp = {0, 0};
    if(!QueryPerformanceCounter(&liTemp))
    {
        assert(0 && "get high-resolution performance counter fail");
        //int i = GetLastError();
    }
    m_dwTimeBegin = liTemp.QuadPart;
}

void CTimeLight::Stop()
{
    LARGE_INTEGER liTemp = {0, 0};
    if(!QueryPerformanceCounter(&liTemp))
    {
        assert(0 && "get high-resolution performance counter fail");
        //int i = GetLastError();
    }
    m_dwTimeEnd = liTemp.QuadPart;
}

DWORD CTimeLight::GetPass()
{
    return (m_dwTimeEnd - m_dwTimeBegin)/ (m_dwFrequencyPerSecond);
}

double CTimeLight::GetPassMS()
{
    return (m_dwTimeEnd - m_dwTimeBegin)/ (m_dwFrequencyPerSecond / (double)1000);
}

DWORD CTimeLight::Pass()
{
	Stop();
	return GetPass();
}

DWORD CTimeLight::PassMS()
{
	Stop();
	return GetPassMS();
}

bool CTimeLight::IsStart()
{
	return m_dwTimeBegin != 0;
}

tstring CTimeLight::GetCurrentTick()
{
	tchar szBuf[12] = {0};
	_ltot_s(GetTickCount(), szBuf, _countof(szBuf), 10);
	return szBuf;
}

tstring CTimeLight::GetPassStr()
{
	DWORD dwMS = PassMS();
	_bstr_t bstrt(dwMS);
	return (tstring)bstrt;
}
