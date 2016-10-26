#include "StdAfx.h"
#include "OPShareQzone.h"
#include "OpenPlatformDef.h"
#include <WeiboParam.h>
#include "PiURl.h"
#include "StrCoding.h"
#include "Markup.h"
#include "XmlUtil.h"
#include "OPQzoneDef.h"
#include "CallAsync.h"
#include "JsonUtil.h"
#include "functional.h"


#define STRING_URL_AUTH         "https://graph.qq.com/oauth2.0/authorize"

/*
#define STRING_URL_CALLBACK     "https://api.weibo.com/oauth2/default.html"
#define STRING_APP_ID           "8801493840"
#define STRING_APP_KEY          "91ab5a4777a1af268eacc18f9bf955ed"*/

///*
/*
#define STRING_URL_CALLBACK     "http://www.udongman.cn/" //"http://suolong.sinaapp.com/index.htm"
#define STRING_APP_ID           "100573370" //"101051421" //
#define STRING_APP_KEY          "9223037602383d5b5f2f5c0987e431e2"  //"c1c7a09d1fbc5329fe2b4f429502f659" //*/

#define STRING_URL_CALLBACK     "http://dev.t.qq.com/apps/add/1"
#define STRING_APP_ID           "801540870" //"101051421" //
#define STRING_APP_KEY          "fd2551b01bd4e793ec408a7e735c96ac"  //"c1c7a09d1fbc5329fe2b4f429502f659" //

//*/
//#define STRING_URL_FORMAT_CODE  STRING_URL_CALLBACK "?code="

#define STRING_URL_TEXT_PIC     _T("https://upload.api.weibo.com/2/statuses/upload.json")
#define STRING_URL_LIST_ALBUM   _T("https://graph.qq.com/photo/list_album")
#define STRING_URL_USER_INFO    _T("https://graph.qq.com/user/get_user_info")
#define STRING_URL_OPENID       _T("https://graph.qq.com/oauth2.0/me")
#define STRING_URL_UPLOAD_PIC   _T("https://graph.qq.com/photo/upload_pic")



COPShareQzone::COPShareQzone(void)
{
    m_strState = "test";
    m_opAppInfo.m_strCallBackUrl    = STRING_URL_CALLBACK;
    m_opAppInfo.m_strAppID          = STRING_APP_ID;
    m_opAppInfo.m_strAppKey         = STRING_APP_KEY;
    m_strUrlAuth                    = STRING_URL_AUTH;
    COPShareBase::AutoParam();
    //m_strCallBackUrlSuffix = STRING_URL_FORMAT_CODE;
    Init();
}


COPShareQzone::~COPShareQzone(void)
{
}

bool COPShareQzone::QueryToken()
{

    std::string c_strCustomKey		= TXWeibo::Param::strCustomKey;
    std::string c_oauthCallback		= TXWeibo::Param::strCallbackUrl;
    std::string c_strCustomSecrect	= TXWeibo::Param::strCustomSecrect;
    std::string c_strCode			= TXWeibo::Param::strCode;
    std::string c_strGrantType		= TXWeibo::Param::strGrantType;

    m_oParam.Clear();
    m_oParam.AddParam(c_strGrantType.c_str()          , "authorization_code");
    m_oParam.AddParam(c_strCustomKey.c_str()          , m_opAppInfo.m_strAppID.c_str());
    m_oParam.AddParam(c_strCustomSecrect.c_str()      , m_opAppInfo.m_strAppKey.c_str());
    m_oParam.AddParam(c_oauthCallback.c_str()         , m_opAppInfo.m_strCallBackUrl.c_str());
    m_oParam.AddParam(c_strCode.c_str()               , m_strCode.c_str());
    //oParam.AddParam(c_strOpenid.c_str()             , m_strOpenid.c_str());
    size_t nSizeReturn = 0;

    tstring strUrl = _T("https://graph.qq.com/oauth2.0/token");

    SyncQuest(strUrl, EHttpMethod_Get, m_strOutput);

    return this->ParseToken(m_strOutput, m_strTokenKey);
}

bool COPShareQzone::ParseToken( const tstring& strRet, string& strToken )
{
    tstring strValue;
    //CJsonUtil::GetChild(strRet, , strValue);
    if ( IsValidStrPos(strRet.find(_T("error")) ))
    {
        //获取token失败
        return false;
    }

    CPiURl url(strRet);
    
    strToken = w2s(url.GetParamValue(STRING_TAG_TOKEN));

    return true;
}

bool COPShareQzone::QuestUserInfo()
{
    //无需获取用户信息
    return true;

    /*m_oParam.Clear();
    this->AddParamGeneral();

    if( ! this->SyncQuest(STRING_URL_USER_INFO, EHttpMethod_Get, m_strOutput))
    {
        return false;
    }
    return ParseUserInfo(m_strOutput);*/
}

bool COPShareQzone::QuestAlbumList()
{
    m_oParam.Clear();
    this->AddParamGeneral();

    std::string c_strFormat	= TXWeibo::Param::strFormat;
    m_oParam.AddParam(c_strFormat.c_str(), "json");

    if( ! this->SyncQuest(STRING_URL_LIST_ALBUM, EHttpMethod_Get, m_strOutput))
    {
        return false;
    }
    
    return this->ParseAlbum(m_strOutput);
}

bool COPShareQzone::QuestAppAuthInfo()
{
    //test
    /*m_strTokenKey = "98C0BBBBA3638EE1EE7B52DBA8F65399";
    m_opAppInfo.m_strAppID = "200019";
    m_strOpenid = "34E24643F9760D25A1EAA856265EC87C";
    return true;*/

    if( QueryToken()
        && GetOpenId()
        )
    {
        return true;
    }
    return false;
}

bool COPShareQzone::GetOpenId()
{
    std::string c_strTokenKey		= TXWeibo::Param::strTokenKey;
    m_oParam.Clear();
    
    m_oParam.AddParam(c_strTokenKey.c_str(), m_strTokenKey.c_str());
    if( ! this->SyncQuest(STRING_URL_OPENID, EHttpMethod_Get, m_strOutput))
    {
        return false;
    }

    return this->ParseOpenID(m_strOutput, m_strOpenid);
}

bool COPShareQzone::ParseOpenID( const tstring& strWeb, string& m_strOpenid )
{
    //第一个openid后的双引号里的内容
    const tstring szKey = _T("openid");
    size_t nSize = strWeb.find(szKey);
    if ( ! IsValidStrPos(nSize))
    {
        return false;
    }
    size_t nBegin = strWeb.find(_T("\""), nSize + szKey.length() + 2);//忽略下一个比邻的双引号
    size_t nEnd = strWeb.find(_T("\""), nBegin + 1);

    m_strOpenid = w2s(strWeb.substr(nBegin + 1, nEnd - nBegin - 1));
    return true;
}

bool COPShareQzone::ParseAlbum( const tstring& strWeb )
{
    m_pAlbumLst->Clear();

    Json::Value lstAlbum;
    CJsonUtil::GetChild(strWeb, _T("album"), lstAlbum);
    FOR_EACH_ITER(Value, lstAlbum, it)
    {
        Value elem = *it;
        _ParseOneAlbum(elem, m_pAlbumLst.get());
    }
    return true;

    /*CMarkup xml;
    xml.SetDoc(strWeb);

    xml.FindElem(_T("data"));
    xml.IntoElem();
    xml.FindElem(_T("album"));
    xml.IntoElem();

    tagAlbumQzone* pAlbum = new tagAlbumQzone;
    
    CXmlUtil xmlUt(xml);
    xmlUt.GetChildData(_T("albumid"), pAlbum->strAlbumID);
    xmlUt.GetChildData(_T("classid"), pAlbum->strClassID);
    m_pAlbumLst->Add(pAlbum);*/
    return true;
}

bool COPShareQzone::Init()
{
    Create_Auto_Ptr(m_pAlbumLst, CAlbumList);
    return true;
}

void COPShareQzone::Test()
{
    this->QuestUserInfo();
}

bool COPShareQzone::ParseAuthInfo( const tstring& strWeb )
{
    CPiURl url(strWeb.c_str());
    m_strCode = w2s(url.GetParamValue(STRING_TAG_CODE));
    //OutInfo(_T("code:"), strCode.c_str());

    return true;
}

bool COPShareQzone::ParseUserInfo( const tstring& strOutput )
{
    int nErr = 0;
    CJsonUtil::GetChild(strOutput, _T("ret"), nErr);
    if (nErr)
    {
        return false;
    }
    return true;
}

bool COPShareQzone::_ParseOneAlbum( Json::Value elem, CAlbumList* pAbLst)
{
    tagAlbumQzone* pAlbum = new tagAlbumQzone;
    pAlbum->strAlbumID  = s2w(elem["albumid"].asString());
    pAlbum->strName     = s2w(elem["name"].asString());

    pAbLst->Add(pAlbum);
    return true;
}


bool COPShareQzone::AddParamGeneral()
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

bool COPShareQzone::ParseUploadPic( const tstring& m_strOutput )
{
    int nErr = 0;
    CJsonUtil::GetChild(m_strOutput, _T("ret"), nErr);
    if (nErr)
    {
        return false;
    }
    return true;
}

/*
bool COPShareQzone::GetAlbumList( void*& pAlbum )
{
    pAlbum = &m_pAlbumLst;
    return true;
}*/

bool COPShareQzone::UploadPic( const tstring& strDesc, const tstring& strPath, const tstring& strAlbumID )
{
    m_oParam.Clear();
    this->AddParamGeneral();
    m_oParam.AddParam("photodesc", w2s(strDesc).c_str());
    m_oParam.AddParam("format", "json");
    m_oParam.AddParam("albumid", w2s(strAlbumID).c_str());
    m_oParam.AddPicNameParam("picture", w2s(strPath).c_str());
    if(! this->SyncQuest( STRING_URL_UPLOAD_PIC, EHttpMethod_Post, m_strOutput))
    {
        return false;
    }
    OutInfo(_T("SendWb ret:"), m_strOutput.c_str());
    return this->ParseUploadPic(m_strOutput);
}

bool COPShareQzone::CreateAlbum( const tagCreateAlbumRR& car )
{
    m_oParam.Clear();
    this->AddParamGeneral();
    m_oParam.AddParam("albumname", w2s(car.strName).c_str());
    m_oParam.AddParam("albumdesc", w2s(car.strDesc).c_str());
    m_oParam.AddParam("format", "json");

    if(! this->SyncQuest( _T("https://graph.qq.com/photo/add_album"), EHttpMethod_Post, m_strOutput))
    {
        return false;
    }
    ParseAddAlbum(m_strOutput);
    return true;
}

bool COPShareQzone::ParseAddAlbum( tstring strOutput )
{
    CAlbumList abLst;

    Json::Value root;
    if(!CJsonUtil::GetChild(strOutput, root))
    {
        return false;
    }
    _ParseOneAlbum(root, &abLst);
    if(!abLst.Count())
    {
        return false;
    }
    tagAlbumQzone* pAlbum = new tagAlbumQzone(*abLst.First());
    m_pAlbumLst->Add(pAlbum);
    return true;
}
