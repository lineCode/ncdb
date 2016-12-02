#pragma once

#include "include/cef_app.h"
#include "include/cef_sandbox_win.h"
#include "include/cef_client.h"
//#include "NCApp.h"
//#include "NCHandle.h"
class CNCHandle;
class CNCApp;
class CNCCef
{
public:
	CNCCef(HINSTANCE hInstance);
	~CNCCef();
public:
	int NeedReturn();
	bool Init(bool bEmbed = false);
	void Create(HWND hParent);
	bool UnInit();
	void Start();
	HWND Parent() const { return m_hParent; }
	void Parent(HWND val) { m_hParent = val; }
	bool Load(LPCTSTR szUrl);
	void AddBrowser(CefRefPtr<CefBrowser> m_Browser);
	bool FreeBrowser();
	bool IsClosing();
	void Test();
private:
	HINSTANCE		m_hInstance;
	HWND		m_hParent;
	bool			m_bEmbed;
	CefMainArgs		m_main_args;
	CefRefPtr<CNCApp>		m_app;

	CefRefPtr<CefBrowser>		m_browser;
	CefRefPtr<CNCHandle>		m_handler;
};

