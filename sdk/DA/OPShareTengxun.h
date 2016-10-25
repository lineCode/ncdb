#pragma once
#include <WeiboApi.h>
#include "PiTypeDef.h"
#include "OPShareBase.h"
#include "OPShareMsg.h"

class COPShareTengxun :public COPShareMsg
{
public:
    COPShareTengxun(void);
    ~COPShareTengxun(void);
public:
    virtual bool QueryToken();  

    virtual bool SendWeiboPic(const tstring& strText, const tstring& strPath);
    virtual bool ParseToken(const tstring& strRet, string& strToken);
    bool ParseAuthInfo( const tstring& strWeb );
    virtual bool ParseAddTopic( tstring strWeb );
    bool AddParamGeneral();

    //string GetRequestCodeUrl();
protected:
    //string m_strOpenid;
};

