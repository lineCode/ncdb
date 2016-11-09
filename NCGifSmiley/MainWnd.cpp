#include "StdAfx.h"
#include "MainWnd.h"
#include "UIGifContainer.h"
//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CMainWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()
CMainWnd::CMainWnd(void)
:m_pMinBtn(nullptr)
, m_pMaxBtn(nullptr)
, m_pRestoreBtn(nullptr)
, m_pCloseBtn(nullptr)
, m_pGifLayout(nullptr)
, m_pSelfLayout(nullptr)
, m_pList(nullptr)
{
}

CMainWnd::~CMainWnd(void)
{
}

void CMainWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CMainWnd::GetSkinFile()
{
	return _T("skin.xml");
}

CControlUI* CMainWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("GifAnimEx")) == 0)
	{
		return new CGifAnimUIEx();
	}
	else if (_tcsicmp(pstrClass, _T("GifContainer")) == 0)
	{
		return new CGifContainerUI();
	}
	return NULL;
}

LPCTSTR CMainWnd::GetWindowClassName( void ) const
{
	return _T("MainWnd");
}

void CMainWnd::InitWindow()
{
	m_pGifLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("gifLayout")));
	m_pSelfLayout = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("selfLayout")));
	LoadGif();
	LoadSelfGif();
}

void CMainWnd::LoadGif()
{
	WCHAR buf[16];
	for (int i = 0; i < 9; i++){
		CHorizontalLayoutUI* pHorLayout = new CHorizontalLayoutUI();
		pHorLayout->SetFixedHeight(64);
		pHorLayout->SetPadding({0,-1,0,0});
		pHorLayout->SetBorderColor(0xFFCCCCCC);
		pHorLayout->SetLeftBorderSize(0);
		for (int j = 1; j < 8; j++){
			CDialogBuilder builder;
			CVerticalLayoutUI* m_pContainer = (CVerticalLayoutUI*)builder.Create(_T("gifbutton.xml"), NULL, this, &m_pm, NULL);
			CGifAnimUIEx* m_pGif = static_cast<CGifAnimUIEx*>(m_pm.FindSubControlByName(m_pContainer, _T("gifBtn")));
			m_pGif->SetToolTip(_T("啦啦啦啦啦啦啦"));

			wsprintfW(buf, L"%d", i*7+j);
			CDuiString filePath = _T("lvface\\");
			filePath += buf;
			filePath += _T(".gif");
			if (i * 7 + j == 58)
				filePath = _T("");
			m_pGif->SetBkImage(filePath);
			pHorLayout->Add(m_pContainer);
		}
		m_pGifLayout->Add(pHorLayout);
	}
}

void CMainWnd::OnClick( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

	if( msg.pSender == m_pCloseBtn ) {
		PostQuitMessage(0);
		return; 
	}
}

LRESULT CMainWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
 {
	 if (uMsg == WM_MENUCLICK)
	 {
		 MenuCmd* pMenuCmd = (MenuCmd*)wParam;
		 if (pMenuCmd != NULL)
		 {
			 CDuiString sMenuName = pMenuCmd->szName;
			 CDuiString sUserData = pMenuCmd->szUserData;
			 m_pm.DeletePtr(pMenuCmd);
			 if (sMenuName == _T("delGif")){
				 for (auto gif : vecGif){
					 CDuiString path = gif->GetBkImage();
					 if (path == sUserData)
						 gif->SetBkImage(_T(""));
				 }
			 }
		 }
		 if (m_pMenu != NULL) {
			 delete m_pMenu;
			 m_pMenu = NULL;
		 }
	 }
	 bHandled = FALSE;
	 return 0;
 }
void CMainWnd::Notify( TNotifyUI &msg )
{
	if (msg.sType == DUI_MSGTYPE_SCROLL){
	}
	else if (msg.sType == DUI_MSGTYPE_GIF_CLICK){
		CDuiString path = static_cast<CGifAnimUIEx*>(msg.pSender)->GetBkImage();
		this->Close();
	}
	else if (msg.sType == DUI_MSGTYPE_GIF_MENU){
		//InitMenu();
		if (m_pMenu != NULL) {
			delete m_pMenu;
			m_pMenu = NULL;
		}
		CDuiPoint point;
		::GetCursorPos(&point);
		m_pMenu = new CMenuWnd();
		m_pMenu->Init(NULL, _T("menu.xml"), point, &m_pm);
		// 动态添加后重新设置菜单的大小
		m_pMenu->ResizeMenu();

		//获取菜单控件
		CMenuElementUI* m_pMenuEle = static_cast<CMenuElementUI*>(m_pm.FindSubControlByName(m_pMenu->GetMenuUI(), _T("delGif")));
		m_pMenuEle->SetUserData(static_cast<CGifAnimUIEx*>(msg.pSender)->GetBkImage());
	}
	return WindowImplBase::Notify(msg);
}

void CMainWnd::LoadSelfGif()
{
	WCHAR buf[16];
	for (int i = 0; i < 4; i++){
		CHorizontalLayoutUI* pHorLayout = new CHorizontalLayoutUI();
		pHorLayout->SetFixedHeight(64);
		pHorLayout->SetPadding({ 0, -1, 0, 0 });
		pHorLayout->SetBorderColor(0xFFCCCCCC);
		pHorLayout->SetLeftBorderSize(0);
		for (int j = 1; j < 8; j++){
			CDialogBuilder builder;
			CVerticalLayoutUI* m_pContainer = (CVerticalLayoutUI*)builder.Create(_T("gifbutton.xml"), NULL, this, &m_pm, NULL);
			CGifAnimUIEx* m_pGif = static_cast<CGifAnimUIEx*>(m_pm.FindSubControlByName(m_pContainer, _T("gifBtn")));
			m_pGif->SetToolTip(_T("啦啦啦啦啦啦啦"));
			m_pGif->SetTag(1);

			wsprintfW(buf, L"%d", i * 7 + j);
			CDuiString filePath = _T("insert\\");
			filePath += buf;
			filePath += _T(".gif");
			m_pGif->SetBkImage(filePath);
			vecGif.push_back(m_pGif);
			pHorLayout->Add(m_pContainer);
		}
		m_pSelfLayout->Add(pHorLayout);
	}
}