
// BinVersionerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BinVersioner.h"
#include "BinVersionerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBinVersionerDlg 对话框




CBinVersionerDlg::CBinVersionerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBinVersionerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBinVersionerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBinVersionerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CBinVersionerDlg 消息处理程序


bool SetPEResource( LPCTSTR exepath, LPCTSTR type, LPCTSTR name, tagMemBlock& mb, int language /*= 0*/ )
{
	VS_FIXEDFILEINFO* pffi = NULL;
	//pffi = 设置其实位置
	HANDLE hexe = BeginUpdateResource(exepath, FALSE);

	if (!hexe)
		return false;

	BOOL r = UpdateResource(hexe, MAKEINTRESOURCE(RT_VERSION),  MAKEINTRESOURCE(VS_VERSION_INFO), language,
		(LPVOID)mb.pMem, (DWORD)mb.dwSize);

	BOOL er = EndUpdateResource(hexe, FALSE);

	return (r && er);
}

BOOL CBinVersionerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	wstring strVersion(_T("1.2.3.50"));
	tagMemBlock mb = GetFileVersionData();
	SetPEResource(_T("e:\\person\\ncdb\\project\\BinVersioner\\Debug\\test.exe"), RT_VERSION
		, MAKEINTRESOURCE(VS_VERSION_INFO), mb, 0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CBinVersionerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBinVersionerDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBinVersionerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

tagMemBlock CBinVersionerDlg::GetFileVersionData()
{
	tagMemBlock mb = {0};
	HGLOBAL hResLoad;     // handle to loaded resource 
	HMODULE hExe;        // handle to existing .EXE file 
	HRSRC hRes;         // handle/ptr. to res. info. in hExe 
	HANDLE hUpdateRes;  // update resource handle 
	void *lpResLock;    // pointer to resource data 
	BOOL result; 
	hExe = LoadLibrary(_T("e:\\person\\ncdb\\project\\BinVersioner\\Debug\\test.exe")); 
	if (!hExe) 
	{ 
		return mb;
	} 

	hRes = FindResource(hExe, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION); 
	if (!hRes) 
	{ 
		return mb;
	} 

	// Load the dialog box into global memory. 
	hResLoad = LoadResource(hExe, hRes); 
	if (!hResLoad) 
	{ 
		return mb;
	} 

	// Lock the dialog box into global memory. 
	lpResLock = LockResource(hResLoad); 
	mb.pMem = lpResLock;
	mb.dwSize = *(WORD*)lpResLock;
	return mb;
	//VS_VERSIONINFO* 

}
