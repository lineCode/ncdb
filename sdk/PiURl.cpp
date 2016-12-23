#include "StdAfx.h"
#include "PiURl.h"
#include "define_gnl.h"
#include "functional.h"
#include "dostring.h"
#include "PiString.h"
#include "StrCoding.h"
#define     STRING_PARAM_BEGIN  _T("?")
#define     STRING_TAG_FLAG  _T("#")

#define     STRING_PARAM_SPLIT  _T("&")
#define     STRING_PARAM_NAME_VALUE_SPLIT  _T("=")

Pi_NameSpace_Using

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
    FOR_COUNT((UINT)nCount - 1, i)
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
    lstStr = CPiString::SpilitStr(strParam, _T("&"));   //根据指定字符, 把字符串分割成一个列表

    MAP_STRING mapValue;
	mapValue = CPiString::SpilitStrMap(lstStr, _T("="));
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

tstring CPiURl::DecodeParam()
{
	//把路径后面的url参数从utf8解码, 前面的路径和后面的参数可能是不同编码， 分开解码
	CPiString strSrc(m_strUrl);
	CPiString strDecode = CStrCoding::UrlDecode(m_strUrl);

	int nPosSpilit = strSrc.findFirstOf(_T("#?"));
	if (nPosSpilit < 0)
	{
		return strDecode;
	}
	tstring strDecodePath = CStrCoding::UrlDecode(strSrc.substr(0, nPosSpilit));	
	string sA = t2s(tstring(strSrc.substr(nPosSpilit)));
	tstring strDecodeParam = s2t(CStrCoding::UrlUtf8Decode(sA));
	return strDecodePath + strDecodeParam;
}
