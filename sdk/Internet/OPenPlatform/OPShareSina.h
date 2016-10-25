#pragma once
#include "piTypeDef.h"
#include "OPShareMsg.h"
class COPShareSina: public COPShareMsg
{
public:
    COPShareSina(void);
    ~COPShareSina(void);
public:
    virtual bool ParseAuthInfo( const tstring& strWeb );
    virtual bool QueryToken();
    virtual bool ParseAddTopic(const tstring& strWeb );
    virtual bool SendWeiboPic(const tstring& strText, const tstring& strPath);

    virtual bool ParseToken(const tstring& strRet, string& strToken);
private:

};

