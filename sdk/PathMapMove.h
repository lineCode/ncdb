#pragma once
#include "PiTypeDef.h"
#include "PathMapDef.h"
#include "ListMng.h"
#include "PathMap.h"
#include "PathLight.h"
#include "UCommon.h"
#include <assert.h>

typedef CListMng<tagStrPair, LST_PATH_PAIR> CLstPath;
template<class TKey>
class CPathMapMove
{
public:
    CPathMapMove(void);
    virtual ~CPathMapMove(void);
    bool UpdateFile();
    bool ParsePathMap( const tstring& strPath, const tstring& strFileName);
    virtual bool Init();
    void SetSrcDir( const tstring& strSrcDir );
    virtual tstring GetMapPath( const tstring& strPath );
protected:
    auto_ptr<CPathMap<TKey>>            m_pathMap;
    CLstPath            m_lstPath;
    tstring             m_strSrcDir;
    bool                m_bDeleteSrc;   //移动后是否删除源文件
};

template<class TKey>
CPathMapMove<TKey>::~CPathMapMove( void )
{

}

template<class TKey>
CPathMapMove<TKey>::CPathMapMove( void )
{
    m_bDeleteSrc = false;
}

template<class TKey>
bool CPathMapMove<TKey>::ParsePathMap( const tstring& strPath, const tstring& strFileName )
{
    tstring strPathReal;
    //m_strPathNative = strPath;
    strPathReal = GetMapPath(strPath);
    //strPathReal =  m_pathMap.GetMapPath((USOFT)0 /*m_nSoftType*/, strPath);
    if ( ! CPathLight::IsValidPath(strPathReal.c_str()) )
    {
        return false;
    }

    //合成完整路径
    tagStrPair* pPair = new tagStrPair;
    pPair->strSrc = m_strSrcDir + _T("\\") + strFileName;
    pPair->strDist = strPathReal;
    m_lstPath.Add(pPair);

    return true;
}

template<class TKey>
bool CPathMapMove<TKey>::UpdateFile()
{
    CLstPath& lstPath = m_lstPath;
    tagStrPair* pPair = NULL;
    while(pPair = lstPath.Next())
    {
        CDirectoryUtil::CreateMultipleDirectory(TO_CSTRING(pPair->strDist));
        if(! CFileUtil::CopyFile(TO_CSTRING(pPair->strSrc), TO_CSTRING(pPair->strDist)))
        {
            return false;
        }
        if (m_bDeleteSrc)
        {
            CFileUtil::DeleteFile(TO_CSTRING(pPair->strSrc));
        }
    }

    return true;
}

template<class TKey>
bool CPathMapMove<TKey>::Init()
{
    assert(0 && _T("子类实现"));;
    /*Create_Auto_Ptr(m_pathMap, CPathMap<TKey>);
    //m_pathMap = new CPathMap<TKey>;
    m_pathMap.Init();*/
    
    return true;
}

template<class TKey>
void CPathMapMove<TKey>::SetSrcDir( const tstring& strSrcDir )
{
    m_strSrcDir = strSrcDir;
}

template<class TKey>
tstring CPathMapMove<TKey>::GetMapPath( const tstring& strPath )
{
    assert(0 && _T("需要子类实现该接口"));
    tstring strTemp;
    //strPathReal =  m_pathMap.GetMapPath((USOFT)0 /*m_nSoftType*/, strPath);
    return strTemp;
}
