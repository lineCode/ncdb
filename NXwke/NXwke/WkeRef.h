#pragma once
#include "DllManager.h"
#include "wke.h"

typedef void(*FunwkeInitialize)();
typedef wkeWebView* (*FunwkeCreateWebView)();


class CWkeRef
{
public:
	CWkeRef();
	~CWkeRef();
public:
	bool Init();
	bool UnInit();
	
	void RwkeInitialize();
	wkeWebView* RwkeCreateWebView();

	void RwkeOnNavigation(wkeWebView* webView, wkeNavigationCallback callback, void* param);
	void RwkeOnConsoleMessage(wkeWebView* webView, wkeConsoleMessageCallback callback, void* callbackParam);
	void RwkeOnAlertBox(wkeWebView* webView, wkeAlertBoxCallback callback, void* callbackParam);
	void RwkeOnLoadingFinish(wkeWebView* webView, wkeLoadingFinishCallback callback, void* param);

private:
	CDllManager			m_dllMng;

	//-----------------wke½Ó¿Ú
	//FunwkeInitialize			m_pFunwkeInitialize;
	//FunwkeCreateWebView			m_pFunwkeCreateWebView;
};

