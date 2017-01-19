#pragma once
#include "PiTypeDef.h"
#include "NCBrowser.h"
#include "wke/PiWkeDef.h"
#include <map>
#include "WkeMng.h"
using std::multimap;
/************************************************************************** 
        *  @Copyright (c) 2017, ChenMH, All rights reserved. 
     
        *  @file     : PiNCWke.h 
        *  @version  : ver 1.0 
     
        *  @author   : liwanliang 
        *  @date     : 2017/01/16 17:44 
        *  @brief    : wke对象， 对应每个wke页面 
**************************************************************************/

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
		fun:	执行脚本
		param:	szScript:脚本字符串;     strReturnString: 执行后的返回值
		memo:	
	************************************************************************/
	bool ExecScript(tcpchar szScript, tstring& strReturnString);
	/************************************************************************
		fun:	是否允许响应文件拖拽进来
		param:	
		memo:	
	************************************************************************/
	bool EnableDrag(bool bEnabel);
	/************************************************************************
		fun:	是否允许设置键盘前进后退
		param:	
		memo:	
	************************************************************************/
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
	bool NotifyConsoleError();

	void NotifyError();
	void SetStatus(wkeLoadingResult result, const wkeString* failedReason);
	bool IsLoadError();
	void NotifyLoadError();
	bool NotifyLoadEnd();
	bool NotifyConsoleMsg(const tstring& strMsg);
	/************************************************************************
		fun:	重新加载当前页面
		param:	
		memo:	
	************************************************************************/
	bool ReLoad();
	HNCwkeWebView GetWeb();
	HWND GetParent();
	static void SetInst(HINSTANCE hModule);
	void SetLoadError(FunOnLoadError pFun);
	void ClearData();
	bool DestroySync();
private:
	/************************************************************************
		fun:	创建wke页面依赖的父窗口
		param:	
		memo:	
	************************************************************************/
	HWND CreateHostWnd(HWND hParent);
	bool registerWebViewDllWindowClass();
	static LRESULT CALLBACK WebViewDllWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static void onTitleChanged(HNCwkeWebView webView, void* param, const wkeString* title);
	static void onURLChanged(HNCwkeWebView webView, void* param, const wkeString* url);
	static bool onNavigation(HNCwkeWebView webView, void* param, wkeNavigationType type, const wkeString* url_);
	static HNCwkeWebView onCreateView(HNCwkeWebView webView, void* param, const wkeNewViewInfo* info);

	/************************************************************************
		fun:	wkeView的控制台信息， 警告信息， 重定向， 加载完成的回调
		param:	
		memo:	
	************************************************************************/
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
	void LoadEnd();
	bool IsCancel();
	tstring GetUrl();
	bool PostReload();
	bool DealTimer(int nTimerID);
	bool NotifyDropFile(StringCRef strName);
private:
	multimap<tstring, tstring>		m_mapErrorConsole;
	HNCwkeWebView					m_web;
	HNCwkeWebView					m_webDestroy;		//保存上次销毁前的HWeb
	tagWKE_DATA*					m_pWData;
	int								m_nReloadTimes;
	static		HINSTANCE			m_hInstance;

	wkeLoadingResult				m_result; 
	const wkeString*				m_failedReason;		//ref
	bool							m_bTryReload;		//网页出现错误是否reload
	bool							m_bDestroying;		
public:
	static		CWkeMng				g_wkeMng;
	
};
