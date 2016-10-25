#pragma once
#include "PiTypeDef.h"

struct tagAlbumQzone
{
    tstring strAlbumID;
    tstring strClassID;         //相册分类ID
    tstring strCreateTime;
    tstring strDesc;
    tstring strName;
    tstring strCoverUrl;        //相册封面照片地址
    tstring strPicNum;      //照片数
};

struct tagCreateAlbumQzone
{
    tstring strLocation;
    tstring strDesc;
    tstring strName;

};

typedef list<tagAlbumQzone*> LST_ALBUM_QZONE;