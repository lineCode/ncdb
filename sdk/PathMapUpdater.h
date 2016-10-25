#pragma once
#include "pathmap.h"
class CPathMapUpdater :
    public CPathMap<USOFT>
{
public:
    CPathMapUpdater(void);
    ~CPathMapUpdater(void);
public:
    bool Init();
    bool AddPath( USOFT nSoftType );
};

