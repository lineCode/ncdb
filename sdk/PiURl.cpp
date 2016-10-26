#include "StdAfx.h"
#include "PiURl.h"
#include "define_gnl.h"
#include "functional.h"
#include "dostring.h"
#define     STRING_PARAM_BEGIN  _T("?")
#define     STRING_TAG_FLAG  _T("#")

#define     STRING_PARAM_SPLIT  _T("&")
#define     STRING_PARAM_NAME_VALUE_SPLIT  _T("=")

CPiURl::CPiURl( tcpchar szUrl )
{
    m_strUrl = szUrl;
}

CPiURl::CPiURl( const tstring& szUrl )
{
    m_strUrl = szUrl;
}


CPiURl::~CPiURl(void)
{
}

tstring CPiURl::GetParamValue( int nCount )
{
    size_t nPos = m_strUrl.find(STRING_PARAM_BEGIN);
    if ( ! IsValidStrPos(nPos))
    {
        nPos = m_strUrl.find(_T("#"));
    }
    FOR_COUNT(nCount - 1, i)
    {
        if(!IsValidStrPos(nPos))
        {
            break;
        }
        nPos = m_strUrl.find(STRING_PARAM_SPLIT, nPos + 1);
    }

    size_t nPosEnd = m_strUrl.find(STRING_PARAM_SPLIT, nPos + 1);
    size_t nLength = nPosEnd - nPos - 1;
    if ( !IsValidStrPos(nPosEnd) )
    {
        nLength = tstring::npos;
    }
    tstring strParamField = m_strUrl.substr(nPos + 1, nLength);
    return strParamField.substr(strParamField.find(STRING_PARAM_NAME_VALUE_SPLIT) + 1);
}

tstring CPiURl::GetParamValue( tcpchar szParam )
{
    /*先截取出参数字符串
        根据分隔符, 左边是参数名, 右边是参数值
    */
    size_t nPos = m_strUrl.find(STRING_PARAM_BEGIN);
    if ( ! IsValidStrPos(nPos))
    {
        nPos = m_strUrl.find(_T("#"));
    }
    

    tstring strParam = m_strUrl.substr(NextPos(nPos));
    LST_STRING lstStr;
    MAP_STRING mapParam;
    lstStr = SpilitStr(strParam, _T("&"));   //根据指定字符, 把字符串分割成一个列表

    MAP_STRING mapValue;
    mapValue = SpilitStrMap(lstStr, _T("="));
    return mapValue[szParam];

    /*FOR_COUNT(nCount - 1, i)
    {
        if(!IsValidStrPos(nPos))
        {
            break;
        }
        nPos = m_strUrl.find(STRING_PARAM_SPLIT, nPos + 1);
    }

    size_t nPosEnd = m_strUrl.find(STRING_PARAM_SPLIT, nPos + 1);
    size_t nLength = nPosEnd - nPos - 1;
    if ( !IsValidStrPos(nPosEnd) )
    {
        nLength = tstring::npos;
    }
    tstring strParamField = m_strUrl.substr(nPos + 1, nLength);
    return strParamField.substr(strParamField.find(STRING_PARAM_NAME_VALUE_SPLIT) + 1);*/
}

bool CPiURl::AppendParam( tcpchar szParam, tcpchar szValue )
{
    bool bHasParamFlag = (IsValidStrPos(m_strUrl.find(STRING_PARAM_BEGIN)) 
        || IsValidStrPos(m_strUrl.find(STRING_TAG_FLAG)));

    tcpchar szParamPrefix = STRING_PARAM_BEGIN;
    if (bHasParamFlag)
    {
        szParamPrefix = STRING_PARAM_SPLIT;
    }
    m_strUrl += szParamPrefix ;
    m_strUrl += szParam;
    m_strUrl += STRING_PARAM_NAME_VALUE_SPLIT;
    m_strUrl += szValue;
    return true;
}
