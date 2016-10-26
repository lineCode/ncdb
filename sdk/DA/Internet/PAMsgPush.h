#pragma once
#include "PAMsgPushDef.h"
#include "piTypeDef.h"
#include "ResCommon.h"
#include "ThreadLight.h"
class CPARestNews;
class CNewsMng;
class CPAMsgPush :public CThreadLight
{
public:
    CPAMsgPush(HWND hwnd = NULL);
    ~CPAMsgPush(void);
public:
    bool Init();
    bool UnInit();
    bool GetMsg(ENUM_MSG_TYPE msgType, CNewsMng& newsMng);
    virtual DWORD DoWork();
    
protected:
    CPARestNews* GetRestfulObj( ENUM_MSG_TYPE msgType );
    bool ResetQuestCycle( int nCycle );
    bool NotifyUI();
private:
    auto_ptr<CNewsMng>           m_pNewsMng;
    HWND                m_hwnd;
};

