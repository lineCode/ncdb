
// UseNCCefDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UseNCCef.h"
#include "UseNCCefDlg.h"
#include "afxdialogex.h"
#include "NCCef.h"
#include "functional.h"
#pragma comment(lib, "libcef.lib")
#pragma comment(lib, "libcef_dll_wrapper.lib")
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


// CUseNCCefDlg 对话框



CUseNCCefDlg::CUseNCCefDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUseNCCefDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUseNCCefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUseNCCefDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &CUseNCCefDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CUseNCCefDlg 消息处理程序

BOOL CUseNCCefDlg::OnInitDialog()
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
	TestCef();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUseNCCefDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUseNCCefDlg::OnPaint()
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
HCURSOR CUseNCCefDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUseNCCefDlg::TestCef()
{
	m_pCef = theApp.m_pCef;
	
	int nRet = 0;
	m_pCef->Parent(m_hWnd);
	m_pCef->Init(true);
	m_pCef->Create(m_hWnd);
	m_pCef->Start();
	
	//m_pCef->Load(_T("sdf"));
}



int CUseNCCefDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	/*CNCCef* pCef = theApp.m_pCef;

	int nRet = 0;
	pCef->Parent(m_hWnd);
	pCef->Init(true);
	pCef->Create(m_hWnd);*/

	return 0;
}


void CUseNCCefDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	OutInfo(_T("OnClose"));
	if (m_pCef && !m_pCef->IsClosing())
	{
		OutInfo(_T("m_pCef->FreeBrowser"));
		m_pCef->FreeBrowser();
		m_pCef->UnInit();
	}
	else
	{
		CDialogEx::OnClose();
	}
}


void CUseNCCefDlg::OnDestroy()
{
	OutInfo(_T("OnDestroy"));
	//CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
}


void CUseNCCefDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	OutInfo(_T("keyDown"));
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CUseNCCefDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN
		|| pMsg->message == WM_KEYDOWN)
	{
		
		OutInfo(_T("main WM_KEYDOWN, WM_KEYUP"));
		int nKey = pMsg->wParam;
		if (nKey == VK_RETURN)
		{

			OutInfo(_T("回车键"));

			CString strEdit;
			GetDlgItemText(IDC_EDIT1, strEdit);
			m_pCef->Load(strEdit);
			return FALSE;
		}


	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CUseNCCefDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	m_pCef->Test();
}
