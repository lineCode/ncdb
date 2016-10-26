#include "StdAfx.h"
#include "PathMapUpdater.h"
#include "PathLight.h"
#include "UCommon.h"


CPathMapUpdater::CPathMapUpdater(void)
{
}


CPathMapUpdater::~CPathMapUpdater(void)
{
}

bool CPathMapUpdater::Init()
{
    m_uSoftMng.Init();
    AddPath(USOFT_PAINTAID);
    return true;    
}

bool CPathMapUpdater::AddPath( USOFT nSoftType )
{
    CString strPath = m_uSoftMng.GetExePath(nSoftType);
    if (strPath.IsEmpty())
    {
        return false;
    }

    m_mapPath[tagPATH_MAP<USOFT>(nSoftType, _T("self"))] = CPathLight::GetParentPathFromPath(strPath);
    m_mapPath[tagPATH_MAP<USOFT>(nSoftType, _T("MainAppData"))] = CFolderUtil::GetPaUserMainFolder();
    return true;
}