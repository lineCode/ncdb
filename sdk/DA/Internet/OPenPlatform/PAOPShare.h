#pragma once
#include "parestful.h"
#include "PAOPDef.h"
#include "OpenPlatformDef.h"
class CPAOPShare :
    public CPARestful
{
public:
    CPAOPShare(ENUM_OP_SHARE_TYPE type);
    ~CPAOPShare(void);
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
    
    static tagOP_APP_INFO QueryAppInfo( ENUM_OP_SHARE_TYPE lType );
protected:
    /************************************************************************
        fun:    获取平台返回的登录授权url
        remark:
        ret:   
    ************************************************************************/
    bool ParseOPAppInfo();
protected:
    ENUM_OP_SHARE_TYPE          m_pl_type;
    tagOP_APP_INFO              m_OpAppInfo;
};

