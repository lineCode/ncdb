#pragma once
#include <windows.h>
#include "wke.h"
#include <map>
using std::map;

struct tagWKE_DATA
{
	HWND hParent;
	wkeWebView* pWeb;
	tagWKE_DATA()
		:hParent(NULL)
		, pWeb(NULL)
	{

	}
};

class CWkeMng
{
public:
	CWkeMng();
	~CWkeMng();
public:
	tagWKE_DATA* operator [](HWND hWnd);
	bool Exist(HWND hWnd);
	wkeWebView* GetWebView(HWND hWnd);
	tagWKE_DATA* GetStruct(wkeWebView* pWeb);
private:
	map<HWND, tagWKE_DATA>		m_pWkeData;
};

