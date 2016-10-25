#pragma once
#define STRING_TAG_TOKEN        _T("access_token")
#define STRING_TAG_CODE         _T("code")
#define STRING_TAG_OPENID         _T("openid")
#define STRING_TAG_OPENKEY         _T("openkey")

#define STRING_OPEN_API_JSON_ERR_CODE       _T("errorCode")

struct tagOP_APP_INFO
{
    string m_strAppID;      //appkey
    string m_strAppKey;// AppSecret
    string m_strCallBackUrl;

};


enum ENUM_OP_URL
{
    qzone_auth = 0,
    qzone_photo_list,
};



/*
const char* STRING_URL[] = 
{
    ("https://graph.qq.com/oauth2.0/authorize"),
    ("https://graph.qq.com/photo/list_album"),
    ("sdfdf"),
};*/

struct tagSEND_WEIBO
{
    tstring strText;
    tstring strFile;
};


struct tagUPLOAD_PIC
{
    tstring strText;
    tstring strFile;
    tstring strAlbumID;     //相册ID

};

//创建相册结构体
struct tagCreateAlbumRR
{
    tstring strLocation;
    tstring strDesc;
    tstring strName;

};