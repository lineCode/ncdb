#include "StdAfx.h"
#include "PiNetRestful.h"
#include <wininet.h>
#include <afxinet.h>
#include "define_gnl.h"
#include "ResCommon.h"
#include "StrCoding.h"
#include "Math/mathUnit.h"

#define  STRING_URL_PARAM_SEPT      _T("?")
Pi_NameSpace_Using

CPiNetRestful::CPiNetRestful(void)
{
}


CPiNetRestful::~CPiNetRestful(void)
{
}

bool CPiNetRestful::SendHttpQuest( tstring strUrl, string strParam, ENUM_RESTFUL_TYPE nType )
{
    CString strServerName = _T("");
    CString strObject = _T("");
    INTERNET_PORT nPort;
    DWORD dwServerType = 0;
    const TCHAR szHeaders[] = _T("Content-Type:application/x-www-form-urlencoded");
    BOOL bRes = AfxParseURL(
        strUrl.c_str(),
        dwServerType,
        strServerName,
        strObject,
        nPort);

    bRes = bRes && (INTERNET_SERVICE_HTTP == dwServerType);
    if (!bRes)
    {
        return FALSE;
    }

    HINTERNET hSession = ::InternetOpen(NULL, PRE_CONFIG_INTERNET_ACCESS, NULL,
        INTERNET_INVALID_PORT_NUMBER, 0);
    if (NULL == hSession)
    {
        return FALSE;
    }

    HINTERNET hConnect = ::InternetConnect(hSession, strServerName,
        nPort, NULL, NULL, dwServerType, NULL, NULL);
    if (NULL == hConnect)
    {
        return FALSE;
    }

    HINTERNET hHttpFile = ::HttpOpenRequest(hConnect, g_szType[nType], strObject,
        HTTP_VERSION, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, NULL);
    if (NULL == hHttpFile)
    {
        ::InternetCloseHandle(hConnect);
        ::InternetCloseHandle(hSession);
        return FALSE;
    }

    BOOL bSendRequest = ::HttpSendRequest(hHttpFile, szHeaders, _tcslen(szHeaders),
        (LPVOID)strParam.c_str(), strParam.length());
    if (FALSE == bSendRequest)
    {
        ::InternetCloseHandle(hHttpFile);
        ::InternetCloseHandle(hConnect);
        ::InternetCloseHandle(hSession);
        return FALSE;
    }

    if( ! EnsureQueryOk(hHttpFile) )
    {
        return false;
    }


    tchar bufQuery[512] = {0};
    DWORD dwLengthBufQuery = sizeof(bufQuery);
    DWORD dwFileSize = 0;
    BOOL bQuery = ::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, bufQuery, &dwLengthBufQuery, NULL);
    dwFileSize = (DWORD)_ttol(bufQuery);
    if (FALSE == bQuery)
    {
        dwFileSize = NUM_FILE_SIZE_1M;
    }

    NewMemStrAndInit(fileBuf, dwFileSize + 1);
    //char* fileBuf
    //char* fileBuf = new char[dwFileSize + 1];

    DWORD dwByteRead = 0;
    DWORD dwByteReadTotal = 0;
    //Sleep(1000);

    DWORD dwRemain = 0;
    DWORD dwRead = 0;
    
    BOOL bRet = ::InternetQueryDataAvailable(hHttpFile, &dwRemain, 0, 0);

    while(1)
    {
        BOOL bRead = ::InternetReadFile(hHttpFile, fileBuf + dwByteReadTotal, dwRemain, &dwByteRead);
        dwByteReadTotal += dwByteRead;
        if (bRead && 0 == dwByteRead)
        {
            break;//全部获取完
        }
        if (dwByteReadTotal >= dwFileSize)
        {
            break;
        }
    }
    
    //fileBuf[dwByteRead] = 0;
    //	strWebString.Format(_T("%s"), fileBuf);
    fileBuf[dwByteReadTotal] = 0;
#ifdef _UNICODE
    m_strWebData = CStrCoding::AnsiToUnicode(fileBuf);
#else
    m_strWebData = fileBuf;
#endif
    ClearMemArr(fileBuf);

    ::InternetCloseHandle(hHttpFile);
    ::InternetCloseHandle(hConnect);
    ::InternetCloseHandle(hSession);
    return true;
}



const tstring& CPiNetRestful::GetWebData()
{
    //TODO:根据需要做编码转换
    return m_strWebData;
}

/*
bool CPiNetRestful::CheckServerOk()
{
    return true;
}*/

bool CPiNetRestful::GetDistWebData( tstring& strData )
{
    strData = m_strWebData;
    return true;
}

tstring CPiNetRestful::MakeParam( MAP_STRING mapString )
{
    //df=343&dd=44
    tstring strData;
    tstring strTemp;
    
    FOR_EACH_ITER(MAP_STRING, mapString, it)
    {
        strTemp = it->first;
        strTemp += _T("=");
        strTemp +=it->second;
        strTemp += _T("&");

        strData += strTemp;
    }
    if (strData.length())
    {
        *strData.rbegin() = 0;
    }
    return strData.c_str();
}

string CPiNetRestful::MakeParam( MAP_STRINGA mapString )
{
    //df=343&dd=44
    string strData;
    string strTemp;

    FOR_EACH_ITER(MAP_STRINGA, mapString, it)
    {
        strTemp = it->first;
        strTemp += ("=");
        strTemp +=it->second;
        strTemp += ("&");

        strData += strTemp;
    }
    if (strData.length())
    {
        *strData.rbegin() = 0;
    }
    return strData.c_str();
}

bool CPiNetRestful::SwitchSendParam( tstring& strParam )
{
    return true;
}

bool CPiNetRestful::Send( ENUM_RESTFUL_TYPE nType )
{
    m_strUrl = this->GetUrl();
    m_MapParam = this->GetParam();
    DoParam();
    m_strParam = this->MakeParam(m_MapParam);
    this->SwitchSendParam(m_strParam);
    string strParamTemp;
    
    AppendUrlParam(nType);
    

#ifdef _UNICODE
    strParamTemp = CStrCoding::UnicodeToAnsi(m_strParam.c_str());
#else
    strParamTemp = m_strParam;
#endif
        if( ! this->SendHttpQuest(m_strUrl, strParamTemp, nType))
    {
        return false;
    }
    return true;
}

bool CPiNetRestful::Send()
{
    return this->Send(m_SendType);
}

bool CPiNetRestful::EnsureQueryOk( HINTERNET hInet )
{
    TCHAR cBuf[20] = {0};
    UNLONG dwBufLen = sizeof(cBuf);
    UNLONG dwIndex = 0;
    if(!HttpQueryInfo(hInet, HTTP_QUERY_STATUS_CODE, cBuf, &dwBufLen, &dwIndex))
    {
    }

    int nState = _ttol(cBuf);
    return nState == HTTP_STATUS_OK;
}

bool CPiNetRestful::AppendUrlParam( ENUM_RESTFUL_TYPE nType )
{
    if (Restful_Get != nType)
    {
        return true;
    }

    if ( m_strParam == _T(""))
    {
        return true;
    }
    m_strUrl += STRING_URL_PARAM_SEPT + m_strParam;
    m_strParam = _T("");
    return true;
}

bool CPiNetRestful::DoParam()
{
    throw std::exception("The method or operation is not implemented.");
}

tstring CPiNetRestful::GetUrl()
{
    return m_strUrl;
}

MAP_STRING CPiNetRestful::GetParam()
{
    return m_MapParam;
}

bool CPiNetRestful::SendSync( StringCRef strUrl, StringCRef strParam, ENUM_RESTFUL_TYPE nType, tstring& strDataWebRet )
{
    CPiNetRestful piNet;
    piNet.m_strUrl = strUrl;
    piNet.m_strParam = strParam;
    piNet.m_SendType = nType;
    
    piNet.AppendUrlParam(nType);

    string strParamTemp;
#ifdef _UNICODE
    strParamTemp = CStrCoding::UnicodeToAnsi(piNet.m_strParam.c_str());
#else
    strParamTemp = piNet.m_strParam;
#endif
    if( ! piNet.SendHttpQuest(piNet.m_strUrl, strParamTemp, nType))
    {
        return false;
    }

    strDataWebRet = piNet.GetWebData();
    return true;
}

bool CPiNetRestful::SendHttps( tstring strUrl, tstring strParam )
{
   /* string urlA     = CStrCoding::UnicodeToAnsi(strUrl.c_str());
    string paramA   = CStrCoding::UnicodeToAnsi(strParam.c_str());

    CURL *curl;
    curl = curl_easy_init();

    if(curl) 
    {
        curl_easy_setopt(curl, CURLOPT_POST, 1 );
        curl_easy_setopt(curl, CURLOPT_URL, urlA.c_str());

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, paramA.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, paramA.length());

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CHttp::ProcessResult);
        //curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
    }

    CURLcode urlCode = curl_easy_perform(curl);
    curl_easy_cleanup(curl);	*/
    return true;
}
