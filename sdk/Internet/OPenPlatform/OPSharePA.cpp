#include "StdAfx.h"
#include "OPSharePA.h"
#include "pinetrestful.h"
#include "PAOPShare.h"


COPSharePA::COPSharePA(void)
{
}


COPSharePA::~COPSharePA(void)
{
}

string COPSharePA::GetRequestCodeUrl()
{
    //从服务器获取, 并赋值
    tagOP_APP_INFO ai;
    ai = CPAOPShare::QueryAppInfo(m_pl_type);
    if (ai.m_strAppID.empty())
    {
        return "";
    }
    m_opAppInfo = ai;

    /*tstring strWeb;
    if( CPiNetRestful::SendSync(_T(""), _T("param"), Restful_Get, strWeb) )
    {
        //解析出数据结构
        tagOP_APP_INFO tagAI;
        if(ParseAppInfo(strWeb, tagAI))
        {
            m_opAppInfo = tagAI;
        }
    }*/

    return COPShareBase::GetRequestCodeUrl();
}

bool COPSharePA::ParseAppInfo( const tstring& strWeb, tagOP_APP_INFO& ai )
{
    
    return true;
}

tagOP_APP_INFO COPSharePA::QueryAppInfo()
{
    tagOP_APP_INFO ai;
    ai = CPAOPShare::QueryAppInfo(st_sina);

    return ai;
}
