#pragma once
#include "proxyuse.h"
#include "ResCommon.h"
class CLock;
class CProxyPC :
    public CProxyUse
{
public:
    CProxyPC(void);
    ~CProxyPC(void);
public:
    virtual bool InitData();
    static bool ReleaseInst();
    static CProxyPC* GetInst();
    static auto_ptr<CLock>  m_pLock;
    static auto_ptr<CProxyPC>        m_pInst;
};

