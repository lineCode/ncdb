#include "stdafx.h"
#include "NCCef.h"
#include "include/cef_sandbox_win.h"
#include "NCApp.h"
#include "NCHandle.h"
#include <string>
//#include "functional.h"
using namespace std;

CNCCef::CNCCef(HINSTANCE hInstance)
	:m_hInstance(hInstance)
	,m_main_args(hInstance)
{
}


CNCCef::~CNCCef()
{
}

int CNCCef::NeedReturn()
{
	CefEnableHighDPISupport();
	void* sandbox_info = NULL;

	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	int exit_code = CefExecuteProcess(m_main_args, NULL, sandbox_info);
	if (exit_code >= 0) {
		// The sub-process has completed so return here.
		return exit_code;
	}
	return -1;
}

bool CNCCef::Init(bool bEmbed /*= false*/)
{
	void* sandbox_info = NULL;

	CefSettings settings;

	// SimpleApp implements application-level callbacks for the browser process.
	// It will create the first browser instance in OnContextInitialized() after
	// CEF has initialized.
	m_app = new CNCApp(m_hParent);
	m_bEmbed = bEmbed;
	// Initialize CEF.
	settings.multi_threaded_message_loop = m_bEmbed;
	//settings.single_process = true;

	//CefInitialize(m_main_args, settings, m_app, sandbox_info);
	CefInitialize(m_main_args, settings, m_app, sandbox_info);
	//CefRunMessageLoop();

	// Shut down CEF.
	//CefShutdown();
	return true;
}

void CNCCef::Create(HWND hParent)
{
	// SimpleHandler implements browser-level callbacks.
	m_handler = (new CNCHandle(false));
	m_handler->SetParam(this);
	m_handler->SetMode(m_bEmbed);
	// Specify CEF browser settings here.
	CefBrowserSettings browser_settings;

	
	std::string url;
	{
		CefRefPtr<CefCommandLine> command_line =
			CefCommandLine::GetGlobalCommandLine();
		// Check if a "--url=" value was provided via the command-line. If so, use
		// that instead of the default URL.
		url = command_line->GetSwitchValue("url");
	}
	if (url.empty())
	{
		url = "http://www.baidu.com";
	}
	

	// Information used when creating the native window.
	CefWindowInfo window_info;


	RECT rt = { 00, 40, 900, 900 };
	if (m_bEmbed)
	{
		window_info.SetAsChild(hParent, rt);
	}
	else
	{
		window_info.SetAsPopup(NULL, _T("cefPiPi"));

	}
	
	// Create the first browser window.
	CefBrowserHost::CreateBrowser(window_info, m_handler, url, browser_settings,
		NULL);
	
}

bool CNCCef::UnInit()
{
	if (m_bEmbed)
	{
		CefShutdown();
	}
	return true;
}

void CNCCef::Start()
{
	if (!m_bEmbed)
	{
		CefRunMessageLoop();
		CefShutdown();
	}
	
}

bool CNCCef::Load(LPCTSTR szUrl)
{
	if (!m_browser.get())
	{
		return false;
	}
	m_browser->GetMainFrame()->LoadURL(szUrl);
	return true;
}

void CNCCef::AddBrowser(CefRefPtr<CefBrowser> br)
{
	m_browser = br;
}

bool CNCCef::FreeBrowser()
{
	if (!m_browser.get())
	{
		return false;
	}
	m_browser->GetHost()->CloseBrowser(false);
	return true;
}

bool CNCCef::IsClosing()
{
	return m_handler->IsClosing();
}

void CNCCef::Test()
{
	//wstring strJS = _T("testNC();");
	/*wstring strJS = GetFileDataStr(_T("d:\\test.txt"), true);
	m_browser->GetMainFrame()->ExecuteJavaScript(strJS.c_str(), m_browser->GetMainFrame()->GetURL(), 0);*/
}
