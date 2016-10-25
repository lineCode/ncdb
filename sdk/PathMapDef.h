#pragma once
#include "PiTypeDef.h"
#include "USoftMgr.h"

struct tagStrPair
{
    tstring strSrc;
    tstring strDist;
};
typedef list<tagStrPair*> LST_PATH_PAIR;


template<class T>
struct tagPATH_MAP
{
    T nSoftType;
    tstring strMap;
    tagPATH_MAP()
    {
        
    }
    tagPATH_MAP(T nSoftTypeR, tstring strMapR)
    {
        nSoftType = nSoftTypeR;
        strMap = strMapR;
    }
    bool operator<(const tagPATH_MAP& pm) const
    {
        return nSoftType < pm.nSoftType || strMap < pm.strMap;
    }
};

/*
template<class T>
typedef map<T, tstring> PATH_MAP;*/