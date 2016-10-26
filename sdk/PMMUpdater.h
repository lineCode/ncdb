#pragma once
#include "pathmapmove.h"
class CPMMUpdater :
    public CPathMapMove<USOFT>
{
public:
    CPMMUpdater(void);
    ~CPMMUpdater(void);
public:
    virtual bool Init();
    void SetType(USOFT uSoftType);
    virtual tstring GetMapPath( const tstring& strPath );
private:
    USOFT           m_uSoftType;
};

