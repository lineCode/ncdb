#include "StdAfx.h"
#include "XmlUtil.h"
#include "Markup.h"
#include "StrCoding.h"


CXmlUtil::CXmlUtil(tcpchar szDoc)
{
    m_xml.SetDoc(szDoc);
}

CXmlUtil::CXmlUtil( CMarkup& xml )
    :m_xml(xml)
{

}

CXmlUtil::CXmlUtil()
{

}


CXmlUtil::~CXmlUtil(void)
{
}

bool CXmlUtil::GetChildElemData( tcpchar szDoc, tcpchar szElem, tstring& strData )
{
    CMarkup xml;
    if( ! xml.SetDoc(szDoc)
        || ! xml.FindChildElem(szElem))
    {
        return false;
    }

    strData = xml.GetChildData();
    return true;
}


bool CXmlUtil::GetChildElemDataBase64( tcpchar szDoc, tcpchar szElem, tstring& strData )
{
    if( ! GetChildElemData(szDoc, szElem, strData))
    {
        return false;
    }
    strData = CStrCoding::DecodeBase64Str_T(strData.c_str());
    return true;
}

bool CXmlUtil::GetChildData( tcpchar szElem, tstring& strData )
{
    if( ! m_xml.FindChildElem(szElem))
    {
        return false;
    }

    strData = m_xml.GetChildData();
    return true;
}

bool CXmlUtil::Open( tcpchar szPath )
{
    return m_xml.Load(szPath);
}

bool CXmlUtil::Into( tcpchar szElem )
{
    if(m_xml.FindElem(szElem)
        && m_xml.IntoElem())
    {
        return true;
    }
    return false;
}

bool CXmlUtil::IntoChild( tcpchar szElem )
{
    if(m_xml.FindChildElem(szElem)
        && m_xml.IntoElem())
    {
        return true;
    }
    return true;

}
