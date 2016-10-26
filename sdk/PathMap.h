#pragma once
#include "PathMapDef.h"
#include "doString.h"

#define STRING_PATH_MAP_FLAG            _T("?")

template<class T>
class CPathMap
{
public:
    
    typedef map<tagPATH_MAP<T>, tstring> PATH_MAP;

    CPathMap();
    ~CPathMap(void);
public:
    virtual bool Init();
    tstring GetMapPath( T key, const tstring& strPath );
protected:
    PATH_MAP        m_mapPath;
    CUSoftMgr       m_uSoftMng;
    
    //USOFT           m_nSoftType;
};

template<class T>
CPathMap<T>::~CPathMap( void )
{

}

template<class T>
CPathMap<T>::CPathMap()
{

}




template<class T>
tstring CPathMap<T>::GetMapPath( T key, const tstring& strPath )
{
    if (strPath.empty())
    {
        return _T("");
    }
    tstring strPathReal(strPath);
    tstring strMap;
    int nPos = GetContentBetweenChar(strPath, strMap, STRING_PATH_MAP_FLAG );
    if( !IsValidStrPos(nPos) )
    {
        return strPathReal;
    }
    if (strMap.empty())
    {
        return strPathReal;
    }
    tagPATH_MAP<T> pm(key, strMap);
    if(! m_mapPath.count(pm) )
    {
        return _T("");
    }
    return m_mapPath[pm] + strPath.substr(NextPos(nPos));
}

template<class T>
bool CPathMap<T>::Init()
{
    return true;
}
