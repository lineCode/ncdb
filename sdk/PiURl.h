#pragma once
#include "piTypeDef.h"
class CPiURl
{
public:
    CPiURl(tcpchar szUrl);
    CPiURl(const tstring& szUrl);
    ~CPiURl(void);
public:
    tstring GetParamValue(int nCount);
    /************************************************************************
        fun:    获取指定参数名的参数值, 支持url中有& 和#, 以及直接参数字符串的情况
        remark:
        ret:   
    ************************************************************************/
    tstring GetParamValue(tcpchar szParam);
    bool AppendParam( tcpchar szParam, tcpchar szValue );
    tstring GetUrl(){ return m_strUrl; }
private:
    tstring             m_strUrl;
};

