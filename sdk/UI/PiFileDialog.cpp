#include "stdafx.h"
#include "PiFileDialog.h"
#include "PiUITool.h"
#include "PiWindowPack.h"
#include "PathLight.h"
#include "StrCoding.h"

#define STRING_WND_PROP_NAME _T("propPiFileDialog")
typedef CPIUITool::tagSELECT_FILE_DIR	tagSELECT_FILE_DIR;
static LONG	g_lOriWndProc;

CPiFileDialog::CPiFileDialog(tcpchar szTitle, tcpchar szFilter /*= nullptr*/, tcpchar szInitDir /*= nullptr*/, bool bSelectMulti /*= true*/)
	:CFileDialog(true, NULL, NULL, (bSelectMulti ? OFN_ALLOWMULTISELECT : 0) | OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL)
	, m_bInit(false)
	, m_hParentDlg(NULL)
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
	return EndSelect() ? TRUE : FALSE;
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
			if (pFile->EndSelect())
			{
				RemoveProp(hwnd, STRING_WND_PROP_NAME);
			}
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
	/************************************************************************
		处理顺序
			用户目录中的选择
			用户文本框输入
				如果为文件路径则为用户选择的路径， 否则返回当前目录
	************************************************************************/
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
		m_strSelect.push_back(pw2psSmart(szName));
		CoTaskMemFree(szName);
	}
	
	do 
	{
		//用户没选择的情况， 有1个item， 为当前路径。 如果用户没选择， 处理文本框的值是否为用户选择的文件
		if (dwCount == 1)
		{
			IShellItem* pISI = nullptr;
			CPathLight path(m_strSelect.at(0));
			tstring strDirCurrent;
			tstring strTemp;
			LPWSTR szTemp = nullptr;

			if (!SUCCEEDED(m_pIFileDialog->GetFolder(&pISI)))
			{
				break;
			}
			if (!SUCCEEDED(pISI->GetDisplayName(SIGDN_FILESYSPATH, &szTemp)))
			{
				break;
			}
			strDirCurrent = pw2psSmart(szTemp);
			CoTaskMemFree(szTemp);

			if (path.GetPath() != strDirCurrent)
			{
				//用户选择了一个项
				break;
			}

			//有选择按选择的项， 没选择给系统默认的
			//TODO:获取文本框的文本
			if (!SUCCEEDED(m_pIFileDialog->GetFileName(&szTemp)))
			{
				break;
			}
			strTemp = pw2psSmart(szTemp);
			CoTaskMemFree(szTemp);
			if (strTemp.empty())
			{
				//没选择， 没输入有效文件
				m_strSelect.clear();
				return;
			}
			path = strTemp;
			CPathLight pathAppend(strDirCurrent);
			pathAppend += path.GetPath();
			if (path.IsDirExist())
			{
				//进入指定目录
			}
			else if (path.IsFileExist())
			{
				//一个文件， 用户选择的文件
				m_strSelect.clear();
				m_strSelect.push_back(path);
			}
			else if (pathAppend.IsFileExist())
			{
				//单纯输入一个存在的文件名
				m_strSelect.clear();
				m_strSelect.push_back(pathAppend);
			}
			else
			{
				m_strSelect.clear();
				return;
			}
		}
	} while (0);
	
	int n = 0;
	n++;

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
	if (m_strSelect.size())
	{
		//m_pIFileDialog->Close(S_OK);
		::EndDialog(m_hParentDlg, IDOK);
		return true;
	}
	return false;
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
	m_hParentDlg = NULL;
	if (!SUCCEEDED(hr = pIOle->GetWindow(&m_hParentDlg)))
	{
		return false;
	}


	//TODO:替换窗口过程
	tagSELECT_FILE_DIR* pTag = (tagSELECT_FILE_DIR*)m_pTag;
	//居中显示
	HWND hParentToCenter = ::GetDesktopWindow();
	if (pTag->bCenterToParent && pTag->hParent)
	{
		hParentToCenter = pTag->hParent;
	}
	CPiWindowPack::CenterWindow(m_hParentDlg, hParentToCenter);
	
	//CPiWindowPack::TopMostWindow(hParent);
	

	::SetDlgItemText(m_hParentDlg, IDOK, pTag->szBtnOkName);
	SetPropW(m_hParentDlg, STRING_WND_PROP_NAME, (CPiFileDialog*)(this));
	g_lOriWndProc = ::SetWindowLongW(m_hParentDlg, GWL_WNDPROC, (LONG)_WndProc);
	return true;
}
