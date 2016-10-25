#include "StdAfx.h"
#include "OPShareTengxun.h"
#include "WeiboParam.h"
#include "StrCoding.h"
#include "functional.h"
#include "define_gnl.h"
#include "PiNetRestful.h"
#include "OPShareSina.h"
#include "PiURl.h"

#include "JsonHeader.h"
#include "JsonUtil.h"

const TCHAR*  wszErrorString[] = {L"成功",L"请求超时",L"不能解析地址",L"不能连接",L"错误",L"返回的数据量太大",L"失败",L"类型错误，无法生成有效的url"};

#define STRING_URL_AUTH         "https://open.t.qq.com/cgi-bin/oauth2/authorize"

/*
#define STRING_URL_CALLBACK     "https://api.weibo.com/oauth2/default.html"
#define STRING_APP_ID           "801539827" //"801493842"
#define STRING_APP_KEY          "c297ca1566d82a3eeeefdf5a9c49d240" //"91ab5a4777a1af268eacc18f9bf955ed"*/


#define STRING_URL_CALLBACK     "http://dev.t.qq.com/apps/add/1"
#define STRING_APP_ID           "801540870" //"801493842"
#define STRING_APP_KEY          "fd2551b01bd4e793ec408a7e735c96ac"

//#define STRING_URL_FORMAT_CODE  STRING_URL_CALLBACK "?code="

#define STRING_URL_ADD_PIC      _T("http://open.t.qq.com/api/t/add_pic")

COPShareTengxun::COPShareTengxun(void)
{
    m_opAppInfo.m_strCallBackUrl        = STRING_URL_CALLBACK;
    m_opAppInfo.m_strAppID              = STRING_APP_ID;
    m_opAppInfo.m_strAppKey             = STRING_APP_KEY;
    m_strUrlAuth            = STRING_URL_AUTH;
    COPShareBase::AutoParam();
    //m_strCallBackUrlSuffix  = STRING_URL_FORMAT_CODE;
}

COPShareTengxun::~COPShareTengxun(void)
{
}

bool COPShareTengxun::QueryToken()
{
    std::string c_strCustomKey		= TXWeibo::Param::strCustomKey;
    std::string c_oauthCallback		= TXWeibo::Param::strCallbackUrl;
    std::string c_strCustomSecrect	= TXWeibo::Param::strCustomSecrect;
    std::string c_strTokenKey		= TXWeibo::Param::strTokenKey;
    std::string c_strCode			= TXWeibo::Param::strCode;
    std::string c_strOpenid			= TXWeibo::Param::strOpenid;
    std::string c_strGrantType		= TXWeibo::Param::strGrantType;

    m_oParam.Clear();
    m_oParam.AddParam(c_strCustomKey.c_str()          , m_opAppInfo.m_strAppID.c_str());
    m_oParam.AddParam(c_strCustomSecrect.c_str()      , m_opAppInfo.m_strAppKey.c_str());
    m_oParam.AddParam(c_oauthCallback.c_str()         , m_opAppInfo.m_strCallBackUrl.c_str());
    m_oParam.AddParam(c_strCode.c_str()               , m_strCode.c_str());
    m_oParam.AddParam(c_strOpenid.c_str()             , m_strOpenid.c_str());
    m_oParam.AddParam(c_strGrantType.c_str()          , "authorization_code");

    tstring strUrl = _T("https://open.t.qq.com/cgi-bin/oauth2/access_token");
    string strResult;

    if( ! this->SyncQuest(strUrl, EHttpMethod_Get, m_strOutput))
    {
        return FALSE;
    }

    return this->ParseToken(m_strOutput, m_strTokenKey);
}

bool COPShareTengxun::SendWeiboPic(const tstring& strText, const tstring& strPath)
{
    if (m_strOpenid.empty() || m_strTokenKey.empty())
    {
        return false;
    }
    m_oParam.Clear();
    this->AddParamGeneral();

    m_oParam.AddParam("format", "json");
    m_oParam.AddParam("content", CStrCoding::UnicodeToAnsi(strText.c_str()).c_str());
    m_oParam.AddParam("clientip", "127.0.0.1");
    m_oParam.AddPicNameParam("pic", CStrCoding::UnicodeToAnsi(strPath.c_str()).c_str());
    
    string strOut;
    if( ! SyncQuest(STRING_URL_ADD_PIC, EHttpMethod_Post, m_strOutput))
    {
        return false;
    }

    OutInfo(_T("sendWeibo Ret:"), m_strOutput.c_str());
    return ParseAddTopic(m_strOutput);
}

bool COPShareTengxun::ParseToken( const tstring& strRet, string& strToken )
{
    CPiURl url(strRet);
    if( ! url.GetParamValue(STRING_OPEN_API_JSON_ERR_CODE).empty())
    {
        return false;
    }

    strToken = w2s(url.GetParamValue(STRING_TAG_TOKEN));
    if( strToken.empty())
    {
        return false;
    }
    return true;
}

bool COPShareTengxun::ParseAuthInfo( const tstring& strWeb )
{
    CPiURl url(strWeb.c_str());
    
    m_strCode = w2s(url.GetParamValue(STRING_TAG_CODE));
    m_strOpenid = w2s(url.GetParamValue(STRING_TAG_OPENID));
    return true;
}

bool COPShareTengxun::ParseAddTopic( tstring strWeb )
{
    int nErrCode = 0;
    CJsonUtil::GetChild(strWeb, _T("errcode"), nErrCode);
    if (nErrCode != 0)
    {
        return false;
    }
    return true;
}

bool COPShareTengxun::AddParamGeneral()
{
    //以下三个参数在每次请求必须带上
    std::string c_strTokenKey	= TXWeibo::Param::strTokenKey;
    std::string c_strOpenid		= TXWeibo::Param::strOpenid;

    m_oParam.AddParam(c_strTokenKey.c_str(), m_strTokenKey.c_str());
    m_oParam.AddParam(c_strOpenid.c_str(),m_strOpenid.c_str());
    m_oParam.AddParam("oauth_consumer_key", m_opAppInfo.m_strAppID.c_str());
    m_oParam.AddParam("oauth_version", "2.a");
    m_oParam.AddParam("scope", "all");

    //"&oauth_consumer_key=%s&access_token=%s&openid=%s&oauth_version=2.a&scope=all"
    return true;
}