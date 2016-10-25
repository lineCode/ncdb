#include "StdAfx.h"
#include "OPShareQQ.h"

COPShareQQ::COPShareQQ(void)
{
}

COPShareQQ::~COPShareQQ(void)
{
}

bool COPShareQQ::AddParamGeneral()
{
    //以下三个参数在每次请求必须带上
    std::string c_strTokenKey	= TXWeibo::Param::strTokenKey;
    //std::string c_strCustomKey	= TXWeibo::Param::strCustomKey;
    std::string c_strOpenid		= TXWeibo::Param::strOpenid;

    m_oParam.AddParam(c_strTokenKey.c_str(), m_strTokenKey.c_str());
    m_oParam.AddParam("oauth_consumer_key", m_opAppInfo.m_strAppID.c_str());
    m_oParam.AddParam(c_strOpenid.c_str(),m_strOpenid.c_str());

    //"&oauth_consumer_key=%s&access_token=%s&openid=%s&oauth_version=2.a&scope=all"
    return true;
}
