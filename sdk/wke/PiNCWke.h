#pragma once
#include "PiTypeDef.h"
#include "NCBrowser.h"
#include "wke/PiWkeDef.h"
#include <map>
#include "WkeMng.h"
using std::multimap;
class CPiNCWke
{
public:
	CPiNCWke();
	~CPiNCWke();
	/************************************************************************
					外部使用接口
	************************************************************************/
public:
	bool Create(HWND hParent, tagCallBack* pTagCallBack);
	bool LoadURL(tcpchar szUrl);
	bool LoadFile(tcpchar szPath);
	bool Show(bool bShow);
	bool SetFocus();
	void Move(int x, int y);
	void Resize(int cx, int cy);
	void ChangeSize(int x, int y, int width, int height);
	bool Destroy();
	/************************************************************************
		fun:	通知外部网页加载完成后有错误发生
		param:	
		memo:	
	************************************************************************/
	bool ExecScript(tcpchar szScript, tstring& strReturnString);
	bool EnableDrag(bool bEnabel);
	bool EnableKeyNavigate(bool bEnabel);
	static bool BindJsFunction(tcpchar szFunc, FunOnClientWebFunc pCallBack);

	operator HWebView();

	/************************************************************************
			内部使用接口
	************************************************************************/
public:
	bool NotifyNavigation(tstring& strUrl);
	void CheckMsg(const wchar_t * szMsg);
	bool HasConsoleError();
	bool NotifyError();
	bool IsLoadError();
	void SendReLoad();
	bool NotifyLoadEnd();
	bool NotifyConsoleMsg(const tstring& strMsg);
	bool ReLoad();
	HNCwkeWebView GetWeb(){ return m_web; }
	HWND GetParent();
	static void SetInst(HINSTANCE hModule);
	void SetLoadError(FunOnLoadError pFun);
private:
	
	HWND CreateHostWnd(HWND hParent);
	bool registerWebViewDllWindowClass();
	static LRESULT CALLBACK WebViewDllWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static void onTitleChanged(HNCwkeWebView webView, void* param, const wkeString* title);
	static void onURLChanged(HNCwkeWebView webView, void* param, const wkeString* url);
	static bool onNavigation(HNCwkeWebView webView, void* param, wkeNavigationType type, const wkeString* url_);
	static HNCwkeWebView onCreateView(HNCwkeWebView webView, void* param, const wkeNewViewInfo* info);

	static void OnwkeConsoleMessageCallback(HNCwkeWebView webView, void* param, const wkeConsoleMessage* message);
	static void OnwkeAlertBoxCallback(HNCwkeWebView webView, void* param, const wkeString* msg);
	static bool OnwkeNavigationCallback(HNCwkeWebView webView, void* param, wkeNavigationType navigationType, const wkeString* url);
	static void OnwkeLoadingFinishCallback(HNCwkeWebView webView, void* param, const wkeString* url, wkeLoadingResult result, const wkeString* failedReason);
	FunOnDragFiles GetDropFilesFun();
	static wkeJSValue JS_CALL JS_CallBack_P2(wkeJSState* es);

	void NCDrawIfNeed();
	void NCDraw();
	bool IsEnableKeyNavigation();
	static CPiNCWke* WkeGetObject(HNCwkeWebView pWeb);
	void NotifyAlert(const wstring& strMsg);
	void LoadOk();
	bool CanReLoad();
private:
	multimap<tstring, tstring>		m_mapErrorConsole;
	HNCwkeWebView					m_web;
	tagWKE_DATA*					m_pWData;
	int								m_nRepeatTotal;
	static		HINSTANCE			m_hInstance;

public:
	static		CWkeMng				g_wkeMng;
	
};
