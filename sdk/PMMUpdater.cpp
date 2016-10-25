#include "StdAfx.h"
#include "PMMUpdater.h"
#include "PathMapUpdater.h"
#include "ResCommon.h"


CPMMUpdater::CPMMUpdater(void)
{
    m_uSoftType = USOFT_PAINTAID;
    m_bDeleteSrc = true;
}


CPMMUpdater::~CPMMUpdater(void)
{
}


bool CPMMUpdater::Init()
{
    Create_Auto_Ptr(m_pathMap, CPathMapUpdater);
    m_pathMap->Init();
    return true;
}

tstring CPMMUpdater::GetMapPath( const tstring& strPath )
{
    return m_pathMap->GetMapPath(m_uSoftType, strPath);

}
