#pragma once
#include "PiTypeDef.h"

#define  STRING_PARAM_TOKEN         "access_token"

#define  STRING_URL_USER_GET            _T("https://api.renren.com/v2/user/get"     )
#define  STRING_URL_ALBUM_LIST          _T("https://api.renren.com/v2/album/list"   )
#define  STRING_URL_PHOTO_UPLOAD        _T("https://api.renren.com/v2/photo/upload" )


#define  STRING_JSON_RESP           _T("response")
struct tagRRAlbum
{
    tstring strAlbumID;
    tstring strClassID;         //相册分类ID
    tstring strCreateTime;
    tstring strDesc;
    tstring strName;
    tstring strCoverUrl;        //相册封面照片地址
    tstring strPicNum;      //照片数
};


typedef list<tagRRAlbum*> LST_RR_ALBUM;

struct tagRRUser
{
    string strID;
    string strName;
};