#include "StdAfx.h"
#include "OPShareBase.h"
#include "StdAfx.h"
#include "OPShareTengxun.h"
#include "WeiboParam.h"
#include "StrCoding.h"
#include "functional.h"
#include "define_gnl.h"
#include "PiNetRestful.h"
#include "OPShareSina.h"
#include "CallAsync.h"
#include <assert.h>
#include "PiURl.h"

COPShareBase::COPShareBase( )
{
    m_strRespType = "code";
}


COPShareBase::~COPShareBase(void)
{
}

bool COPShareBase::QueryToken()
{
    assert(0);
    /*CWeiboParam oParam;

    std::string c_strCustomKey		= TXWeibo::Param::strCustomKey;
    std::string c_oauthCallback		= TXWeibo::Param::strCallbackUrl;
    std::string c_strCustomSecrect	= TXWeibo::Param::strCustomSecrect;
    std::string c_strTokenKey		= TXWeibo::Param::strTokenKey;
    std::string c_strCode			= TXWeibo::Param::strCode;
    std::string c_strOpenid			= TXWeibo::Param::strOpenid;
    std::string c_strGrantType		= TXWeibo::Param::strGrantType;

    oParam.AddParam(c_strCustomKey.c_str()          , m_opAppInfo.m_strAppID.c_str());
    oParam.AddParam(c_strCustomSecrect.c_str()      , m_opAppInfo.m_strAppKey.c_str());
    oParam.AddParam(c_oauthCallback.c_str()         , m_opAppInfo.m_strCallBackUrl.c_str());
    oParam.AddParam(c_strCode.c_str()               , m_strCode.c_str());
    //oParam.AddParam(c_strOpenid.c_str()             , m_strOpenid.c_str());
    oParam.AddParam(c_strGrantType.c_str()          , "authorization_code");

    / *MAP_STRINGA mapParam;
    mapParam[c_strCustomKey.c_str()] = m_opAppInfo.m_strAppID;
    mapParam[c_strCustomSecrect.c_str()] = m_opAppInfo.m_strAppKey;
    mapParam[c_oauthCallback.c_str()] = m_strCallBackUrl;
    mapParam[c_strCode.c_str()] = m_strCode;
    mapParam[c_strGrantType.c_str()] = "authorization_code";* /

    size_t nSizeReturn = 0;

    string strUrl = "https://api.weibo.com/oauth2/access_token";

    char* pRetData = NULL;
    int nLen = 0;

    / *
    string strParam;
    //CRequestUrl ru;
    tstring strWeb;
    strParam = CPiNetRestful::MakeParam(mapParam);
    CWeiboParam::MapKey2Value mapKey = oParam.GetParamMap();
    FOR_EACH_ITER(CWeiboParam::MapKey2Value, mapKey, it)
    {
        strParam += it->first + ("=") + it->second + ("&");
    }
    strParam.resize(strParam.length() - 1);

    / *if(! CPiNetRestful::SendSync(CStrCoding::AnsiToUnicode(strUrl.c_str()), CStrCoding::AnsiToUnicode(strParam.c_str()), Restful_Post, strWeb))
    {
        return false;
    }* /
    HTTP_RESULTCODE eHttpCode = SyncRequest(strUrl.c_str(), EHttpMethod_Post, oParam, pRetData,nLen);

    if(eHttpCode != HTTPRESULT_OK || pRetData == NULL)
    {
        return FALSE;
    }

    string strResult = pRetData;
    strResult.resize(nLen);

    ReleaseData(pRetData);

    string strMark1 = "access_token=";
    string strMark2 = "&expires_in=";
    string strMark3 = "&refresh_token=";

    if(this->ParseToken(strResult, m_strTokenKey))
    {
        return true;
    }*/

    return FALSE;
}

string COPShareBase::GetRequestCodeUrl()
{
    //从服务器获取
    CWeiboParam oParam;

    std::string c_strCustomKey		= TXWeibo::Param::strCustomKey;
    std::string c_oauthCallback		= TXWeibo::Param::strCallbackUrl;
    std::string c_resonseType		= TXWeibo::Param::strRespType;

    string strUrl = m_strUrlAuth;
    strUrl += "?redirect_uri=";
    strUrl += m_opAppInfo.m_strCallBackUrl;

    strUrl += "&response_type=";
    strUrl += m_strRespType;
    strUrl += "&client_id=";
    strUrl += m_opAppInfo.m_strAppID;
    if (m_strState.size())
    {
        strUrl += "state=";
        strUrl += m_strState;
    }
    return strUrl;
}

void COPShareBase::OnRequestComplete( HTTP_RESULTCODE eHttpRetCode,const char* pReqUrl, CWeiboParam oParam, const char*pData,int nLen )
{

    if(eHttpRetCode != HTTPRESULT_OK && eHttpRetCode <= HTTPRESULT_FAIL)
    {
        return;
    }

    if (eHttpRetCode == HTTPRESULT_OK)
    {
        wchar_t* pUnicodeData = Util::String::Utf8ToUnicode(pData);
        m_strOutput = pUnicodeData;
        OutInfo(_T("return data:"), m_strOutput.c_str());
        ReleaseData(pUnicodeData);
    }
    else
    {
        assert(0);
    }

}

bool COPShareBase::SetAuthInfo( const tstring& strCode, const tstring& strOpenID )
{
    m_strCode       = CStrCoding::UnicodeToAnsi(strCode.c_str());
    m_strOpenid     = CStrCoding::UnicodeToAnsi(strOpenID.c_str());
    return true;
}

bool COPShareBase::IsCodeUrl( tcpchar szUrl )
{
    string strUrl(CStrCoding::UnicodeToAnsi(szUrl));
    
    if( IsValidStrPos(strUrl.find(m_strCallBackUrlSuffix)))
    {
        return true;
    }
    return false;
}


bool COPShareBase::SyncQuest( const tstring& strUrl, EHttpMethod method, tstring& strWeb )
{
    char* pRetData = NULL;
    int nLen = 0;

    HTTP_RESULTCODE eHttpCode = SyncRequest(w2s(strUrl).c_str(), method, m_oParam, pRetData, nLen);

    if(eHttpCode != HTTPRESULT_OK || pRetData == NULL)
    {
        return false;
    }
    string strTemp(pRetData);
    strTemp.resize(nLen);
    strWeb = CStrCoding::Utf8ToUnicode(strTemp.c_str());
    /*strWeb = s2w(strTemp);
    string strCheck = w2s(strWeb);*/
    //strWeb.resize(nLen);

    ReleaseData(pRetData);
    return true;
}

bool COPShareBase::AutoParam()
{
    CPiURl url(s2w(CStrCoding::UrlGB2312Decode(m_opAppInfo.m_strCallBackUrl)));
    url.AppendParam(_T("code"), _T(""));
    m_strCallBackUrlSuffix = w2s(url.GetUrl())/* + "?code="*/;
    return true;
}

bool COPShareBase::ParseAuthInfo( const tstring& strWeb )
{
    return true;
}

tstring COPShareBase::GetWebOutData()
{
    return m_strOutput;
}

/*

bool COPShareBase::SendPicASync( const tstring& strText, const tstring& strPath )
{
    tagSEND_WEIBO wb;
    wb.strText = strText;
    wb.strFile = strPath;
    CallASync(this, &COPShareBase::SendPic, wb, );
}*/

