#include "StdAfx.h"
#include "OPShareSina.h"
#include <WeiboParam.h>
#include "StrCoding.h"
#include "PiURl.h"
#include "JsonUtil.h"
#include "functional.h"

#define STRING_URL_AUTH         "https://api.weibo.com/oauth2/authorize"

//#define STRING_URL_CALLBACK     /*"http://pa.udongman.cn/index.php?c%3Dmember%26m%3Dsina_callback" //*/"http://www.baidu.com/"
//#define STRING_APP_ID           /*测试*/"2293241697" //"3200326645" 
//#define STRING_APP_KEY          /*测试*/"17a5617eb50db4d2cfb3934988c71bee" // "7c8b8006f3a9870a630ff65128920eb7" 

#define STRING_APP_ID           "3200326645" 
#define STRING_APP_KEY          "7c8b8006f3a9870a630ff65128920eb7" 
#define STRING_URL_CALLBACK     "http://pa.udongman.cn/index.php/recall"


#define STRING_URL_TEXT_PIC   _T("https://upload.api.weibo.com/2/statuses/upload.json")
//#define STRING_URL_FORMAT_CODE  STRING_URL_CALLBACK "?code="

COPShareSina::COPShareSina(void)
{
    m_opAppInfo.m_strCallBackUrl    = STRING_URL_CALLBACK;
    m_opAppInfo.m_strAppID          = STRING_APP_ID;
    m_opAppInfo.m_strAppKey         = STRING_APP_KEY;
    m_strUrlAuth        = STRING_URL_AUTH;
    COPShareBase::AutoParam();
    m_pl_type = st_sina;
    //m_strCallBackUrlSuffix = STRING_URL_FORMAT_CODE;
}


COPShareSina::~COPShareSina(void)
{
}

bool COPShareSina::QueryToken()
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
    m_oParam.AddParam(c_strGrantType.c_str()          , "authorization_code");

    tstring strUrl = _T("https://api.weibo.com/oauth2/access_token");
    
    if(! SyncQuest(strUrl.c_str(), EHttpMethod_Post, m_strOutput))
    {
        return false;
    }

    if(COPShareSina::ParseToken(m_strOutput, m_strTokenKey))
    {
        return true;
    }
    OutInfo(_T("get token err:"), m_strOutput.c_str());
    return FALSE;
}


bool COPShareSina::SendWeiboPic(const tstring& strText, const tstring& strPath)
{
    if (m_strTokenKey.empty())
    {
        return false;
    }
    //CWeiboParam oParam;
    //以下三个参数在每次请求必须带上
    std::string c_strCustomKey	= TXWeibo::Param::strCustomKey;
    std::string c_strTokenKey	= TXWeibo::Param::strTokenKey;
    std::string c_strOpenid		= TXWeibo::Param::strOpenid;

    m_oParam.Clear();
    m_oParam.AddParam(c_strTokenKey.c_str(), m_strTokenKey.c_str());
    m_oParam.AddParam("status", CStrCoding::UnicodeToAnsi(strText.c_str()).c_str());
    m_oParam.AddPicNameParam("pic", CStrCoding::UnicodeToAnsi(strPath.c_str()).c_str());

    if(SyncQuest(STRING_URL_TEXT_PIC, EHttpMethod_Post, m_strOutput))
    {
    }
    else
    {
    }
    OutInfo(m_strOutput.c_str());
    return ParseAddTopic(m_strOutput);
}

bool COPShareSina::ParseToken( const tstring& strRet, string& strToken )
{
    tstring strError;
    //有error字段, 并且有描述, 则有错误发生
    if(! CJsonUtil::GetChild(strRet, _T("error"), strError)
        || !strError.empty())
    {
        //有错误
        return false;
    }
    //有access_token字段, 该字段为值
    tstring strTokenTemp;
    if (CJsonUtil::GetChild(strRet, STRING_TAG_TOKEN, strTokenTemp)
        && strTokenTemp.empty())
    {
        return false;
    }
    strToken = w2s(strTokenTemp);

    return true;
}

bool COPShareSina::ParseAuthInfo( const tstring& strWeb )
{
    CPiURl url(strWeb.c_str());
    m_strCode = w2s(url.GetParamValue(STRING_TAG_CODE));
    return true;
}

bool COPShareSina::ParseAddTopic( const tstring& strWeb )
{
    tstring strError;
    if(! CJsonUtil::GetChild(strWeb, _T("error"), strError)
        || !strError.empty())
    {
        //有错误
        return false;
    }
    return true;
}
