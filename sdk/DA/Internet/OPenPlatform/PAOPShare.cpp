#include "StdAfx.h"
#include "PAOPShare.h"


CPAOPShare::CPAOPShare(ENUM_OP_SHARE_TYPE type)
{
    m_pl_type = type;
}


CPAOPShare::~CPAOPShare(void)
{
}

tstring CPAOPShare::GetUrl()
{
    return _T("http://www.baidu.com");
}

bool CPAOPShare::DoParam()
{
    return true;

}

MAP_STRING CPAOPShare::GetParam()
{
    MAP_STRING mapStr;
    mapStr[_T("type")] = _T("1");
    return mapStr;
}


tagOP_APP_INFO CPAOPShare::QueryAppInfo( ENUM_OP_SHARE_TYPE lType )
{
    tagOP_APP_INFO tap;
    CPAOPShare paShare(lType);
    if(! paShare.Send())
    {
        return tap;
    } 
    if( paShare.ParseOPAppInfo() )
    {
        tap = paShare.m_OpAppInfo;
    }
    return tap;
}

bool CPAOPShare::ParseOPAppInfo()
{
    m_strWebData;
    return true;
}
