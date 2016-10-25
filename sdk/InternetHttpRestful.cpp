#include "stdafx.h"
#include "InternetHttpRestful.h"
#include <WinInet.h>
#include <afxinet.h>
#include "StrTrans.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CInternetHttpRestful

CInternetHttpRestful::CInternetHttpRestful()
{

}

CInternetHttpRestful::~CInternetHttpRestful()
{

}

BOOL CInternetHttpRestful::PackParam(CString& strCmd, CStringArray& arrKey, CStringArray& arrValue)
{
	int nKeySize = arrKey.GetSize();
	int nValueSize = arrKey.GetSize();
	if (nKeySize != nValueSize)
		return FALSE;

	int i = 0;
	for (i = 0; i < nKeySize; ++i)
	{
		CString strData;
		strData.Format(_T("%s=%s"), arrKey[i], arrValue[i]);
		strCmd += strData;
		if (i < (nKeySize - 1))
			strCmd += _T("&");
	}
	return TRUE;
}

BOOL CInternetHttpRestful::GetReq(const LPCTSTR pUrl, const char strPostData[], CString& strWebString)
{
	CString strServerName = _T("");
	CString strObject = _T("");
	INTERNET_PORT nPort;
	DWORD dwServerType = 0;
	const TCHAR szHeaders[] = _T("Content-Type:application/x-www-form-urlencoded");

	BOOL bRes = AfxParseURL(
		pUrl,
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

	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect, _T("GET"), strObject,
		HTTP_VERSION, NULL, NULL, INTERNET_FLAG_DONT_CACHE, NULL);
	if (NULL == hHttpFile)
	{
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	BOOL bSendRequest = ::HttpSendRequest(hHttpFile, szHeaders, _tcslen(szHeaders),
		(LPVOID)strPostData, strlen(strPostData));
	if (FALSE == bSendRequest)
	{
		::InternetCloseHandle(hHttpFile);
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	TCHAR bufQuery[512] = {0};
	DWORD dwLengthBufQuery = sizeof(bufQuery);
	BOOL bQuery = ::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, bufQuery, &dwLengthBufQuery, NULL);
	if (FALSE == bQuery)
	{
		return FALSE;
	}

	DWORD dwFileSize = (DWORD)_ttol(bufQuery);
	char* fileBuf = new char[dwFileSize + 1];

	DWORD dwByteRead = 0;
	BOOL bRead = ::InternetReadFile(hHttpFile, fileBuf, dwFileSize + 1, &dwByteRead);
	fileBuf[dwByteRead] = 0;
//	strWebString.Format(_T("%s"), fileBuf);
	strWebString = CStrTrans::AnsiToUnicode_Str(fileBuf);
	
	::InternetCloseHandle(hHttpFile);
	::InternetCloseHandle(hConnect);
	::InternetCloseHandle(hSession);

//	AfxMessageBox(strWebString);

	return TRUE;
}

BOOL CInternetHttpRestful::PostReq(const LPCTSTR pUrl, const char strPostData[], CString& strWebString)
{
	CString strServerName = _T("");
	CString strObject = _T("");
	INTERNET_PORT nPort;
	DWORD dwServerType = 0;
	const TCHAR szHeaders[] = _T("Content-Type:application/x-www-form-urlencoded");

	BOOL bRes = AfxParseURL(
		pUrl,
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

	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect, _T("POST"), strObject,
		HTTP_VERSION, NULL, NULL, INTERNET_FLAG_DONT_CACHE, NULL);
	if (NULL == hHttpFile)
	{
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	BOOL bSendRequest = ::HttpSendRequest(hHttpFile, szHeaders, _tcslen(szHeaders),
		(LPVOID)strPostData, strlen(strPostData));
	if (FALSE == bSendRequest)
	{
		::InternetCloseHandle(hHttpFile);
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	TCHAR bufQuery[512] = {0};
	DWORD dwLengthBufQuery = sizeof(bufQuery);
	BOOL bQuery = ::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, bufQuery, &dwLengthBufQuery, NULL);
	if (FALSE == bQuery)
	{
		return FALSE;
	}

	DWORD dwFileSize = (DWORD)_wtol(bufQuery);
	char* fileBuf = new char[dwFileSize + 1];

	DWORD dwByteRead = 0;
	BOOL bRead = ::InternetReadFile(hHttpFile, fileBuf, dwFileSize + 1, &dwByteRead);
	fileBuf[dwByteRead] = 0;
//	strWebString.Format(_T("%s"), fileBuf);
	strWebString = CStrTrans::AnsiToUnicode_Str(fileBuf);

	::InternetCloseHandle(hHttpFile);
	::InternetCloseHandle(hConnect);
	::InternetCloseHandle(hSession);

//	AfxMessageBox(strWebString);

	return TRUE;
}

BOOL CInternetHttpRestful::DeleteReq(const LPCTSTR pUrl, const char strPostData[], CString& strWebString)
{
	CString strServerName = _T("");
	CString strObject = _T("");
	INTERNET_PORT nPort;
	DWORD dwServerType = 0;
	const TCHAR szHeaders[] = _T("Content-Type:application/x-www-form-urlencoded");

	BOOL bRes = AfxParseURL(
		pUrl,
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

	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect, _T("DELETE"), strObject,
		HTTP_VERSION, NULL, NULL, INTERNET_FLAG_DONT_CACHE, NULL);
	if (NULL == hHttpFile)
	{
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	BOOL bSendRequest = ::HttpSendRequest(hHttpFile, szHeaders, _tcslen(szHeaders),
		(LPVOID)strPostData, strlen(strPostData));
	if (FALSE == bSendRequest)
	{
		::InternetCloseHandle(hHttpFile);
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	TCHAR bufQuery[512] = {0};
	DWORD dwLengthBufQuery = sizeof(bufQuery);
	BOOL bQuery = ::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, bufQuery, &dwLengthBufQuery, NULL);
	if (FALSE == bQuery)
	{
		return FALSE;
	}

	DWORD dwFileSize = (DWORD)_wtol(bufQuery);
	char* fileBuf = new char[dwFileSize + 1];

	DWORD dwByteRead = 0;
	BOOL bRead = ::InternetReadFile(hHttpFile, fileBuf, dwFileSize + 1, &dwByteRead);
	fileBuf[dwByteRead] = 0;
	//	strWebString.Format(_T("%s"), fileBuf);
	strWebString = CStrTrans::AnsiToUnicode_Str(fileBuf);

	::InternetCloseHandle(hHttpFile);
	::InternetCloseHandle(hConnect);
	::InternetCloseHandle(hSession);

	//	AfxMessageBox(strWebString);

	return TRUE;
}

BOOL CInternetHttpRestful::PutReq(const LPCTSTR pUrl, const char strPostData[], CString& strWebString)
{
	CString strServerName = _T("");
	CString strObject = _T("");
	INTERNET_PORT nPort;
	DWORD dwServerType = 0;
	const TCHAR szHeaders[] = _T("Content-Type:application/x-www-form-urlencoded");

	BOOL bRes = AfxParseURL(
		pUrl,
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

	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect, _T("PUT"), strObject,
		HTTP_VERSION, NULL, NULL, INTERNET_FLAG_DONT_CACHE, NULL);
	if (NULL == hHttpFile)
	{
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	BOOL bSendRequest = ::HttpSendRequest(hHttpFile, szHeaders, _tcslen(szHeaders),
		(LPVOID)strPostData, strlen(strPostData));
	if (FALSE == bSendRequest)
	{
		::InternetCloseHandle(hHttpFile);
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	TCHAR bufQuery[512] = {0};
	DWORD dwLengthBufQuery = sizeof(bufQuery);
	BOOL bQuery = ::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, bufQuery, &dwLengthBufQuery, NULL);
	if (FALSE == bQuery)
	{
		return FALSE;
	}

	DWORD dwFileSize = (DWORD)_wtol(bufQuery);
	char* fileBuf = new char[dwFileSize + 1];

	DWORD dwByteRead = 0;
	BOOL bRead = ::InternetReadFile(hHttpFile, fileBuf, dwFileSize + 1, &dwByteRead);
	fileBuf[dwByteRead] = 0;
	//	strWebString.Format(_T("%s"), fileBuf);
	strWebString = CStrTrans::AnsiToUnicode_Str(fileBuf);

	::InternetCloseHandle(hHttpFile);
	::InternetCloseHandle(hConnect);
	::InternetCloseHandle(hSession);

	//	AfxMessageBox(strWebString);

	return TRUE;
}

#if 0
BOOL CInternetHttpRestful::UpdownEx(const LPCTSTR pUrl, const char strPostData[], const int nPostDataSize, CString& strWebString)
{
	CString strServerName = _T("");
	CString strObject = _T("");
	INTERNET_PORT nPort;
	DWORD dwServerType = 0;
	const TCHAR szHeaders[] = _T("Content-Type:application/x-www-form-urlencoded");

	BOOL bRes = AfxParseURL(
		pUrl,
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

	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect, _T("POST"), strObject,
		HTTP_VERSION, NULL, NULL, INTERNET_FLAG_DONT_CACHE, NULL);
	if (NULL == hHttpFile)
	{
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	BOOL bSendRequest = ::HttpSendRequest(hHttpFile, szHeaders, _tcslen(szHeaders),
		(LPVOID)strPostData, nPostDataSize);
	if (FALSE == bSendRequest)
	{
		::InternetCloseHandle(hHttpFile);
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	TCHAR bufQuery[512] = {0};
	DWORD dwLengthBufQuery = sizeof(bufQuery);
	BOOL bQuery = ::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, bufQuery, &dwLengthBufQuery, NULL);
	if (FALSE == bQuery)
	{
		return FALSE;
	}

	DWORD dwFileSize = (DWORD)_wtol(bufQuery);
	char* fileBuf = new char[dwFileSize + 1];
    memset(fileBuf, 0, dwFileSize + 1);

	DWORD dwByteRead = 0;
	BOOL bRead = ::InternetReadFile(hHttpFile, fileBuf, dwFileSize + 1, &dwByteRead);
	fileBuf[dwByteRead] = 0;
	//	strWebString.Format(_T("%s"), fileBuf);
	strWebString = CStrTrans::AnsiToUnicode_Str(fileBuf);

	::InternetCloseHandle(hHttpFile);
	::InternetCloseHandle(hConnect);
	::InternetCloseHandle(hSession);

	AfxMessageBox(strWebString);

	return TRUE;
}
#endif

BOOL CInternetHttpRestful::SendReq(const LPCTSTR pUrl, CStringArray& arrKey, CStringArray& arrValue, CString& strWebString, Restful_Type emRfType)
{
	CString strCmd;
	if (!PackParam(strCmd, arrKey, arrValue))
		return FALSE;

	BOOL bRet;

	switch (emRfType)
	{
	case Restful_Post:
		{
			char* pCmd = CStrTrans::UnicodeToAnsi(strCmd.GetBuffer(strCmd.GetLength()));
			bRet = PostReq(pUrl, pCmd, strWebString);
			if (pCmd)
			{
				delete[] pCmd;
				pCmd = NULL;
			}
		}
		break;
	case Restful_Get:
		{
			CString strSendCmd;
			strSendCmd.Format(_T("%s?%s"), pUrl, strCmd);
			bRet = GetReq(strSendCmd, "", strWebString);
		}
		break;
	case Restful_Delete:
		{
			char* pCmd = CStrTrans::UnicodeToAnsi(strCmd.GetBuffer(strCmd.GetLength()));
			bRet = DeleteReq(pUrl, pCmd, strWebString);
			if (pCmd)
			{
				delete[] pCmd;
				pCmd = NULL;
			}
		}
		break;
	case Restful_Put:
		{
			char* pCmd = CStrTrans::UnicodeToAnsi(strCmd.GetBuffer(strCmd.GetLength()));
			bRet = PutReq(pUrl, pCmd, strWebString);
			if (pCmd)
			{
				delete[] pCmd;
				pCmd = NULL;
			}
		}
		break;
	}

	return bRet;
	// return SendReq(pUrl, strCmd, strWebString);
}

BOOL CInternetHttpRestful::UpdownEx(const LPCTSTR pUrl, const char strPostData[], const int nPostDataSize, CString& strWebString)
{
	CString strServerName = _T("");
	CString strObject = _T("");
	INTERNET_PORT nPort;
	DWORD dwServerType = 0;
	const TCHAR szHeaders[] = _T("Content-Type:application/x-www-form-urlencoded");

	BOOL bRes = AfxParseURL(
		pUrl,
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

	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect, _T("POST"), strObject,
		HTTP_VERSION, NULL, NULL, INTERNET_FLAG_DONT_CACHE, NULL);
	if (NULL == hHttpFile)
	{
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	BOOL bSendRequest = ::HttpSendRequest(hHttpFile, szHeaders, _tcslen(szHeaders),
		(LPVOID)strPostData, nPostDataSize);
	if (FALSE == bSendRequest)
	{
		::InternetCloseHandle(hHttpFile);
		::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hSession);
		return FALSE;
	}

	TCHAR bufQuery[512] = {0};
	DWORD dwLengthBufQuery = sizeof(bufQuery);
	BOOL bQuery = ::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, bufQuery, &dwLengthBufQuery, NULL);
	if (FALSE == bQuery)
	{
		return FALSE;
	}

	DWORD dwFileSize = (DWORD)_wtol(bufQuery);
	char* fileBuf = new char[dwFileSize + 1];

	DWORD dwByteRead = 0;
	BOOL bRead = ::InternetReadFile(hHttpFile, fileBuf, dwFileSize + 1, &dwByteRead);
	fileBuf[dwByteRead] = 0;
	//	strWebString.Format(_T("%s"), fileBuf);
	strWebString = CStrTrans::AnsiToUnicode_Str(fileBuf);

	::InternetCloseHandle(hHttpFile);
	::InternetCloseHandle(hConnect);
	::InternetCloseHandle(hSession);

	//	AfxMessageBox(strWebString);

	return TRUE;
}