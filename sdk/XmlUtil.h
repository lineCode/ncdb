#pragma once
#include "piTypeDef.h"
#include <Markup.h>
class CXmlUtil
{
public:
    CXmlUtil();
    CXmlUtil(tcpchar szDoc);
    CXmlUtil(CMarkup& xml);
    ~CXmlUtil(void);
public:
    static bool GetChildElemData(tcpchar szDoc, tcpchar szElem, tstring& strData);
    static bool GetChildElemDataBase64( tcpchar szDoc, tcpchar szElem, tstring& strData);
    
    bool GetChildData(tcpchar szElem, tstring& strData);
    bool Into(tcpchar szElem);
    bool IntoChild(tcpchar szElem);
    bool Open( tcpchar szPath );
    CMarkup* GetXmlPtr(){ return &m_xml; }
private:
    CMarkup m_xml;
};

