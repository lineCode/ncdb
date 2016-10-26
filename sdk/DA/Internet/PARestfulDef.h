#pragma once
#include "piTypeDef.h"
struct tagDATA_NEWS
{
    tstring strTime;
    tstring strType;    //新闻类别
    tstring strTitle;
    tstring strContent;
    tstring strUrl;    
};

typedef list<tagDATA_NEWS*>      LST_NEWS;