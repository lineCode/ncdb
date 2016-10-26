#pragma once
#include "ThreadLight.h"
#include <memory>

class CPAMonitorPrc;
class CEventLight;

using namespace std;
class CThreadDetection :
    public CThreadLight
{
public:
    CThreadDetection(CallBackFun pFun = NULL, void* pAdd = NULL);
    ~CThreadDetection(void);
public:
    bool Init();

    bool Start();
    bool Stop();


    bool SetPeriod(int nSecond);
private:
    virtual DWORD DoWork();
//     bool IsEnd();
//     bool WaitFor();
//     bool StopThread();
private:
    int                                     m_nDetectPeriodMS;
    bool                                    m_bWillStop;
    auto_ptr<CPAMonitorPrc>                 m_pMonitor;
    auto_ptr<CEventLight>                   m_pLockDetect;
};

