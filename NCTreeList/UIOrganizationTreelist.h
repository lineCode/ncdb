#ifndef UIORGANIZATIONTREELIST_A6A09709_9687_4B3D_8924_25CA7CBC4A8F_H__
#define UIORGANIZATIONTREELIST_A6A09709_9687_4B3D_8924_25CA7CBC4A8F_H__

#include "UIIMList.h"

class OrganizationListItemInfo final :public IMListItemInfo
{
public:
	int parentId;        //父id
};

/**
* The class <code>群、讨论组会话树形控件</code>
*
*/
class COrganizationTreelistUI final : public UIIMList
{
public:
	
	COrganizationTreelistUI(CPaintManagerUI& paint_manager);

	Node* AddNode(const OrganizationListItemInfo& item, Node* parent = NULL);//增加群
	Node* UpdateNode(const OrganizationListItemInfo& OrganizationInfo);//更新群信息
	BOOL UpdateItemBySId(const std::string& sId);
	void SortChildren(Node *parent);
};
/******************************************************************************/
#endif// UIORGANIZATIONTREELIST_A6A09709_9687_4B3D_8924_25CA7CBC4A8F_H__