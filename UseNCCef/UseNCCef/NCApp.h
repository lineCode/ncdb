#pragma once
#include "include/cef_app.h"

class CNCApp : public CefApp,
	public CefBrowserProcessHandler {
public:
	CNCApp(HWND hParent);
	~CNCApp();

	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
		OVERRIDE{ return this; }

		// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() OVERRIDE;

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CNCApp);

public:
private:
	HWND			m_hParent;
	bool			m_bMsgLoopSame;
};

