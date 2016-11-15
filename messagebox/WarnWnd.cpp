#include "stdafx.h"
#include "WarnWnd.h"
#include "NCMessageBox.h"

vector<HWND> vecWnd;
CDuiString strTitle;
HWND pForeground = NULL;


#define NUM_TIMER_ENABLE_SELF		200

CWarnWnd::CWarnWnd(void)
	: m_pCloseBtn(nullptr)
	, m_pBtn1(nullptr)
	, m_pBtn2(nullptr)
	, m_pBtn3(nullptr)
	, m_pTitle(nullptr)
	, wndHeight(0)
	, oneStepLength(30)
	, wndDistance(4)
	, overStepWnd(nullptr)
	, closeIndex(0)
{
}

CWarnWnd::~CWarnWnd(void)
{
}

void CWarnWnd::OnFinalMessage(HWND hWnd)
{
	for (vector<HWND>::iterator it = vecWnd.begin(); it != vecWnd.end(); it++)
	{
		if (*it == hWnd){
			vecWnd.erase(it);
			break;
		}
	}
	__super::OnFinalMessage(hWnd);
}

void CWarnWnd::LvWarnWnd(LPCTSTR pTitle)
{
	pForeground = NULL;
	pForeground = GetForegroundWindow();
	strTitle = pTitle;
	CWarnWnd *s_pInstance = new CWarnWnd();
	s_pInstance->Create(NULL, _T(""), UI_WNDSTYLE_FRAME &~WS_VISIBLE | WS_DISABLED, WS_EX_TOOLWINDOW, 0, 0, 0, 0);

	//HWND pWnd = s_pInstance->Create(NULL, _T(""), WS_CAPTION | WS_POPUPWINDOW | WS_SYSMENU, WS_EX_NOACTIVATE, 0, 0, 0, 0);
	//::ShowWindow(pWnd, SW_SHOWNOACTIVATE);
	//s_pInstance->ShowWindow(true, false);
	//::ShowWindow(s_pInstance->GetHWND(), SW_SHOWNOACTIVATE);
	vecWnd.push_back(s_pInstance->GetHWND()); 	
}

//DuiLib::CDuiString CWarnWnd::GetSkinFile()
//{
//	return _T("WarnWnd.xml");
//}
//
//LPCTSTR CWarnWnd::GetWindowClassName(void) const
//{
//	return _T("MessageBoxWnd");
//}
void CWarnWnd::SetWndAttribute()
{
	SetSkinFile(_T("WarnWnd.xml"));
	ResizeClient(WND_WIDTH, WND_HEIGHT);

	//RECT rc = { 0, 0, 0, -1 };
	//m_pm.SetCaptionRect(rc);
	RECT rcSizeBox = { 0, 0, 0, 0 };
	m_pm.SetSizeBox(rcSizeBox);
	m_pm.SetMaxInfo(WND_WIDTH, WND_HEIGHT);
}

void CWarnWnd::InitWindow()
{		
	SetWndAttribute();
	//设置窗口靠近屏幕边缘不最大化
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
	
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("close")));
	m_pBtn1 = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn1")));
	m_pBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn2")));
	m_pBtn3 = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn3")));
	m_pTitle = static_cast<CLabelUI*>(m_pm.FindControl(_T("title")));

	m_pTitle->SetText(strTitle);

	//窗口位置
	RECT rc;   
	GetWindowRect(this->GetHWND(), &rc);

	//获取工作区域
	RECT rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);

	int workAreaHeight = rt.bottom - rt.top;
	int workAreaWidth = rt.right - rt.left;
	int column = workAreaWidth / (WND_WIDTH + GAP_WIDTH);
	int row = workAreaHeight / (WND_HEIGHT + GAP_HEIGHT);
	int numWnd = vecWnd.size();
	if (row == 0 || column == 0)
		return;
	int columnWnd = numWnd/row;
	int rowWnd = numWnd%row;

	SetWindowPos(GetHWND(), HWND_TOPMOST, workAreaWidth - (columnWnd + 1)*(WND_WIDTH + GAP_WIDTH), workAreaHeight - (rowWnd + 1)*(WND_HEIGHT + GAP_HEIGHT), WND_WIDTH, WND_HEIGHT, SWP_NOACTIVATE);

	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	SetTimer(m_hWnd, NUM_TIMER_ENABLE_SELF, 100, NULL);
}

void CWarnWnd::Notify(TNotifyUI &msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK){
		if (msg.pSender == m_pCloseBtn){
			this->ShowWindow(false);
			SetTimer(GetHWND(), MOVEWND_TIMERID, 100, NULL);
			RECT rcWnd;
			GetWindowRect(GetHWND(), &rcWnd);
			wndHeight = rcWnd.bottom - rcWnd.top + wndDistance;

			//获取关闭窗口索引
			int index = 0;
			for (auto hWnd : vecWnd){
				if (hWnd == GetHWND())
					break;
				index++;
			}
			closeIndex = index;

			//获取工作区域
			RECT rt;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
			int row = (rt.bottom - rt.top) / (WND_HEIGHT + GAP_HEIGHT);
			int iSize = vecWnd.size();
			//获取点击列最上面一个窗口序号，因为动画显示的时候只有点击位置到顶部部分动画
			if ((closeIndex + 1) % row == 0)                   
				topIndex = ((closeIndex + 1) / row)*row;
			else
			{
				topIndex = ((closeIndex + 1) / row + 1)*row;
				if (topIndex > iSize)
					topIndex = iSize;
			}
			overStepWnd = vecWnd[topIndex - 1];
		}
			
	}
	return WindowImplBase::Notify(msg);
}

LRESULT CWarnWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MOUSEMOVE){

	}
	else if (uMsg == WM_TIMER){
		OnTimer((UINT_PTR)wParam);
		OnMoveWndTimer((UINT_PTR)wParam);
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CWarnWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_NCLBUTTONDBLCLK != uMsg)
		return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	else
		return 0;
}

void CWarnWnd::OnTimer(UINT_PTR idEvent)
{
	if (idEvent != NUM_TIMER_ENABLE_SELF)
	{
		return;
	}
	KillTimer(m_hWnd, idEvent);
	EnableWindow(m_hWnd, TRUE);
	return;
}

void CWarnWnd::OnMoveWndTimer(UINT_PTR idEvent)
{
	if (idEvent != MOVEWND_TIMERID)
		return;
	KillTimer(GetHWND(), MOVEWND_TIMERID);

	if (wndHeight - oneStepLength >= 0)
		wndHeight -= oneStepLength;
	else{
		oneStepLength = wndHeight;
		wndHeight = 0;
	}	

	for (int i = closeIndex; i < topIndex; i++){
		RECT rcWnd;
		GetWindowRect(vecWnd[i], &rcWnd);
		MoveWindow(vecWnd[i], rcWnd.left, rcWnd.top + oneStepLength, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top, true);
	}
	
	if (wndHeight!=0)
		SetTimer(GetHWND(), MOVEWND_TIMERID, 10, NULL);
	else{
		if (overStepWnd)
			SetWndPos();   //动画结束后移动第一列后面的窗口
		Close(MSGID_CANCEL);
	}
		
}

void CWarnWnd::SetWndPos()
{
	int index = 0;
	for (auto hWnd : vecWnd){
		if (hWnd == overStepWnd)
			break;
		index++;
	}

	//获取工作区域
	RECT rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);

	int workAreaHeight = rt.bottom - rt.top;
	int workAreaWidth = rt.right - rt.left;
	int column = workAreaWidth / (WND_WIDTH + GAP_WIDTH);
	int row = workAreaHeight / (WND_HEIGHT + GAP_HEIGHT);
	int numWnd = vecWnd.size();
	if (row == 0 || column == 0)
		return;
	for (int i = index; i < numWnd; i++){
		int columnWnd = i / row;
		int rowWnd = i%row;
		if (i + 1 == numWnd)
			return;
		SetWindowPos(vecWnd[i+1], HWND_TOPMOST, workAreaWidth - (columnWnd + 1)*(WND_WIDTH + GAP_WIDTH), workAreaHeight - (rowWnd + 1)*(WND_HEIGHT + GAP_HEIGHT), WND_WIDTH, WND_HEIGHT, SWP_NOACTIVATE);
	}
}

LRESULT CWarnWnd::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 1;
}
