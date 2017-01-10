
// MFCTest1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTest1.h"
#include "MFCTest1Dlg.h"
#include "afxdialogex.h"
#include "PiWindowPack.h"
#include "functional.h"
#include "ui/PiDataSource.h"
#include "Math/PiMathUnit.h"
#include "EventLight.h"
//#include "UI/PiFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
Pi_NameSpace_Using

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	OutInfo(_T("abdout wnd show"));
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	return TRUE;
}


// CMFCTest1Dlg 对话框



CMFCTest1Dlg::CMFCTest1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCTest1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bBtnDown = false;
	m_bDraging = false;
}

void CMFCTest1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_LIST2, c_FileList);
}    


BEGIN_MESSAGE_MAP(CMFCTest1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CMFCTest1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTest1Dlg::OnBnClickedButton1)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST, OnBegindragFilelist)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMFCTest1Dlg 消息处理程序

BOOL CMFCTest1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	c_FileList.ModifyStyle(0, LVS_SHAREIMAGELISTS);


	// Set up the list columns.

	c_FileList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 0, 0);
	c_FileList.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 0, 1);
	c_FileList.InsertColumn(2, _T("Size"), LVCFMT_LEFT, 0, 2);

	c_FileList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	c_FileList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	c_FileList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);

	// Enable tooltips for items that aren't completely visible.
	c_FileList.InsertItem(0, _T("e:\\work\\svn\\gitlwl\\wkeBrowserConsole.txt"));

	c_FileList.SetExtendedStyle(LVS_EX_INFOTIP);


	Test();
	//c_FileList.InsertItem(0, _T("3333"));

	m_droptarget.SetSelfFlagFormat(CPiDataSource::DataDragSelfFlag());
	//bool SetEventSign(bool bRet = true);
	CEventLight ev;
	
	//std::function<bool(bool)> fDrag = std::bind(&CEventLight::SetEventSign, &ev, std::placeholders::_1);

	m_droptarget.SetDragFun(std::bind(&CMFCTest1Dlg::DealDrag, this, std::placeholders::_1));
	m_droptarget.SetDragFun(std::bind(&CMFCTest1Dlg::DealDragStatic, std::placeholders::_1));
	m_droptarget.DragDropRegister(GetSafeHwnd());

	/*const wchar_t pszFilter[] = _T("EXE File (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		pszFilter, this);

	dlg.m_ofn.lpstrInitialDir = _T("c:\\WINDOWS\\");  //设置对话框默认呈现的路径

	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		/ *如果有多个文件,则
		for(POSITION pos = dlg.GetStartPosition(); pos!=NULL; )
		{
		CString strFilePathName = dlg.GetNextPathName(pos);
		* /
	}*/

	

		
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCTest1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCTest1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCTest1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCTest1Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CAboutDlg* pDlg = new CAboutDlg;
	pDlg->Create(IDD_ABOUTBOX, this);
	//pDlg->ShowWindow(SW_SHOW);
	CPiWindowPack::MoveWindow(pDlg->GetSafeHwnd(), 700, 700);
}


void CMFCTest1Dlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	TestOleDrag();
	{
		CFileDialog fileDialog(TRUE, _T("*.*"), NULL, NULL, NULL);

		if (fileDialog.DoModal() != IDOK)
			return ;

		CString str = fileDialog.GetFileExt();
		str = _T(".") + str;

		SHFILEINFO   sfi;
		SHGetFileInfo(str, 0, &sfi, sizeof(sfi), SHGFI_ICON | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);

		int i = sfi.iIcon;
		CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC);
		HICON hIcon = sfi.hIcon;
		pStatic->SetIcon(hIcon);

		m_btn1.ModifyStyle(0, BS_ICON); //设置Icon属性
		m_btn1.SetIcon(hIcon); //设置图标
		return;

	}
	/*tstring strFilter = _T("All Files (*.*)|*.*|txt Files (*.txt)|*.txt|bin Files (*.exe)|*.exe;*.obj||");

	CPiFileDialog dlgFile(_T("选择多个文件(目录)"), strFilter.c_str());
	if (!dlgFile.Popup())
	{
		return;
	}
	tstring strOut;
	CWnd* pEdit = GetDlgItem(IDC_EDIT_OUTPUT);
	pEdit->SetWindowText(strOut.c_str());

	ARR_STRING lstSel = dlgFile.GetSelect();
	for (auto& strSel : lstSel)
	{
		strOut += strSel;
		strOut += _T("\r\n");
	}
	pEdit->SetWindowText(strOut.c_str());*/
	return;
}

void CMFCTest1Dlg::TestOleDrag()
{	
	
}


void CMFCTest1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	OutInfo(_T("lBtnDown"));
	m_bBtnDown = true;

	m_idSource.SetWindow(m_hWnd);
	m_idSource.SetDragImageSize(200, 200);
	m_idSource.PrepareDrag();

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCTest1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	OutInfo(_T("lBtnUp"));
	m_bBtnDown = false;

	m_idSource.CancelDrag();
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCTest1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值


	/*BeginDrag();
	//DragIng2();
	DragDui();*/
	RECT rtClient = { 0 };
	GetClientRect(&rtClient);
	RECT rtWnd = { 0 };
	GetWindowRect(&rtWnd);
	//int nCaptionHeight = rtWnd.bottom - rtWnd.top - rtClient.bottom;
	int nCaptionHeight = 0;
	POINT ptSize = {200, 200};
	
	RECT rt = { point.x - ptSize.x / 2, point.y + nCaptionHeight - ptSize.y / 2 };
	rt.right = rt.left + ptSize.x;
	rt.bottom = rt.top + ptSize.y;

	
	/*m_idSource.BeginDrag(_T("e:\\work\\svn\\“发帖子”韩文怎么写？_百度知道.htm")
		, rt
	);*/

	tstring strPath(_T("e:\\work\\svn\\1.jpg"));
	m_idSource.BeginDrag(strPath.c_str());
	

	CDialogEx::OnMouseMove(nFlags, point);
}

bool CMFCTest1Dlg::BeginDrag()
{
	if (!m_bBtnDown || m_bDraging)
	{
		return false;
	}
	//first drag
	m_bDraging = true;

	return true;
}

void CMFCTest1Dlg::DragIng()
{
	if (!m_bDraging)
	{
		return;
	}
	{
		COleDataSource*	pSource = new COleDataSource();
		CSharedFile	sf(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
		CString iText;

		//	lets move our text
		GetWindowText(iText);

		//	write name to clipboard
		sf.Write(iText, iText.GetLength());

		HGLOBAL hMem = sf.Detach();
		if (!hMem)
			return;
		pSource->CacheGlobalData(CF_TEXT, hMem);

		//	Do drag and drop!
		pSource->DoDragDrop();

		//	free source
		delete pSource;
		return;
	}
	COleDataSource*  pOleDataSource = new COleDataSource;

	OutInfo(_T("drag begin"));
	tstring strDragData(_T("e:\\work\\html\\prompt.htm"));
	HGLOBAL        hMemData = NULL;
	DWORD		uBufferSize = sizeof(DROPFILES) + strDragData.size() * sizeof(TCHAR) + 1;

	hMemData = GlobalAlloc(GPTR, uBufferSize);
	ASSERT(hMemData != NULL);
	LPDROPFILES lpDropFiles = (LPDROPFILES)GlobalLock(hMemData); //锁定之,并设置相关成员
	ASSERT(lpDropFiles != NULL);
	lpDropFiles->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
	lpDropFiles->fWide = TRUE;
#else
	lpDropFiles->fWide = FALSE;
#endif

	/************************************************************************
	拷贝要拖放的数据
	************************************************************************/

	LPTSTR pszStart = (LPTSTR)((LPBYTE)lpDropFiles + sizeof(DROPFILES));
	pszStart = lstrcpy(pszStart, strDragData.c_str());
	pszStart = _tcsrchr(pszStart, '\0') + 1; //下次的起始位置是上一次结尾+1

	pOleDataSource->Empty();
	pOleDataSource->CacheGlobalData(CF_TEXT, hMemData);

	DROPEFFECT dwRet = pOleDataSource->DoDragDrop(DROPEFFECT_MOVE | DROPEFFECT_COPY);
	delete pOleDataSource;
	OutInfo(_T("drag ing, ret"), dwRet);
}

void CMFCTest1Dlg::DragIng2()
{
	if (!m_bDraging)
	{
		return ;
	}
	//NMLISTVIEW*    pNMLV = (NMLISTVIEW*)pNMHDR;
	HGLOBAL        hgDrop;
	DROPFILES*     pDrop;
	CStringList    lsDraggedFiles;
	//POSITION       pos;
	int            nSelItem;
	CString        sFile;
	UINT           uBuffSize = 0;
	TCHAR*         pszBuff;
	FORMATETC      etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

	//*pResult = 0;   // return value ignored

	// For every selected item in the list, put the filename into lsDraggedFiles.
	int nCount = c_FileList.GetItemCount();
	if (!nCount)
	{
		return;
	}
	OutputDebugString(_T("DragIng2\n"));
	COleDataSource*			pDatasrc = new COleDataSource;

	sFile = _T("e:\\work\\svn\\gitlwl\\MFCTest1\\MFCTest1\\DragDropIm");

	
	uBuffSize += sFile.GetLength() + 1;

	/*pos = c_FileList.GetFirstSelectedItemPosition();

	while (NULL != pos)
	{
		//nSelItem = c_FileList.GetNextSelectedItem(pos);
		c_FileList.GetItemCount()
		sFile = c_FileList.GetItemText(nSelItem, 0);

		lsDraggedFiles.AddTail(sFile);

		// Calculate the # of chars required to hold this string.

		uBuffSize += lstrlen(sFile) + 1;
	}*/

	// Add 1 extra for the final null char, and the size of the DROPFILES struct.

	uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1);

	// Allocate memory from the heap for the DROPFILES struct.

	hgDrop = GlobalAlloc(GHND | GMEM_SHARE, uBuffSize);

	if (NULL == hgDrop)
		return;

	pDrop = (DROPFILES*)GlobalLock(hgDrop);

	if (NULL == pDrop)
	{
		GlobalFree(hgDrop);
		return;
	}

	// Fill in the DROPFILES struct.

	pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
	// If we're compiling for Unicode, set the Unicode flag in the struct to
	// indicate it contains Unicode strings.

	pDrop->fWide = TRUE;
#endif

	// Copy all the filenames into memory after the end of the DROPFILES struct.

	//pos = lsDraggedFiles.GetHeadPosition();
	pszBuff = (TCHAR*)(LPBYTE(pDrop) + sizeof(DROPFILES));

/*
	while (NULL != pos)
	{
		lstrcpy(pszBuff, (LPCTSTR)lsDraggedFiles.GetNext(pos));
		pszBuff = 1 + _tcschr(pszBuff, '\0');
	}*/

	lstrcpy(pszBuff, (LPCTSTR)sFile.GetBuffer());
	pszBuff = 1 + _tcschr(pszBuff, '\0');
	GlobalUnlock(hgDrop);

	// Put the data in the data source.

	pDatasrc->CacheGlobalData(CF_HDROP, hgDrop, &etc);

	// Add in our own custom data, so we know that the drag originated from our 
	// window.  CMyDropTarget::DragEnter() checks for this custom format, and
	// doesn't allow the drop if it's present.  This is how we prevent the user
	// from dragging and then dropping in our own window.
	// The data will just be a dummy bool.
	HGLOBAL hgBool = NULL;

	/*hgBool = GlobalAlloc(GHND | GMEM_SHARE, sizeof(bool));

	if (NULL == hgBool)
	{
		GlobalFree(hgDrop);
		return;
	}*/

	// Put the data in the data source.

	/*etc.cfFormat = g_uCustomClipbrdFormat;

	pDatasrc->CacheGlobalData(g_uCustomClipbrdFormat, hgBool, &etc);
*/


	// Start the drag 'n' drop!

	DROPEFFECT dwEffect = pDatasrc->DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);

	// If the DnD completed OK, we remove all of the dragged items from our
	// list.

	switch (dwEffect)
	{
	case DROPEFFECT_COPY:
	case DROPEFFECT_MOVE:
	{
		// The files were copied or moved.
		// Note: Don't call GlobalFree() because the data will be freed by the drop target.

		for (nSelItem = c_FileList.GetNextItem(-1, LVNI_SELECTED);
			nSelItem != -1;
			nSelItem = c_FileList.GetNextItem(nSelItem, LVNI_SELECTED))
		{
			c_FileList.DeleteItem(nSelItem);
			nSelItem--;
		}

		// Resize the list columns.

		c_FileList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
		c_FileList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
		c_FileList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	}
	break;

	case DROPEFFECT_NONE:
	{
		// This needs special handling, because on NT, DROPEFFECT_NONE
		// is returned for move operations, instead of DROPEFFECT_MOVE.
		// See Q182219 for the details.
		// So if we're on NT, we check each selected item, and if the
		// file no longer exists, it was moved successfully and we can
		// remove it from the list.

		if (g_bNT)
		{
			bool bDeletedAnything = false;

			for (nSelItem = c_FileList.GetNextItem(-1, LVNI_SELECTED);
				nSelItem != -1;
				nSelItem = c_FileList.GetNextItem(nSelItem, LVNI_SELECTED))
			{
				CString sFilename = c_FileList.GetItemText(nSelItem, 0);

				if (0xFFFFFFFF == GetFileAttributes(sFile) &&
					GetLastError() == ERROR_FILE_NOT_FOUND)
				{
					// We couldn't read the file's attributes, and GetLastError()
					// says the file doesn't exist, so remove the corresponding 
					// item from the list.

					c_FileList.DeleteItem(nSelItem);

					nSelItem--;
					bDeletedAnything = true;
				}
			}

			// Resize the list columns if we deleted any items.

			if (bDeletedAnything)
			{
				c_FileList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
				c_FileList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
				c_FileList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);

				// Note: Don't call GlobalFree() because the data belongs to 
				// the caller.
			}
			else
			{
				// The DnD operation wasn't accepted, or was canceled, so we 
				// should call GlobalFree() to clean up.

				GlobalFree(hgDrop);
				GlobalFree(hgBool);
			}
		}   // end if (NT)
		else
		{
			// We're on 9x, and a return of DROPEFFECT_NONE always means
			// that the DnD operation was aborted.  We need to free the
			// allocated memory.

			GlobalFree(hgDrop);
			GlobalFree(hgBool);
		}
	}
	break;  // end case DROPEFFECT_NONE
	}   // end switch


	m_bBtnDown = false;
	m_bDraging = false;
}


void CMFCTest1Dlg::OnBegindragFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_idSource.SetWindow(m_hWnd);
	m_idSource.PrepareDrag();
	//m_idSource.BeginDrag(_T("e:\\work\\svn\\nc\\src\\“发帖子”韩文怎么写？_百度知道.htm"), RECT{ 0, 0, 100, 100 });
	m_idSource.BeginDrag(_T("e:\\temp\\2\\temp.jpg"), RECT{ 0, 0, 100, 100 });
	m_idSource.CancelDrag();
	return;





	OutInfo(_T("OnBegindragFilelist"));
	OutputDebugString(_T("OnBegindragFilelist\n"));
	NMLISTVIEW*    pNMLV = (NMLISTVIEW*)pNMHDR;
	COleDataSource datasrc;
	HGLOBAL        hgDrop;
	DROPFILES*     pDrop;
	CStringList    lsDraggedFiles;
	POSITION       pos;
	int            nSelItem;
	CString        sFile;
	UINT           uBuffSize = 0;
	TCHAR*         pszBuff;
	FORMATETC      etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

	*pResult = 0;   // return value ignored

	// For every selected item in the list, put the filename into lsDraggedFiles.

	pos = c_FileList.GetFirstSelectedItemPosition();

	while (NULL != pos)
	{
		nSelItem = c_FileList.GetNextSelectedItem(pos);
		sFile = c_FileList.GetItemText(nSelItem, 0);

		lsDraggedFiles.AddTail(sFile);

		// Calculate the # of chars required to hold this string.

		uBuffSize += lstrlen(sFile) + 1;
	}

	// Add 1 extra for the final null char, and the size of the DROPFILES struct.

	uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1);

	// Allocate memory from the heap for the DROPFILES struct.

	hgDrop = GlobalAlloc(GHND | GMEM_SHARE, uBuffSize);

	if (NULL == hgDrop)
		return;

	pDrop = (DROPFILES*)GlobalLock(hgDrop);

	if (NULL == pDrop)
	{
		GlobalFree(hgDrop);
		return;
	}

	// Fill in the DROPFILES struct.

	pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
	// If we're compiling for Unicode, set the Unicode flag in the struct to
	// indicate it contains Unicode strings.

	pDrop->fWide = TRUE;
#endif

	// Copy all the filenames into memory after the end of the DROPFILES struct.

	pos = lsDraggedFiles.GetHeadPosition();
	pszBuff = (TCHAR*)(LPBYTE(pDrop) + sizeof(DROPFILES));

	while (NULL != pos)
	{
		lstrcpy(pszBuff, (LPCTSTR)lsDraggedFiles.GetNext(pos));
		pszBuff = 1 + _tcschr(pszBuff, '\0');
	}

	GlobalUnlock(hgDrop);

	// Put the data in the data source.

	datasrc.CacheGlobalData(CF_HDROP, hgDrop, &etc);

	// Add in our own custom data, so we know that the drag originated from our 
	// window.  CMyDropTarget::DragEnter() checks for this custom format, and
	// doesn't allow the drop if it's present.  This is how we prevent the user
	// from dragging and then dropping in our own window.
	// The data will just be a dummy bool.
	HGLOBAL hgBool;

	hgBool = GlobalAlloc(GHND | GMEM_SHARE, sizeof(bool));

	if (NULL == hgBool)
	{
		GlobalFree(hgDrop);
		return;
	}

	// Put the data in the data source.

	etc.cfFormat = g_uCustomClipbrdFormat;

	datasrc.CacheGlobalData(g_uCustomClipbrdFormat, hgBool, &etc);


	// Start the drag 'n' drop!

	DROPEFFECT dwEffect = datasrc.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);

	// If the DnD completed OK, we remove all of the dragged items from our
	// list.

	switch (dwEffect)
	{
	case DROPEFFECT_COPY:
	case DROPEFFECT_MOVE:
	{
		// The files were copied or moved.
		// Note: Don't call GlobalFree() because the data will be freed by the drop target.

		for (nSelItem = c_FileList.GetNextItem(-1, LVNI_SELECTED);
			nSelItem != -1;
			nSelItem = c_FileList.GetNextItem(nSelItem, LVNI_SELECTED))
		{
			c_FileList.DeleteItem(nSelItem);
			nSelItem--;
		}

		// Resize the list columns.

		c_FileList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
		c_FileList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
		c_FileList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	}
	break;

	case DROPEFFECT_NONE:
	{
		// This needs special handling, because on NT, DROPEFFECT_NONE
		// is returned for move operations, instead of DROPEFFECT_MOVE.
		// See Q182219 for the details.
		// So if we're on NT, we check each selected item, and if the
		// file no longer exists, it was moved successfully and we can
		// remove it from the list.

		if (g_bNT)
		{
			bool bDeletedAnything = false;

			for (nSelItem = c_FileList.GetNextItem(-1, LVNI_SELECTED);
				nSelItem != -1;
				nSelItem = c_FileList.GetNextItem(nSelItem, LVNI_SELECTED))
			{
				CString sFilename = c_FileList.GetItemText(nSelItem, 0);

				if (0xFFFFFFFF == GetFileAttributes(sFile) &&
					GetLastError() == ERROR_FILE_NOT_FOUND)
				{
					// We couldn't read the file's attributes, and GetLastError()
					// says the file doesn't exist, so remove the corresponding 
					// item from the list.

					c_FileList.DeleteItem(nSelItem);

					nSelItem--;
					bDeletedAnything = true;
				}
			}

			// Resize the list columns if we deleted any items.

			if (bDeletedAnything)
			{
				c_FileList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
				c_FileList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
				c_FileList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);

				// Note: Don't call GlobalFree() because the data belongs to 
				// the caller.
			}
			else
			{
				// The DnD operation wasn't accepted, or was canceled, so we 
				// should call GlobalFree() to clean up.

				GlobalFree(hgDrop);
				GlobalFree(hgBool);
			}
		}   // end if (NT)
		else
		{
			// We're on 9x, and a return of DROPEFFECT_NONE always means
			// that the DnD operation was aborted.  We need to free the
			// allocated memory.

			GlobalFree(hgDrop);
			GlobalFree(hgBool);
		}
	}
	break;  // end case DROPEFFECT_NONE
	}   // end switch

}

void CMFCTest1Dlg::DragDui()
{
	if (!m_bDraging)
	{
		return;
	}
	OutInfo(_T("drop Begin"));
	
	m_idSource.SetWindow(m_hWnd);
	m_idSource.Drag(_T("e:\\work\\svn\\nc\\src\\“发帖子”韩文怎么写？_百度知道.htm"));
	m_bBtnDown = false;
	m_bDraging = false;
}

void CMFCTest1Dlg::Test()
{
	SIZE szSrc = { 50, 40 };
	SIZE szLimit = { 100, 100 };
	SIZE szResult = { 0 };
	szResult = CPiMath::GetKeepRadio(szSrc, szLimit);
	assert(szResult.cx == 100 && szResult.cy == 80);

	szLimit.cx = 30;
	szLimit.cy = 30;
	szResult = CPiMath::GetKeepRadio(szSrc, szLimit);

	szLimit.cx = 30;
	szLimit.cy = 70;
	szResult = CPiMath::GetKeepRadio(szSrc, szLimit);

	szLimit.cx = 70;
	szLimit.cy = 50;
	szResult = CPiMath::GetKeepRadio(szSrc, szLimit);

	szLimit.cx = 70;
	szLimit.cy = 40;
	szResult = CPiMath::GetKeepRadio(szSrc, szLimit);

	szLimit.cx = 50;
	szLimit.cy = 80;
	szResult = CPiMath::GetKeepRadio(szSrc, szLimit);

	/*(1.0 * szLimit.cx / szLimit.cx)
	assert(szResult.cx == 100 && szResult.cy == 100);*/
	szResult.cx = 0;
}

void CMFCTest1Dlg::DealDrag(const LST_STRING& lstPath)
{
	for (auto& strPath:lstPath)
	{
		OutInfo(strPath.c_str());
	}
}

void CMFCTest1Dlg::DealDragStatic(const LST_STRING& lstPath)
{

}
