#pragma once
#include "PARestful.h"
#include "OpenPlatformDef.h"
#include "PAOPDef.h"
class COPLogin :public CPARestful
{
public:
    
    COPLogin(ENUM_OP_LOGIN_TYPE loginType);
    ~COPLogin(void);

//inherit
public:
    virtual tstring GetUrl();
    virtual bool DoParam();

protected:
    virtual MAP_STRING GetParam();
//self
public:
    /************************************************************************
        fun:    向服务器请求授权地址, 返回解析后的url
        remark:
        ret:   
    ************************************************************************/
    
    static tstring QueryUrlAuth( ENUM_OP_LOGIN_TYPE lType );
protected:
    /************************************************************************
        fun:    获取平台返回的登录授权url
        remark:
        ret:   
    ************************************************************************/
    tstring ExtractRetUrl();
protected:
    ENUM_OP_LOGIN_TYPE          m_login_type;
};

