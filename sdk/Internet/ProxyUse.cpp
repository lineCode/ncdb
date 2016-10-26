#include "StdAfx.h"
#include "ProxyUse.h"
#include "define_gnl.h"
#include "RAIILock.h"
#include "functional.h"


CProxyUse::CProxyUse(void)
{
}


CProxyUse::~CProxyUse(void)
{
}

bool CProxyUse::Init()
{
    Create_Auto_Ptr(m_pLockLstProxyServer, CLock);
    m_itProxyCur = m_lstProxyServer.end();

    InitData();
    return true;
}

tstring CProxyUse::GetNextProxy()
{
    CRAIILock raii(m_pLockLstProxyServer->Lock());
    if(!IsValidIter(m_itProxyCur, m_lstProxyServer))
    {
        m_itProxyCur = m_lstProxyServer.begin();
    }
    else
    {
        ++m_itProxyCur;
    }
    OutInfo(_T("new proxyIP:"), m_itProxyCur->c_str());

    tstring strProxy;
    if( ! IsValidIter(m_itProxyCur, m_lstProxyServer))
    {
        OutInfo(_T("error, no more proxy"));
    }
    else
    {
        strProxy = *m_itProxyCur;
        //OutInfo(_T("new proxyIP:"), strProxy.c_str());
    }
    return strProxy;
}

tstring CProxyUse::GetProxy()
{
    CRAIILock raii(m_pLockLstProxyServer->Lock());
    tstring strProxy;
    if (IsValidIter(m_itProxyCur, m_lstProxyServer))
    {
        strProxy = *m_itProxyCur;
    }
    return strProxy;
}

bool CProxyUse::SetProxyList(const LST_STRING& lstProxy)
{
    CRAIILock raii(m_pLockLstProxyServer->Lock());
    m_lstProxyServer = lstProxy;
    return true;
}
