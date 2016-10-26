#include "StdAfx.h"
#include "OPLogin.h"
#include "OPParamMng.h"
#include "Markup.h"
#include "XmlUtil.h"
#include "StrCoding.h"


COPLogin::COPLogin(ENUM_OP_LOGIN_TYPE loginType)
{
    m_login_type = loginType;
    m_SendType = Restful_Get;
}


COPLogin::~COPLogin(void)
{
}

tstring COPLogin::GetUrl()
{
    return COPParamMng::GetUrlLoginAuth();
}

MAP_STRING COPLogin::GetParam()
{
    return COPParamMng::GetParamLoginAuth(m_login_type);
}

tstring COPLogin::ExtractRetUrl()
{
    //截取url
    tstring strUrl;
    tstring strWebData;
    CPARestful::GetDistWebData(strWebData);

    if( ! CXmlUtil::GetChildElemData(strWebData.c_str(), _T("url"), strUrl))
    {
        return strUrl;
    }
    
    //url解码
    strUrl = CStrCoding::DecodeBase64Str_T(strUrl.c_str());
   // strUrl = CStrCoding::UrlDecode(strUrl); // modify:xuemingzhi [2014/04/22]:不需要这部操作
    return strUrl;
}

bool COPLogin::DoParam()
{
    /* 重写, 屏蔽父类的加密操作*/
    return true;
}

tstring COPLogin::QueryUrlAuth( ENUM_OP_LOGIN_TYPE lType )
{    
    COPLogin opLogin(lType);
    if( ! opLogin.Send())
    {
        return _T("");
    }
    return opLogin.ExtractRetUrl();
}
