#include "stdafx.h"
#include "PiFileDialog.h"
#include "functional.h"


CPiFileDialog::CPiFileDialog(tcpchar szTitle, tcpchar szFilter /*= nullptr*/)
	:CFileDialog(true, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL)
	, m_bInit(false)
{
	//m_IFileDialog
	m_ofn.lpstrTitle = szTitle;
	m_pIFileDialog = (IFileDialog*)CFileDialog::m_pIFileDialog;
	//AddPushButton(1, _T("选择"));
	//AddPushButton(2, _T("bbb"));
	//AddControlItem(1, 2, _T("ci1"));
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
#if 0

	IFileDialog* pIFD = nullptr;
	IFolderView2 *pFolderView;
	OutInfo(_T("m_pIFileDialog"), (int)m_pIFileDialog);
	HRESULT hr = IUnknown_QueryService((IUnknown*)m_pIFileDialog, SID_SFolderView, IID_PPV_ARGS(&pFolderView));
	if (hr == S_FALSE)
	{
		OutInfo(_T("IUnknown_QueryService error"));
		return FALSE;
	}

	

	//pIFD->QueryInterface(SID_SFolderView, IID_PPV_ARGS(&pShellView));
	HRESULT hRet = S_FALSE;
	IShellItemArray* pIArr = nullptr;
	hRet = pFolderView->GetSelection(true, &pIArr);
	
	DWORD dwCount = 0;
	hRet = pIArr->GetCount(&dwCount);
	if (!dwCount)
	{
		return FALSE;
	}
	for (UINT i = 0; i < dwCount; ++i)
	{
		IShellItem* pItem = nullptr;
		hRet = pIArr->GetItemAt(i, &pItem);
		LPWSTR szName = nullptr;
		hRet = pItem->GetDisplayName(SIGDN_FILESYSPATH, &szName);
		OutInfo(szName);
		CoTaskMemFree(szName);
	}
	
	int n = 0;
	n++;
#endif
	return FALSE;
}

LONG g_lOriWndProc = NULL;

LRESULT static __stdcall  _WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			CPiFileDialog* pFile = (CPiFileDialog*)GetProp(hwnd, _T("ttt"));
			pFile->EndSelect();
			EndDialog(hwnd, IDOK);
			RemoveProp(hwnd, _T("ttt"));
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
	
	{
		m_bInit = true;
		//AddPushButton(5, _T("bbbb"));

		HRESULT hr = S_FALSE;
		IOleWindow* pIOle = nullptr;
		hr = m_pIFileDialog->QueryInterface(IID_IOleWindow, (void**)&pIOle);
		HWND hParent = NULL;
		hr = pIOle->GetWindow(&hParent);
		
		HWND hIDOK = ::GetDlgItem(hParent, IDOK);
		HWND hIDCancel = ::GetDlgItem(hParent, IDCANCEL);
		/*::ShowWindow(hIDOK, SW_HIDE);
		::ShowWindow(hIDCancel, SW_HIDE);*/


		//TODO:替换窗口过程
		HWND hDlgCommon = hParent;
		::SetDlgItemText(hDlgCommon, IDOK, _T("发送吧"));
		SetPropW(hDlgCommon, _T("ttt"), (CPiFileDialog*)(this));
		g_lOriWndProc = ::SetWindowLongW(hDlgCommon, GWL_WNDPROC, (LONG)_WndProc);


	}

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
		OutInfo(szName);
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
	//pIFD->QueryInterface(SID_SFolderView, IID_PPV_ARGS(&pShellView));
	HRESULT hRet = S_FALSE;
	IShellItemArray* pIArr = nullptr;
	hRet = pFolderView->GetSelection(true, &pIArr);

	DWORD dwCount = 0;
	hRet = pIArr->GetCount(&dwCount);
	if (!dwCount)
	{
		return;
	}
	for (UINT i = 0; i < dwCount; ++i)
	{
		IShellItem* pItem = nullptr;
		hRet = pIArr->GetItemAt(i, &pItem);
		LPWSTR szName = nullptr;
		hRet = pItem->GetDisplayName(SIGDN_FILESYSPATH, &szName);
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
