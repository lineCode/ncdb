#pragma once
#include <windows.h>
#include "wke/PiWkeDef.h"

//#include "NCWke.h"
class CPiNCWke;

class CWkeMng
{
public:
	CWkeMng();
	~CWkeMng();
public:
	typedef map<HWND, CPiNCWke*> MAP_WKE;

	//tagWKE_DATA* operator [](HWebView pWeb);
	//tagWKE_DATA* GetFirst();
	//bool Exist(HWND hWnd);
	HWebView GetWebView(HWND hWnd);
	//tagWKE_DATA* GetStruct(HWebView pWeb);
	//MAP_WKE_DATA::iterator GetStructIter(HWebView pWeb);
	//CRender*		GetRender(HWebView pWeb);
	/************************************************************************
			js°ó¶¨
	************************************************************************/
	bool AddJsBind(const wchar_t* szFunc, void* pCallBack);
	void* GetJsCallBack(const string& strName);
	void* GetJsFirstBind();
	//bool Release(HWebView pWeb);
	//void Erase(MAP_WKE_DATA::iterator it);
	void Erase(CPiNCWke* pWke);
	//bool IsValidWeb(HWebView pWeb);

	CPiNCWke*			CreateWke();
	CPiNCWke*			CreateWke(HWND hParent, tagCallBack* pTagCallBack);
	CPiNCWke*			GetObj(HWebView pWeb);
protected:
	//tagWKE_DATA* operator [](HWND hWnd);

private:
	//MAP_WKE_DATA				m_pWkeData;
	MAP_JS_BIND					m_mapJsCallBack;

	auto_ptr<CLock>				m_pLockWkeData;
	auto_ptr<CLock>				m_pLockJsCB;

	//list<CPiNCWke*>				m_lstWke;
	MAP_WKE						m_mapWke;
};
