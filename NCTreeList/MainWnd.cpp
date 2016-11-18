#include "StdAfx.h"
#include "MainWnd.h"
//////////////////////////////////////////////////////////////////////////
///
const TCHAR* const menuDelNode = _T("delNode");

DUI_BEGIN_MESSAGE_MAP(CMainWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()
CMainWnd::CMainWnd(void)
: m_pCloseBtn(nullptr)
, m_pGroupList(nullptr)
, m_pMenu(nullptr)
, m_pOrganizationList(nullptr)
, m_pStartOpt(nullptr)
, m_pStopOpt(nullptr)
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

CControlUI* CMainWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("GroupList")) == 0){
		return new CGroupsTreelistUI(m_pm);
	}
	else if (_tcsicmp(pstrClass, _T("OrganizationList")) == 0){
		return new COrganizationTreelistUI(m_pm);
	}
	return NULL;
}

DuiLib::CDuiString CMainWnd::GetSkinFile()
{
	return _T("skin.xml");
}

LPCTSTR CMainWnd::GetWindowClassName( void ) const
{
	return _T("MainWnd");
}

void CMainWnd::InitWindow()
{
	//SetTimer(GetHWND(), CLOSE_WND_TIMER,3000, NULL);
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("close")));
	m_pGroupList = static_cast<CGroupsTreelistUI*>(m_pm.FindControl(_T("groupsList")));
	m_pOrganizationList = static_cast<COrganizationTreelistUI*>(m_pm.FindControl(_T("organizationList")));
	m_pStartOpt = static_cast<COptionUI*>(m_pm.FindControl(_T("flashOn")));
	m_pStopOpt = static_cast<COptionUI*>(m_pm.FindControl(_T("flashOff")));
	LoadGroup();
	LoadOriganization();
}

void CMainWnd::OnClick( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
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
			 if (sMenuName == menuDelNode){
				 Node* node = m_pGroupList->GetItemBySId(_wtoi(sUserData));
				 m_pGroupList->RemoveNode(node);
			 }
		 }
		 if (m_pMenu != NULL) {
			 delete m_pMenu;
			 m_pMenu = NULL;
		 }
	 }
	 else if (uMsg == WM_TIMER){
		 OnTimer((UINT_PTR)wParam);
	 }
	 bHandled = FALSE;
	 return 0;
 }
void CMainWnd::Notify( TNotifyUI &msg )
{
	if (msg.sType == DUI_MSGTYPE_SCROLL){
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK){
		if (msg.pSender == m_pCloseBtn){
			this->Close();
		}
		else if (msg.pSender == m_pStartOpt){
			m_pGroupList->FlashAvatar(2);
			m_pGroupList->FlashAvatar(4);
		}
		else if (msg.pSender == m_pStopOpt){
			m_pGroupList->StopFlashAvatar(2);
			m_pGroupList->StopFlashAvatar(4);
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMCLICK){
		if (-1 != m_pGroupList->GetItemIndex(msg.pSender)){
			Node* node = (Node*)msg.pSender->GetTag();

			if (m_pGroupList->CanExpand(node))
			{
				m_pGroupList->SetChildVisible(node, !node->data().child_visible_);
			}
		}
		else if (-1 != m_pOrganizationList->GetItemIndex(msg.pSender)){
			Node* node = (Node*)msg.pSender->GetTag();

			if (m_pOrganizationList->CanExpand(node))
			{
				m_pOrganizationList->SetChildVisible(node, !node->data().child_visible_);
			}
		}
	}
	else if (msg.sType == DUI_MSGTYPE_MENU){
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
	
		Node* node = (Node*)msg.pSender->GetTag();
		//获取菜单控件
		CMenuElementUI* m_pMenuEle = static_cast<CMenuElementUI*>(m_pm.FindSubControlByName(m_pMenu->GetMenuUI(), menuDelNode));
		m_pMenuEle->SetUserData(IntToDuistring(node->data().sId));
	}
	return WindowImplBase::Notify(msg);
}

void CMainWnd::OnTimer(UINT_PTR idEvent)
{
	if (idEvent != CLOSE_WND_TIMER)
		return;
	KillTimer(GetHWND(), CLOSE_WND_TIMER);
	this->Close();
}

void CMainWnd::LoadGroup()
{
	InitListData();

	int iSize = vecListInfo.size();
	CDuiString nickName = _T("我的群组(");
	nickName += IntToDuistring(iSize);
	nickName += _T(")");
	GroupsListItemInfo ItemInfo;
	ItemInfo.id = 0;
	ItemInfo.folder = true;
	ItemInfo.nickName = nickName;
	ItemInfo.empty = false;

	Node* rootParent = nullptr;
	rootParent = m_pGroupList->AddNode(ItemInfo);


	for (auto item : vecListInfo){
		m_pGroupList->AddNode(item, rootParent);
	}

	GroupsListItemInfo ItemDiscuss;
	ItemDiscuss.id = 0;
	ItemDiscuss.folder = true;
	ItemDiscuss.nickName = _T("我的讨论组");
	ItemDiscuss.empty = false;

	Node* root = nullptr;
	root = m_pGroupList->AddNode(ItemDiscuss);
	for (auto item : vecListInfo){
		m_pGroupList->AddNode(item, root);
	}
}

void CMainWnd::LoadOriganization()
{
	InitOriganizationData();
	int iSize = vecListInfo.size();
	CDuiString nickName = _T("逆讯上海(");
	nickName += IntToDuistring(iSize);
	nickName += _T(")");
	OrganizationListItemInfo ItemInfo;
	ItemInfo.id = 0;
	ItemInfo.folder = true;
	ItemInfo.nickName = nickName;
	ItemInfo.empty = false;

	Node* rootParent = nullptr;
	rootParent = m_pOrganizationList->AddNode(ItemInfo);


	for (auto item : vecOrganizationInfo){
		Node* pNode= m_pOrganizationList->GetItemBySId(item.parentId);
		if (pNode != 0)
			rootParent = pNode;
		m_pOrganizationList->AddNode(item, rootParent);
	}

	Node* pNodeSort = m_pOrganizationList->GetItemBySId(0);
	m_pOrganizationList->SortChildren(pNodeSort);
}

//int转DUIString
CDuiString CMainWnd::IntToDuistring(const int num)
{
	CDuiString duiStr;
	wsprintf((LPWSTR)duiStr.GetData(), _T("%d"), num);
	return duiStr;
}

void CMainWnd::InitListData()
{
	GroupsListItemInfo ItemInfo;
	ItemInfo.id = 1;
	ItemInfo.avatarPath = _T("1.png");
	ItemInfo.folder = false;
	ItemInfo.nickName = _T("用户1");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = true;
	vecListInfo.push_back(ItemInfo);

	ItemInfo.id = 2;
	ItemInfo.avatarPath = _T("2.png");
	ItemInfo.folder = false;
	ItemInfo.nickName = _T("用户2");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = true;
	vecListInfo.push_back(ItemInfo);

	ItemInfo.id = 3;
	ItemInfo.avatarPath = _T("1.png");
	ItemInfo.folder = false;
	ItemInfo.nickName = _T("用户3");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = true;
	vecListInfo.push_back(ItemInfo);

	ItemInfo.id = 4;
	ItemInfo.avatarPath = _T("2.png");
	ItemInfo.folder = false;
	ItemInfo.nickName = _T("用户4");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = true;
	vecListInfo.push_back(ItemInfo);

	ItemInfo.id = 5;
	ItemInfo.avatarPath = _T("2.png");
	ItemInfo.folder = false;
	ItemInfo.nickName = _T("用户4");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = true;
	vecListInfo.push_back(ItemInfo);
}

void CMainWnd::InitOriganizationData()
{
	OrganizationListItemInfo ItemInfo;
	ItemInfo.id = 1;
	ItemInfo.avatarPath = _T("1.png");
	ItemInfo.folder = false;
	ItemInfo.nickName = _T("上海——用户1");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = true;
	ItemInfo.parentId = 0;
	vecOrganizationInfo.push_back(ItemInfo);

	ItemInfo.id = 2;
	ItemInfo.avatarPath = _T("2.png");
	ItemInfo.folder = false;
	ItemInfo.nickName = _T("上海——用户2");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = true;
	ItemInfo.parentId = 0;
	vecOrganizationInfo.push_back(ItemInfo);

	ItemInfo.id = 3;
	ItemInfo.avatarPath = _T("1.png");
	ItemInfo.folder = true;
	ItemInfo.nickName = _T("测试部");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = false;
	ItemInfo.parentId = 0;
	vecOrganizationInfo.push_back(ItemInfo);

	ItemInfo.id = 4;
	ItemInfo.avatarPath = _T("2.png");
	ItemInfo.folder = false;
	ItemInfo.nickName = _T("上海——用户4");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = true;
	ItemInfo.parentId = 3;
	vecOrganizationInfo.push_back(ItemInfo);

	ItemInfo.id = 5;
	ItemInfo.avatarPath = _T("2.png");
	ItemInfo.folder = false;
	ItemInfo.nickName = _T("上海——用户5");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = true;
	ItemInfo.parentId = 3;
	vecOrganizationInfo.push_back(ItemInfo);

	ItemInfo.id = 6;
	ItemInfo.avatarPath = _T("2.png");
	ItemInfo.folder = false;
	ItemInfo.nickName = _T("上海——用户6");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = true;
	ItemInfo.parentId = 3;
	vecOrganizationInfo.push_back(ItemInfo);

	ItemInfo.id = 7;
	ItemInfo.avatarPath = _T("2.png");
	ItemInfo.folder = false;
	ItemInfo.nickName = _T("上海——用户7");
	ItemInfo.description = _T("描述");
	ItemInfo.empty = true;
	ItemInfo.parentId = 3;
	vecOrganizationInfo.push_back(ItemInfo);
}