#pragma once
#include "opsharepa.h"
class COPShareMsg :
    public COPShareBase
{
public:
    COPShareMsg(void);
    ~COPShareMsg(void);
public:
    virtual bool SendWeiboPic(const tstring& strText, const tstring& strPath) = 0;
    template<class CBObj, class CBFunc, class CBParam>
    bool SendPicASync( const tstring& strText, const tstring& strPath, CBObj obj, CBFunc func, CBParam cbPar);

    virtual bool SendPic(const tstring& strText, const tstring& strPath);
    virtual bool SendPic2(const tagSEND_WEIBO& tagWB);

};


template<class CBObj, class CBFunc, class CBParam>
bool COPShareMsg::SendPicASync( const tstring& strText, const tstring& strPath, CBObj obj, CBFunc func, CBParam cbPar)
{
    tagSEND_WEIBO wb;
    wb.strText = strText;
    wb.strFile = strPath;
    CallASync(this, &COPShareMsg::SendPic2, wb, obj, func, cbPar);
    return true;
}
