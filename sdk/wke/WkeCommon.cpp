#include "WkeCommon.h"


CWkeCommon::CWkeCommon()
{
}


CWkeCommon::~CWkeCommon()
{
}

wstring CWkeCommon::GetString(HNCwkeWebView pWeb, wkeJSValue jsV)
{
	wkeJSState* es = wkeGlobalExec(pWeb);
	wstring strRet;
	if (wkeJSIsString(es, jsV))
	{
		strRet = wkeJSToTempStringW(es, jsV);
	}
	return strRet;
}

bool CWkeCommon::IsJSUnderined(HNCwkeWebView pWeb, wkeJSValue vVal)
{
	wkeJSState* es = wkeGlobalExec(pWeb);
	wstring strRet;
	return wkeJSIsUndefined(es, vVal);
}
