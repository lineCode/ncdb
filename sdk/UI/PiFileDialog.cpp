#include "stdafx.h"
#include "PiFileDialog.h"
#include "PiUITool.h"
#include "PiWindowPack.h"

#define STRING_WND_PROP_NAME _T("propPiFileDialog")
typedef CPIUITool::tagSELECT_FILE_DIR	tagSELECT_FILE_DIR;
static LONG	g_lOriWndProc;

CPiFileDialog::CPiFileDialog(tcpchar szTitle, tcpchar szFilter /*= nullptr*/, tcpchar szInitDir /*= nullptr*/, bool bSelectMulti /*= true*/)
	:CFileDialog(true, NULL, NULL, (bSelectMulti ? OFN_ALLOWMULTISELECT : 0 )| OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL)
	, m_bInit(false)
{
	m_ofn.lpstrTitle = szTitle;
	m_ofn.lpstrInitialDir = szInitDir;
	m_pIFileDialog = (IFileDialog*)CFileDialog::m_pIFileDialog;
}

CPiFileDialog::~CPiFileDialog()
{
}

void CPiFileDialog::OnOK()
{
	int n = 0;
	n++;
}

void CPiFileDialog::OnInitDone()
{
	int n = 0;
	n++;
}

BOOL CPiFileDialog::OnFileNameOK()
{
	EndSelect();
	return TRUE;
}


LRESULT __stdcall  CPiFileDialog::_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool bDeal = false;
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (wParam == IDOK)
		{
			bDeal = true;
			/*EndDialog(hwnd, 1);
			break;*/

			//TODO:如果选择多个， 标志给外部
			CPiFileDialog* pFile = (CPiFileDialog*)GetProp(hwnd, STRING_WND_PROP_NAME);
			pFile->EndSelect();
			::EndDialog(hwnd, IDOK);
			RemoveProp(hwnd, STRING_WND_PROP_NAME);
		}
		break;
	default:
		break;
	}
	}
	if (bDeal)
	{
		return 1;
	}
	int i = CallWindowProc((WNDPROC)g_lOriWndProc, hwnd, uMsg, wParam, lParam);
	return i;
	
}


void CPiFileDialog::OnFileNameChange()
{
	
	if (m_bInit)
	{
		return;
	}
	//HideControl(IDOK);
	//return;
	OnInit();
	return;



	IFileDialog* pIFD = nullptr;
	IFolderView2 *pFolderView;
	HRESULT hr = IUnknown_QueryService((IUnknown*)m_pIFileDialog, SID_SFolderView, IID_PPV_ARGS(&pFolderView));
	//pIFD->QueryInterface(SID_SFolderView, IID_PPV_ARGS(&pShellView));
	HRESULT hRet = S_FALSE;
	IShellItemArray* pIArr = nullptr;
	hRet = pFolderView->GetSelection(true, &pIArr);

	DWORD dwCount = 0;
	hRet = pIArr->GetCount(&dwCount);
	if (!dwCount)
	{
		return ;
	}
	for (UINT i = 0; i < dwCount; ++i)
	{
		IShellItem* pItem = nullptr;
		hRet = pIArr->GetItemAt(i, &pItem);
		LPWSTR szName = nullptr;
		hRet = pItem->GetDisplayName(SIGDN_FILESYSPATH, &szName);
		CoTaskMemFree(szName);
	}


	int n = 0;
	n++;
}

void CPiFileDialog::OnButtonClicked(DWORD dwIDCtl)
{
	m_strSelect.clear();
	IFileDialog* pIFD = nullptr;
	IFolderView2 *pFolderView;
	HRESULT hr = IUnknown_QueryService((IUnknown*)m_pIFileDialog, SID_SFolderView, IID_PPV_ARGS(&pFolderView));
	if (!SUCCEEDED(hr))
	{
		return;
	}
	//pIFD->QueryInterface(SID_SFolderView, IID_PPV_ARGS(&pShellView));

	IShellItemArray* pIArr = nullptr;
	hr = pFolderView->GetSelection(true, &pIArr);
	if (!SUCCEEDED(hr))
	{
		return;
	}
	DWORD dwCount = 0;
	hr = pIArr->GetCount(&dwCount);
	if (!SUCCEEDED(hr))
	{
		return;
	}
	if (!dwCount)
	{
		return;
	}
	for (UINT i = 0; i < dwCount; ++i)
	{
		IShellItem* pItem = nullptr;
		hr = pIArr->GetItemAt(i, &pItem);
		if (!SUCCEEDED(hr))
		{
			break;
		}
		LPWSTR szName = nullptr;
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &szName);
		if (!SUCCEEDED(hr))
		{
			break;
		}
		m_strSelect.push_back(szName);
		CoTaskMemFree(szName);
	}

	int n = 0;
	n++;

	m_pIFileDialog->Close(S_OK);
}

bool CPiFileDialog::Popup()
{
	return DoModal() == IDOK;
}

ARR_STRING CPiFileDialog::GetSelect()
{
	return m_strSelect;
}

bool CPiFileDialog::EndSelect()
{
	OnButtonClicked(IDOK);
	return true;
}

void CPiFileDialog::SetParam(void* pTag)
{
	m_pTag = pTag;
}

bool CPiFileDialog::OnInit()
{
	m_bInit = true;
	//AddPushButton(5, _T("bbbb"));

	HRESULT hr = S_FALSE;
	IOleWindow* pIOle = nullptr;
	if (!SUCCEEDED(hr = m_pIFileDialog->QueryInterface(IID_IOleWindow, (void**)&pIOle)))
	{
		return false;
	}
	HWND hParent = NULL;
	if (!SUCCEEDED(hr = pIOle->GetWindow(&hParent)))
	{
		return false;
	}

	HWND hIDOK = ::GetDlgItem(hParent, IDOK);
	HWND hIDCancel = ::GetDlgItem(hParent, IDCANCEL);
	/*::ShowWindow(hIDOK, SW_HIDE);
	::ShowWindow(hIDCancel, SW_HIDE);*/


	//TODO:替换窗口过程
	tagSELECT_FILE_DIR* pTag = (tagSELECT_FILE_DIR*)m_pTag;
	//居中显示
	HWND hParentToCenter = ::GetDesktopWindow();
	if (pTag->bCenterToParent && pTag->hParent)
	{
		hParentToCenter = pTag->hParent;
	}
	CPiWindowPack::CenterWindow(hParent, hParentToCenter);
	CPiWindowPack::TopMostWindow(hParent);
	::SetDlgItemText(hParent, IDOK, pTag->szBtnOkName);
	SetPropW(hParent, STRING_WND_PROP_NAME, (CPiFileDialog*)(this));
	g_lOriWndProc = ::SetWindowLongW(hParent, GWL_WNDPROC, (LONG)_WndProc);	
	return true;
}
