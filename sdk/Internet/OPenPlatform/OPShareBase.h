#pragma once
#include <WeiboApi.h>
#include "PiTypeDef.h"
#include "WeiboParam.h"
#include "OpenPlatformDef.h"
#include "CallAsync.h"
#include "PAOPDef.h"
class COPShareBase : public VWeiboCallback
{
public:
    COPShareBase();
    virtual ~COPShareBase(void);

public:
    virtual bool ParseAuthInfo( const tstring& strWeb ) = 0;
    virtual bool QueryToken() = 0;

    virtual bool QuestAppAuthInfo() { return true; }
    virtual bool QuestUserInfo()  { return true; }

    virtual string GetRequestCodeUrl();

    bool SetAppInfo(const tagOP_APP_INFO& ai);
    bool SetAuthInfo(const tstring& strCode, const tstring& strOpenID);

    virtual void OnRequestComplete(HTTP_RESULTCODE eHttpRetCode, const char* pReqUrl, CWeiboParam oParam, 
        const char*pData,int nLen);
    bool IsCodeUrl( tcpchar szUrl );
    tstring GetWebOutData();
    bool SyncQuest(const tstring& szUrl, EHttpMethod method, tstring& strWeb);
    bool AutoParam();
protected:
    string m_strUrlAuth;
    tagOP_APP_INFO      m_opAppInfo;
    

    string m_strCallBackUrlSuffix;
    string m_strRespType;
    
    string m_strState;  //状态值, 原样返回
    
    // AccessKey
    string m_strTokenKey;
    string m_strCode;
    string m_strOpenid;

    tstring m_strOutput;

    CWeiboParam m_oParam;
    ENUM_OP_SHARE_TYPE          m_pl_type;

};

