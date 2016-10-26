#include "StdAfx.h"
#include "OPRenRen.h"
#include "StrCoding.h"
#include "PiURl.h"
#include "functional.h"
#include "JsonHeader.h"
#include "AutoType.h"
#include "PiRandom.h"
#include "OpenPlatformDef.h"
#include "JsonUtil.h"
#define STRING_URL_AUTH         "https://graph.renren.com/oauth/authorize"
#define STRING_URL_CALLBACK     "http://graph.renren.com/oauth/login_success.html"
// #define STRING_APP_ID           "f3dd3c281c264d49be250616ff73938a" 
// #define STRING_APP_KEY          "332a8a5d627f4611a7552718fb358f99" 

#define STRING_APP_ID           "890e5936f8eb489db496e7e47da90fe8"
#define STRING_APP_KEY          "8c0d076bce68427b801804350c81bcc3"

#define STRING_URL_FORMAT_CODE  STRING_URL_CALLBACK "#access_token="

COPRenRen::COPRenRen(void)
{
    m_opAppInfo.m_strCallBackUrl        = STRING_URL_CALLBACK;
    m_opAppInfo.m_strAppID              = STRING_APP_ID;
    m_opAppInfo.m_strAppKey             = STRING_APP_KEY;
    m_strUrlAuth            = STRING_URL_AUTH;
    m_strCallBackUrlSuffix  = STRING_URL_FORMAT_CODE;
    //COPShareBase::AutoParam();
    m_strRespType           = "token";
}


COPRenRen::~COPRenRen(void)
{
}

bool COPRenRen::QuestUserInfo()
{
    m_oParam.Clear();
    AddParamGeneral();
    if (!this->SyncQuest(STRING_URL_USER_GET, EHttpMethod_Get, m_strOutput))
    {
        return false;
    }
    return ParseUserInfo(m_strOutput);
}

bool COPRenRen::QuestAlbumList()
{
    m_oParam.Clear();
    AddParamGeneral();

    m_oParam.AddParam("ownerId", m_rrUser.strID.c_str());

    /*string strWeb;
    if( !AsyncRequestNoParam(STRING_URL_ALBUM_LIST, EHttpMethod_Get, m_oParam, this))
    {
        return false;
    }*/

    if (!this->SyncQuest(STRING_URL_ALBUM_LIST, EHttpMethod_Get, m_strOutput))
    {
        return false;
    }

    return ParseAlbumList(m_strOutput);

}

bool COPRenRen::ParseAlbumList( const tstring& strWeb )
{
    m_pAlbumLst->Clear();

    

    return ParseAlbum(strWeb, m_pAlbumLst.get());

    
    /*CAutoType at(body["id"].asUInt());
    m_rrUser.strID = at;*/

    return true;
}

bool COPRenRen::ParseUserInfo( const tstring& strWeb )
{
    //OutInfo(_T("WStr:"), strWeb.c_str());
    //string strA = w2s(strWeb);
    //OutputDebugStringA("Str:", strA.c_str());
    Json::Value body;
    if(!IsQuestOk(strWeb, &body))
    {
        return false;
    }


    /*const string strWebA = w2s(strWeb);
    Json::Reader reader;
    
    if (!reader.parse(strWebA.c_str(), root, false))
    {
        return false;
    }*/
    //ÅÐ¶ÏÊÇ·ñÓÐ´íÎó
    //Value body = root["response"];
    CAutoType at(body["id"].asUInt());
    m_rrUser.strID = at;
    
    return true;
}


bool COPRenRen::ParseAuthInfo( const tstring& strUrl )
{
    //½âÎötoken 
    CPiURl url(strUrl.c_str());
    tstring strCode = url.GetParamValue(STRING_TAG_TOKEN);
    m_strTokenKey = CStrCoding::UnicodeToAnsi(CStrCoding::UrlDecode(strCode).c_str());
    if (m_strTokenKey.empty())
    {
        return false;
    }

    return true;
}

bool COPRenRen::AddParamGeneral()
{
    m_oParam.AddParam(STRING_PARAM_TOKEN, m_strTokenKey.c_str());
    return true;
}

string COPRenRen::GetRequestCodeUrl()
{
    string strUrl = COPShareBase::GetRequestCodeUrl();
    strUrl += "&scope=read_user_album+read_user_feed+photo_upload+create_album";
    
    return strUrl;
}

bool COPRenRen::_ParseOneAlbum( const Json::Value& elem, CRRAlbumList* plbumLst )
{
    CAutoType at;
    tagRRAlbum* pAlbum = new tagRRAlbum;
    pAlbum->strName = s2w(elem["name"].asString());
    
    pAlbum->strAlbumID = (tstring)at.SetData( elem["id"].asUInt() );
    plbumLst->Add(pAlbum);
    return true;
}

bool COPRenRen::QueryToken()
{
    return true;
}

bool COPRenRen::ParseAddTopic( const tstring& m_strOutput )
{
    return IsQuestOk(m_strOutput);
}

bool COPRenRen::IsQuestOk( const tstring& strWeb, Json::Value* pBody /*= NULL*/)
{
    Json::Value body;
    if( ! CJsonUtil::GetChild(strWeb, _T("response"), body))
    {
        return false;
    }
    if (pBody)
    {
        *pBody = body;
    }
    return true;
}

bool COPRenRen::UploadPic( const tstring& strDesc, const tstring& strPath, const tstring& strAlbumID )
{

    m_oParam.Clear();
    AddParamGeneral();
    
    if ( ! strAlbumID.empty())
    {
        m_oParam.AddParam("albumId", w2s(strAlbumID).c_str());
    }
    m_oParam.AddParam("description", CStrCoding::UnicodeToAnsi(strDesc.c_str()).c_str());


    string strFile = CStrCoding::UnicodeToAnsi(strPath.c_str());

    m_oParam.AddPicNameParam("file", strFile.c_str());
    if (!this->SyncQuest(STRING_URL_PHOTO_UPLOAD, EHttpMethod_Post, m_strOutput))
    {
        return false;
    }
    OutInfo(_T("pic resp:"), m_strOutput.c_str());
    return ParseAddTopic(m_strOutput);
}

bool COPRenRen::CreateAlbum( const tagCreateAlbumRR& car )
{
    m_oParam.Clear();
    AddParamGeneral();
    m_oParam.AddParam("description", w2s(car.strDesc).c_str());
    m_oParam.AddParam("name", w2s(car.strName).c_str());
    if(! this->SyncQuest(_T("https://api.renren.com/v2/album/put"), EHttpMethod_Post, m_strOutput))
    {
        return false;
    }
    OutInfo(m_strOutput.c_str());
    return ParseAddAblum(m_strOutput);
}

bool COPRenRen::ParseAddAblum( const tstring& strOutput)
{
    CRRAlbumList albumLst;

    Json::Value elem;
    if( ! CJsonUtil::GetChild(strOutput, STRING_JSON_RESP, elem))
    {
        return false;
    }
    _ParseOneAlbum(elem, &albumLst);
    if (!albumLst.Count())
    {
        return false;
    }

    tagRRAlbum* pRRAlbum = new tagRRAlbum(*albumLst.First());
    m_pAlbumLst->Add(pRRAlbum);
    return true;
}

bool COPRenRen::ParseAlbum( const tstring& strWeb, CRRAlbumList* pAlbumLst )
{
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(w2s(strWeb).c_str(), root, false))
    {
        return false;
    }

    Value body = root["response"];
    FOR_EACH_ITER(Value, body, it)
    {
        Value elem = *it;
        //_ParseOneAlbum(elem);
        _ParseOneAlbum(elem, pAlbumLst);

    }
    return true;
}

/*

bool COPRenRen::GetAlbumList( void*& pAlbum )
{
    //    CRRAlbumList* pRR = (CRRAlbumList*)pAlbum;
    pAlbum = &m_rrAlbumLst;
    return true;
}
*/
