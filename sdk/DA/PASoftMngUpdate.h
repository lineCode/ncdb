#pragma once
#include "PathMap.h"
#include "ListMng.h"

typedef CListMng<tagStrPair, LST_PATH_PAIR> CLstPath;
class CPASoftMngUpdate
{
public:
    CPASoftMngUpdate(USOFT nSoftType);
    ~CPASoftMngUpdate(void);
public:
    bool Init();
    bool ParseFileUpdatePath( tstring strPathConfig, CLstPath& lstPath);
    bool UpdateFile(CLstPath& lstPath );
    bool UpdateSmall(tstring strSrcZip);
    tstring ParsePathMap(const tstring& strPath );
public:
private:
    USOFT               m_nSoftType;
    CPathMap            m_pathMap;
};

