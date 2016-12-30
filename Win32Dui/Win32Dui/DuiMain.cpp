#include "..\samples\UseNCCef\NCCef.h"
#include "DuiMain.h"
#include "PiHeadKernel.h"
#include "UI\PiUITool.h"
#include "resource.h"
#include <Commdlg.h>
#include "PiDataSource.h"
CDuiMain::CDuiMain()
{
	//m_SkinFolder	= _T("34");
	m_SkinFile		= _T("skin.xml");
	m_WndClassName		= _T("CN_TestMain");
}


CDuiMain::~CDuiMain()
{
}

LRESULT CDuiMain::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ((m_pSendEdit != NULL) && m_pSendEdit->IsFocused()
		&& (uMsg == WM_KEYDOWN) && (wParam == 'V') && (lParam & VK_CONTROL))	// 发送消息框的Ctrl+V消息
	{
		m_pSendEdit->Paste();
		return TRUE;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CDuiMain::InitWindow()
{
	m_pSendEdit = static_cast<DuiLib::CRichEditUI*>(m_pm.FindControl(_T("richSend")));

}



UINT_PTR CALLBACK OFNHookProc(HWND hdlg,
	UINT uiMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	OutInfo(_T("OFNHookProc hwnd:"), (int)hdlg);
	bool bRet = 0;
	switch (uiMsg)
	{
	case WM_INITDIALOG:
	{
		LPOPENFILENAME pOFN = (LPOPENFILENAME)lParam;
		OutInfo(_T("OFNHookProc Flag:"), pOFN->Flags);
		
		//pOFN->
	}
		break;
	case WM_COMMAND:
		
	{
		DWORD dwId = wParam;
		if (dwId == IDC_BTN_SEND)
		{
			OutInfo(_T("IDC_BTN_SEND"));
		}
	}
		break;
	case WM_NOTIFY:
		LPOFNOTIFY lN = (LPOFNOTIFY)lParam;
		switch (lN->hdr.code)
		{
		case 1:
			OutInfo(_T("OFNHookProc ok"));
			break;
		case CDN_SELCHANGE:
			OutInfo(_T("OFNHookProc CDN_SELCHANGE"));
			break;
		case CDN_FOLDERCHANGE:
			OutInfo(_T("OFNHookProc CDN_FOLDERCHANGE"));
			break;
		case CDN_FILEOK:
		{
			OutInfo(_T("OFNHookProc click ok"));
			bRet = 1;
			SetWindowLong(hdlg, DWL_MSGRESULT, 1);
			tstring strName = lN->lpOFN->lpstrFile;
			MessageBox(NULL, strName.c_str(), _T("用户选择了"), MB_OK);
		}
			break;
		default:
			break;
		}

		break;
	
	}
	return bRet;	//0,使用默认处理
}

void CDuiMain::OnClick(DuiLib::TNotifyUI& msg)
{
	DuiLib::CDuiString strName = msg.pSender->GetName();
	if (strName == _T("btn_test"))
	{
		TestCef();
		return;


		/*tstring strPath = CPIUITool::SelectFileOrDir(GetHWND(), _T("e:\\work\\svn\\nc2"), _T("你选择吧"), _T("发送"));
		if (strPath.length())
		{
			MessageBox(NULL, strPath.c_str(), _T("用户选择了"), NULL);
		}

		strPath = CPIUITool::SelectFileOrDir(GetHWND(), _T("e:\\work\\svn\\nc"), NULL, NULL);
		if (strPath.length())
		{
			MessageBox(NULL, strPath.c_str(), _T("用户选择了"), NULL);
		}
		return;*/

		/*
		{
			CoInitialize(NULL);
			TCHAR szBuffer[MAX_PATH] = { 0 };
			BROWSEINFO bi;
			ZeroMemory(&bi, sizeof(BROWSEINFO));
			bi.hwndOwner = NULL;
			bi.pszDisplayName = szBuffer;
			bi.lpszTitle = _T("从下面选择文件或文件夹:");
			bi.ulFlags = BIF_BROWSEINCLUDEFILES | BIF_NEWDIALOGSTYLE;
			LPITEMIDLIST idl = SHBrowseForFolder(&bi);
			if (NULL == idl)
			{
				return;
			}
			SHGetPathFromIDList(idl, szBuffer);
			OutInfo(_T("select dir:"), szBuffer);
			CoUninitialize();

		}
		*/
		{	
			OPENFILENAME ofn;       // common dialog box structure
			wchar_t szFile[260];       // buffer for file name
			//HWND hwnd;              // owner window
			HANDLE hf;              // file handle

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = m_hWnd;
			ofn.lpstrFile = szFile; 
			//
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			//
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLEHOOK | OFN_EXPLORER;
			ofn.lpfnHook = OFNHookProc;

			HRSRC hS = FindResource(NULL, MAKEINTRESOURCE(IDD_DIALOG1), RT_DIALOG);
			HGLOBAL hD = LoadResource(NULL, hS);
			//ofn.hInstance = (HINSTANCE)hD;
			// Display the Open dialog box. 

			if (GetOpenFileName(&ofn) == TRUE)
				hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
				0, (LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
				(HANDLE)NULL);



			/*OPENFILENAME ofn = { 0 };
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = m_hWnd;
			ofn.Flags = OFN_EXPLORER;
			GetOpenFileName(&ofn);*/
		}

	}
	else
	{
		__super::OnClick(msg);
	}
}

bool CDuiMain::TestCef()
{
	m_pCef = new CNCCef(DuiLib::CPaintManagerUI::GetInstance());
	
	int nRet = 0;
	if (m_pCef->NeedReturn())
	{
		OutInfo(_T("cef enter return"));
		return false;
	}
	m_pCef->Parent(m_hWnd);
	m_pCef->Init(true);
	m_pCef->Create(m_hWnd);
	//pCef->Start();
	//m_pCef->Load(_T("sdf"));
	return true;
}

LRESULT CDuiMain::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	RECT rt = { 0, 0, 100, 100 };
	CPiDataSource piSour;
	//piSour.SetWindow(hWnd);
	piSour.PrepareDrag();
	//piSour.GeneralPic(strTempFile.c_str());
	//piSour.Drag(strTempFile.c_str());

	piSour.BeginDrag(_T("e:\\work\\svn\\nc\\src\\“发帖子”韩文怎么写？_百度知道.htm"), rt);
	return 1;
}
