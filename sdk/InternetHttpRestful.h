#pragma once

enum Restful_Type
{
	Restful_Post = 1,
	Restful_Get,
	Restful_Delete,
	Restful_Put,
};

class CInternetHttpRestful
{
public:
	CInternetHttpRestful();
	virtual ~CInternetHttpRestful();

public:
	BOOL PackParam(CString& strCmd, CStringArray& arrKey, CStringArray& arrValue);		// ²ÎÊý·â°ü

public:
	BOOL SendReq(const LPCTSTR pUrl, CStringArray& arrKey, CStringArray& arrValue, CString& strWebString, Restful_Type emRfType);
	BOOL UpdownEx(const LPCTSTR pUrl, const char strPostData[], const int nPostDataSize, CString& strWebString);

public:
	BOOL GetReq(const LPCTSTR pUrl, const char strPostData[], CString& strWebString);
	BOOL PostReq(const LPCTSTR pUrl, const char strPostData[], CString& strWebString);
	BOOL DeleteReq(const LPCTSTR pUrl, const char strPostData[], CString& strWebString);
	BOOL PutReq(const LPCTSTR pUrl, const char strPostData[], CString& strWebString);
//	BOOL UpdownEx(const LPCTSTR pUrl, const char strPostData[], const int nPostDataSize, CString& strWebString);
};