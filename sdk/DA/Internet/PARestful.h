#pragma once
#include "pinetrestful.h"
#include "PARestfulDef.h"
#include "ResCommon.h"

class CMarkup;
class CPARestful:
    public CPiNetRestful
{
public:
    CPARestful(void);
    virtual ~CPARestful(void);

//inherit
public:
    /************************************************************************
        fun:    对数据进行UTF8转UNICODE
        remark:
        ret:   
    ************************************************************************/
    virtual bool GetDistWebData( tstring& strData );

    /************************************************************************
        fun:    对所有参数值进行 base64加密
        remark:
        ret:   
    ************************************************************************/
    virtual bool DoParam();
protected:
    auto_ptr<CMarkup>            m_pXML;
    
};


