#pragma once
#include "ThreadLight.h"
#include "PiTypeDef.h"
class CEventLight;
class CProcessDetect:public CThreadLight
{
public:
    CProcessDetect(bool bDetectLunchy = false);
    ~CProcessDetect(void);
public:
    virtual DWORD DoWork();
    bool Init();
    bool SetDetectProcess(tcpchar szProcess);
    bool SetDetectCircle(UNINT nMillSecond);

    bool WaitProcess();
protected:
    bool IsDetectProcessLaunch();
    bool IsNeedNotify( bool bExist );
private:
    auto_ptr<CEventLight>             m_pEvent;
    tstring                     m_strProcess;
    bool                        m_bDetectLunchy;  //true:检测进程开启, false:检测进程关闭
    UNINT                       m_DetectCircle;
};

