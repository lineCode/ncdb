#pragma once
#include "downfilemanager.h"

class CTrDownFile;
class CProxyPC;
class CDMngProxy :
    public CDownFileManager
{
public:
    CDMngProxy(void);
    ~CDMngProxy(void);
public:
    virtual bool Init();
    virtual bool DealDownFail( CTrDownFile* pTrDown );
    virtual bool DoNewDown( CTrDownFile* pDown );
    bool SetAllDownProxy();
private:
    CProxyPC*               m_proxyUse; //ÒýÓÃ

};

