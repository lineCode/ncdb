#include "WkeRef.h"
#include "define_gnl.h"

CWkeRef::CWkeRef()
{
}


CWkeRef::~CWkeRef()
{
}

bool CWkeRef::Init()
{
	//TODO:加载wke.dll模块， 初始化相关类对象
	/*DealBoolRet(m_dllMng.Load("wke.dll"));

	DealBoolRet(m_pFunwkeInitialize				= (FunwkeInitialize)		m_dllMng.GetProc("wkeInitialize"));
	DealBoolRet(m_pFunwkeCreateWebView			= (FunwkeCreateWebView)		m_dllMng.GetProc("wkeCreateWebView"));
	DealBoolRet(m_pFunwkeInitialize = (FunwkeInitialize)m_dllMng.GetProc("wkeInitialize"));
	DealBoolRet(m_pFunwkeInitialize = (FunwkeInitialize)m_dllMng.GetProc("wkeInitialize"));
	DealBoolRet(m_pFunwkeInitialize = (FunwkeInitialize)m_dllMng.GetProc("wkeInitialize"));
	DealBoolRet(m_pFunwkeInitialize = (FunwkeInitialize)m_dllMng.GetProc("wkeInitialize"));*/

	return true;
}

bool CWkeRef::UnInit()
{
	DealBoolRet(m_dllMng.UnLoad());
	return true;
}


void CWkeRef::RwkeInitialize()
{
	wkeInitialize();

	//(*m_pFunwkeInitialize)();
}

wkeWebView* CWkeRef::RwkeCreateWebView()
{
	return wkeCreateWebView();
	//return (*m_pFunwkeCreateWebView)();
}

void CWkeRef::RwkeOnNavigation(wkeWebView* webView, wkeNavigationCallback callback, void* param)
{

}

void CWkeRef::RwkeOnConsoleMessage(wkeWebView* webView, wkeConsoleMessageCallback callback, void* callbackParam)
{

}

void CWkeRef::RwkeOnAlertBox(wkeWebView* webView, wkeAlertBoxCallback callback, void* callbackParam)
{

}

void CWkeRef::RwkeOnLoadingFinish(wkeWebView* webView, wkeLoadingFinishCallback callback, void* param)
{

}
