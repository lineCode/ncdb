#include "stdafx.h"
#include "NCHandle.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "NCCef.h"

namespace {

	CNCHandle* g_instance = NULL;

}  // namespace


CNCHandle::CNCHandle(bool use_views)
	: use_views_(use_views),
	is_closing_(false) 
	, m_bEmbed(false)
{
	DCHECK(!g_instance);
	g_instance = this;
}

CNCHandle::~CNCHandle()
{
}

void CNCHandle::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	CEF_REQUIRE_UI_THREAD();

	if (use_views_) {
		// Set the title of the window using the Views framework.
		CefRefPtr<CefBrowserView> browser_view =
			CefBrowserView::GetForBrowser(browser);
		if (browser_view) {
			CefRefPtr<CefWindow> window = browser_view->GetWindow();
			if (window)
				window->SetTitle(title);
		}
	}
	else {
		// Set the title of the window using platform APIs.
		PlatformTitleChange(browser, title);
	}

}

void CNCHandle::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	// Must be executed on the UI thread.
	CEF_REQUIRE_UI_THREAD();
	//m_Browser = browser;

	//TODO:如果增加保存对CefBrowser的智能引用 退出会崩溃
	if (!m_Browser.get())   {
		// Keep a reference to the main browser.
		m_Browser = browser;
		m_BrowserId = browser->GetIdentifier();
		if (m_pCefMng)
		{
			m_pCefMng->AddBrowser(m_Browser);
		}
	}

	// Keep track of how many browsers currently exist.
	m_BrowserCount++;

	// Add to the list of existing browsers.
	browser_list_.push_back(browser);
}

bool CNCHandle::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.
	if (browser_list_.size() == 1) {
		// Set a flag to indicate that the window close should be allowed.
		is_closing_ = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}

void CNCHandle::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	// Remove from the list of existing browsers.
	BrowserList::iterator bit = browser_list_.begin();
	for (; bit != browser_list_.end(); ++bit) {
		if ((*bit)->IsSame(browser)) {
			browser_list_.erase(bit);
			break;
		}
	}

	if (browser_list_.empty()) {
		// All browser windows have closed. Quit the application message loop.
		if (!m_bEmbed)
		{
			CefQuitMessageLoop();
		}	
	}
}

void CNCHandle::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
	CEF_REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED)
		return;

	// Display a load error message.
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		"<h2>Failed to load URL " << std::string(failedUrl) <<
		" with error " << std::string(errorText) << " (" << errorCode <<
		").</h2></body></html>";
	frame->LoadString(ss.str(), failedUrl);
}

void CNCHandle::CloseAllBrowsers(bool force_close)
{
	if (!CefCurrentlyOn(TID_UI)) {
		// Execute on the UI thread.
		CefPostTask(TID_UI,
			base::Bind(&CNCHandle::CloseAllBrowsers, this, force_close));
		return;
	}

	if (browser_list_.empty())
		return;

	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
		(*it)->GetHost()->CloseBrowser(force_close);
}

void CNCHandle::PlatformTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
	SetWindowText(hwnd, std::wstring(title).c_str());
}

CNCHandle* CNCHandle::GetInstance()
{
	return g_instance;
}

void CNCHandle::SetParam(CNCCef* pData)
{
	m_pCefMng = pData;
}

void CNCHandle::SetMode(bool bEmbed)
{
	m_bEmbed = bEmbed;
}
