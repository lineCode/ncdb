#pragma once
#include <map>
#include <string>
#include "wke.h"
#include "render.h"
#include "ResCommon.h"


using std::map;
using std::string;

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


#define NCWKE_CALL _cdecl
#define NCWKE_API  NCWKE_EXTERN_C NCWKE_SYMBOL

#define IN
#define OUT

typedef wkeWebView* HNCwkeWebView;
typedef HNCwkeWebView HWebView;

/************************************************************************
	回调函数
************************************************************************/

typedef bool (NCWKE_CALL *FunOnClientWebFunc)(const wchar_t* szFunHead, const wchar_t* szParam, OUT wchar_t* szReturnString);
typedef void (NCWKE_CALL *FunOnRequestEventFunc)(HWebView hBrowser, const wchar_t* szUrl);
typedef void (NCWKE_CALL *FunOnAlertEventFunc)(HWebView hBrowser, const wchar_t* szMsg);
typedef void (NCWKE_CALL *FunOnConsoleMsgEventFunc)(HWebView hBrowser, const wchar_t* szMsg);
typedef void (NCWKE_CALL *FunOnLoadEndEventFunc)(HWebView hBrowser);
typedef void (NCWKE_CALL *FunOnLoadError)(HWebView hBrowser);
typedef void (NCWKE_CALL *FunOnDragFiles)(HWebView hBrowser, const wchar_t* szFilePath);

/************************************************************************
	回调结构体声明
************************************************************************/
struct tagCallBack
{
	FunOnRequestEventFunc		pOnRequestEvent;
	FunOnAlertEventFunc			pOnAlertEvent;
	FunOnConsoleMsgEventFunc	pOnConsoleMsgEvent;
	FunOnLoadEndEventFunc		pOnLoadEndEvent;
	FunOnDragFiles				pOnDragFiles;
public:
	tagCallBack()
		: pOnRequestEvent(NULL)
		, pOnAlertEvent(NULL)
		, pOnConsoleMsgEvent(NULL)
		, pOnLoadEndEvent(NULL)
		, pOnDragFiles(NULL)
	{

	}
};

struct tagWKE_DATA
{
	HWND hParent;
	HWebView pWeb;
	tagCallBack tagCB;

	CRender* pRender;
	CLock*	pLock;
	FunOnLoadError	pOnLoadError;	//dll版本兼容， 后期转入回调结构体

	bool	bEnabelDrag;
	bool	bEnabelKeyNavigation;

	tagWKE_DATA()
		:hParent(NULL)
		, pWeb(NULL)
		, pRender(NULL)
		, pLock(NULL)
		, bEnabelDrag(true)
		, bEnabelKeyNavigation(true)
		, pOnLoadError(NULL)
	{

	}
};
typedef map<string, void*>			MAP_JS_BIND;
typedef map<HWND, tagWKE_DATA>		MAP_WKE_DATA;


