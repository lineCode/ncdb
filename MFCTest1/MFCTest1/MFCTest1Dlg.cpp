
// MFCTest1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTest1.h"
#include "MFCTest1Dlg.h"
#include "afxdialogex.h"
#include "PiWindowPack.h"
#include "functional.h"
#include "UI/PiFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	m_bDraging = false;
}

void CMFCTest1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
}

BEGIN_MESSAGE_MAP(CMFCTest1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CMFCTest1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTest1Dlg::OnBnClickedButton1)
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

	const wchar_t pszFilter[] = _T("EXE File (*.txt)|*.txt|All Files (*.*)|*.*||");
	/*CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
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
	tstring strFilter = _T("All Files (*.*)|*.*|txt Files (*.txt)|*.txt|bin Files (*.exe)|*.exe;*.obj||");

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
	pEdit->SetWindowText(strOut.c_str());
	return;
}

void CMFCTest1Dlg::TestOleDrag()
{	
	
}


void CMFCTest1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	BeginDrag();

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCTest1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCTest1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	DragIng();
	CDialogEx::OnMouseMove(nFlags, point);
}

bool CMFCTest1Dlg::BeginDrag()
{
	if (m_bDraging)
	{
		return true;
	}
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
