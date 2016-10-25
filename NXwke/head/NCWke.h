#pragma once
#include "wke.h"
#include <windows.h>

#if defined(__cplusplus)
#define NCWKE_EXTERN_C extern "C"
#else
#define NCWKE_EXTERN_C
#endif

#if defined(BUILDING_NCwke)
#   define NCWKE_SYMBOL __declspec(dllexport)
#else
#   define NCWKE_SYMBOL __declspec(dllimport)
#endif

#define IN
#define OUT

typedef wkeWebView* HWebView;
//typedef wkeWebView* HWND;


typedef void(*FunOnWebFunc)(HWND hParent, const wchar_t* szParam, OUT wchar_t* szReturnString);
typedef void(*FunOnRequestEventFunc)(HWND hParent, const wchar_t* szUrl);
typedef void(*FunOnAlertEventFunc)(HWND hParent, const wchar_t* szMsg);
typedef void(*FunOnConsoleMsgEventFunc)(HWND hParent, const wchar_t* szMsg);
typedef void(*FunOnLoadEndEventFunc)(HWND hParent);

struct tagCallBack
{
	FunOnWebFunc				pOnWebFunc;


	//FunOnRequestEventFunc		pOnRequestEvent;
	wkeNavigationCallback		pOnRequestEvent;

	//FunOnAlertEventFunc			pOnAlertEvent;
	wkeAlertBoxCallback			pOnAlertEvent;
	//FunOnConsoleMsgEventFunc	pOnConsoleMsgEvent;
	wkeConsoleMessageCallback	pOnConsoleMsgEvent;
	//FunOnLoadEndEventFunc		pOnLoadEndEvent;
	wkeLoadingFinishCallback	pOnLoadEndEvent;
};



#define NCWKE_CALL _cdecl
#define NCWKE_API  NCWKE_EXTERN_C NCWKE_SYMBOL

NCWKE_API HWebView      NCWKE_CALL NCInitBrowser(HWND hParent, tagCallBack* pTagCallBack);
NCWKE_API void			NCWKE_CALL NCReSizeBrowser(HWebView hBrowser, int x, int y, int width, int height);
NCWKE_API void			NCWKE_CALL NCShowBrowser(HWebView hBrowser, bool bShow);
NCWKE_API void			NCWKE_CALL NCSetFocusBrowser(HWebView hBrowser);
NCWKE_API void			NCWKE_CALL NCLoadURL(HWebView hBrowser, const wchar_t* szUrl);
NCWKE_API bool			NCWKE_CALL NCExecScript(HWebView hBrowser, const wchar_t* szScript, wchar_t* szReturnString/*1024¸ö×Ö·û*/);
NCWKE_API void			NCWKE_CALL NCFreeBrowser(HWebView hBrowser);
NCWKE_API void			NCWKE_CALL NCDraw(HWebView hBrowser);


/*
	function NCInitBrowser(const AParentHandle : THandle; const ACallBackFunc : PNCBrowserCallBackFunc) : THandle; cdecl;
procedure NCReSizeBrowser(const ABrowserHandle : THandle; const x, y, width, height: Integer); cdecl;
procedure NCShowBrowser(const ABrowserHandle : THandle; const IsShow : Boolean); cdecl;
procedure NCSetFocusBrowser(const ABrowserHandle : THandle); cdecl;
procedure NCLoadURL(const AURL : PChar); cdecl;
function NCExecScript(const ABrowserHandle : THandle; var AReturnString : TIMLongText) : Boolean; cdecl;
	procedure FreeNCBrowser(const ABrowserHandle : THandle); cdecl;
*/

