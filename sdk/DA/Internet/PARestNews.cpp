#include "StdAfx.h"
#include "PARestNews.h"
#include "Markup.h"
#include "NewsMng.h"
#include "PANetParamMng.h"
#include "PAMsgPushDef.h"
#include "StrCoding.h"
#include "functional.h"



//新闻

CPARestNews::CPARestNews(void)
{
    m_SendType = Restful_Get;
}


CPARestNews::~CPARestNews(void)
{
}

bool CPARestNews::GetNewsData( CNewsMng& newsMng )
{
    /************************************************************************
        一个item节点内为一条新闻
    ************************************************************************/
    
    tstring strTotal;
    CPARestful::GetDistWebData(strTotal);
    //解析xml格式
    
    m_pXML->SetDoc(strTotal);

    if (
        ! (m_pXML->FindElem()
        && m_pXML->IntoElem()
        && m_pXML->FindElem(_T("data"))
        && m_pXML->IntoElem()
        ))
    {
        return false;
    }

    while(m_pXML->FindElem(_T("datum")))
    {
        tagDATA_NEWS* tagNews = new tagDATA_NEWS;

        if( ! GetNewsItem(*tagNews))
        {
            continue;
        }

        newsMng.Add(tagNews);
    }


    int nCircle = NUM_CIRCLE_NEXT;
    newsMng.SetCircle(nCircle);

    return true;
}


tstring CPARestNews::GetUrl()
{
    return CPANetParamMng::GetUrlNews();
}

MAP_STRING CPARestNews::GetParam()
{
    return CPANetParamMng::GetParamNews();
}


bool CPARestNews::GetNewsItem( tagDATA_NEWS &strData )
{
    string strTempA;
    tstring strTemp;
    if(m_pXML->FindChildElem(_T("type"))
        )
    {
        strTemp = m_pXML->GetChildData();
        strData.strType = CStrCoding::Utf8Base64W2Unicode(strTemp.c_str());
    }

    if(m_pXML->FindChildElem(_T("title"))
        )
    {
        strTemp = m_pXML->GetChildData();
        strData.strTitle = CStrCoding::Utf8Base64W2Unicode(strTemp.c_str());
        //OutInfo(_T("title:"), strData.strTitle.c_str());
    }

    if(m_pXML->FindChildElem(_T("content")))
    {
        strTemp = m_pXML->GetChildData();
        strData.strContent = CStrCoding::Utf8Base64W2Unicode(strTemp.c_str());
    }

    if(m_pXML->FindChildElem(_T("url")))
    {
        strData.strTime = m_pXML->GetChildData();
        strTempA = (PCSTR)CStrCoding::DecodeBase64_T(strData.strTime.c_str());
        //CStrCoding::UrlUTF8Decode(strTempA);
        //strTempA = CStrCoding::UrlGB2312Decode(strTempA);
        strData.strUrl = CStrCoding::AnsiToUnicode(strTempA.c_str());
    }

    if(m_pXML->FindChildElem(_T("date")))
    {
        strTemp = m_pXML->GetChildData();
        time_t nData = 0;
        //nData = _ttol(CStrCoding::DecodeBase64Str_T(strTemp.c_str()).c_str());
        nData = _ttol(strTemp.c_str());
        if ( ! nData)
        {
            strData.strTime = _T("");
            
        }
        else
        {
            strData.strTime = Second1970ToString(nData);
        }
    }

    return true;
}


