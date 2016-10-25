#pragma once
#include "PiTypeDef.h"
#include "ResCommon.h"
#include "Lock.h"
class CProxyUse
{
public:
    CProxyUse(void);
    virtual ~CProxyUse(void);
public:
    bool Init();
    virtual bool InitData() = 0;
    tstring GetNextProxy();
    tstring GetProxy();
    bool SetProxyList(const LST_STRING& lstProxy);
private:
    LST_STRING      m_lstProxyServer;
    LST_STRING::iterator    m_itProxyCur;
    auto_ptr<CLock>         m_pLockLstProxyServer;

};

