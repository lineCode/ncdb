/******************************************************************************* 
 *  @file      UIGroupsTreelist.cpp 2014\8\7 15:45:20 $
 *  @author    ¿ìµ¶<kuaidao@mogujie.com>
 *  @brief     
 ******************************************************************************/

#include "stdafx.h"
#include "UIOrganizationTreelist.h"
/******************************************************************************/

const int kOrganizationListFolderItemHeight = 32;
const int kOrganizationListNormalItemHeight = 60;

const TCHAR* const kLogoButtonControlName = _T("logo");
const TCHAR* const kLogoContainerControlName = _T("logo_container");
const TCHAR* const kNickNameControlName = _T("nickname");
const TCHAR* const kXMLFilePath = _T("OrganizationListItem.xml");


COrganizationTreelistUI::COrganizationTreelistUI(CPaintManagerUI& paint_manager)
:UIIMList(paint_manager)
{
}


Node* COrganizationTreelistUI::AddNode(const OrganizationListItemInfo& item, Node* parent)
{
	if (!parent)
		parent = root_node_;

	TCHAR szBuf[MAX_PATH] = {0};

    CListContainerElementUI* pListElement = NULL;
	if (!UIIMList::m_dlgBuilder.GetMarkup()->IsValid()) {
		pListElement = static_cast<CListContainerElementUI*>(UIIMList::m_dlgBuilder.Create(kXMLFilePath, (UINT)0, NULL, &paint_manager_));
    }
    else {
		pListElement = static_cast<CListContainerElementUI*>(UIIMList::m_dlgBuilder.Create((UINT)0, &paint_manager_));
    }
    if (pListElement == NULL)
        return NULL;

	Node* node = new Node;
	node->data().level_ = parent->data().level_ + 1;
	if (item.folder)
		node->data().has_child_ = !item.empty;
	else
		node->data().has_child_ = false;

	node->data().folder_ = item.folder;

	node->data().child_visible_ = (node->data().level_ == 0);
	node->data().child_visible_ = false;
	node->data().sId = item.id;
	node->data().text_ = item.nickName;
	node->data().list_elment_ = pListElement;

	if (!parent->data().child_visible_)
		pListElement->SetVisible(false);

	if (parent != UIIMList::root_node_ && !parent->data().list_elment_->IsVisible())
		pListElement->SetVisible(false);

	CDuiRect rcPadding = UIIMList::text_padding_;
	for (int i = 0; i < node->data().level_; ++i)
	{
		rcPadding.left += UIIMList::level_text_start_pos_;
	}
	pListElement->SetPadding(rcPadding);

	CControlUI* log_button = static_cast<CControlUI*>(UIIMList::paint_manager_.FindSubControlByName(pListElement, kLogoButtonControlName));
	if (log_button != NULL)
	{
		if (!item.folder && !item.avatarPath.IsEmpty())
		{
			_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), item.avatarPath);
			log_button->SetBkImage(szBuf);
		}
		else
		{
			CContainerUI* logo_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoContainerControlName));
			if (logo_container != NULL)
				logo_container->SetVisible(false);
		}
        log_button->SetTag((UINT_PTR)pListElement);
        //log_button->OnEvent += MakeDelegate(&OnButtonEvent);
	}

	CDuiString html_text;
	if (node->data().has_child_)
	{
		if (node->data().child_visible_)
			html_text += UIIMList::level_expand_image_;
		else
			html_text += UIIMList::level_collapse_image_;

		_stprintf_s(szBuf, MAX_PATH - 1, _T("<x %d>"), level_text_start_pos_);

		html_text += szBuf;
	}

	if (item.folder)
	{
		html_text += node->data().text_;
	}
	else
	{
		_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), item.nickName);

		html_text += szBuf;
	}

	CLabelUI* nick_name = static_cast<CLabelUI*>(UIIMList::paint_manager_.FindSubControlByName(pListElement, kNickNameControlName));
	if (nick_name != NULL)
	{
		if (item.folder)
			nick_name->SetFixedWidth(0);

		nick_name->SetShowHtml(true);
		nick_name->SetText(html_text);
	}

	if (item.folder)
	{
		pListElement->SetFixedHeight(kOrganizationListFolderItemHeight);
	}
	else
	{
		pListElement->SetFixedHeight(kOrganizationListNormalItemHeight);
	}
	pListElement->SetTag((UINT_PTR)node);
	int index = 0;
	if (parent->has_children())
	{
		Node* prev = parent->get_last_child();
		index = prev->data().list_elment_->GetIndex() + 1;
	}
	else 
	{
		if (parent == UIIMList::root_node_)
			index = 0;
		else
			index = parent->data().list_elment_->GetIndex() + 1;
	}
	if (!CListUI::AddAt(pListElement, index))
	{
		delete pListElement;
		delete node;
		node = NULL;
	}

	parent->add_child(node);

	return node;
}

Node* COrganizationTreelistUI::UpdateNode(const OrganizationListItemInfo& item)
{
	return nullptr;
}
BOOL COrganizationTreelistUI::UpdateItemBySId(const std::string& sId)
{
	return TRUE;
}

void COrganizationTreelistUI::SortChildren(Node *parent)
{
	if (!parent || !parent->folder()) return;
	if (!parent->has_children()) return;

	__super::SortChildren(parent,
		[=](Node *&x, Node *&y) {
		if (x->data().has_child_)
			return false;
		if (x->data().sId > y->data().sId)
			return true;
		else
			return false;
	});
}
/******************************************************************************/
