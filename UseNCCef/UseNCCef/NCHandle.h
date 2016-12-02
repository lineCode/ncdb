#pragma once
#include "include/cef_client.h"

#include <list>
class CNCCef;

class CNCHandle : public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler
{
public:
	explicit CNCHandle(bool use_views);
	~CNCHandle();


	// Provide access to the single global instance of this object.
	static CNCHandle* GetInstance();

	// CefClient methods:
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE{
		return this;
	}
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE{
		return this;
	}
		virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE{
		return this;
	}

		// CefDisplayHandler methods:
		virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) OVERRIDE;

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefLoadHandler methods:
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) OVERRIDE;

	// Request that all existing browser windows close.
	void CloseAllBrowsers(bool force_close);

	bool IsClosing() const {  return is_closing_; }

	/////////////////////¶îÍâ½Ó¿Ú      ///////////////////////////////////////////////
	void SetParam(CNCCef* pData);
	void SetMode(bool bEmbed);

	
private:
	// Platform-specific implementation.
	void PlatformTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title);

	// True if the application is using the Views framework.
	const bool use_views_;

	// List of existing browser windows. Only accessed on the CEF UI thread.
	typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
	BrowserList browser_list_;
	bool is_closing_;

	CefRefPtr<CefBrowser> m_Browser;
	int m_BrowserId;
	int m_BrowserCount;
	CNCCef*				m_pCefMng;
	bool				m_bEmbed;
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CNCHandle);
};

