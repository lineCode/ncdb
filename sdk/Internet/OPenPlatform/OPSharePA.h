#pragma once
#include "opsharebase.h"

/************************************************************************
    说明:COPSharePA, PA分享类
        获取授权url时, 从服务器获取app应用信息, 
        如果不想从服务器获取, 直接从COPShareBase派生
************************************************************************/
class COPSharePA :
    public COPShareBase
{
public:
    COPSharePA(void);
    ~COPSharePA(void);
public:
    virtual string GetRequestCodeUrl();
    bool ParseAppInfo( const tstring& strWeb, tagOP_APP_INFO& ai);
protected:
    tagOP_APP_INFO QueryAppInfo();
};