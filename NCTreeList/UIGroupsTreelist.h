/*******************************************************************************
*  @file      UIGroupsTreelist.h 2016-11-18 15:18:16
*  @author    崔光辉<cuiguanghui@naycent.com>
*  @brief
******************************************************************************/
#ifndef UIGROUPSTREELIST_A6A09709_9687_4B3D_8924_25CA7CBC4A8F_H__
#define UIGROUPSTREELIST_A6A09709_9687_4B3D_8924_25CA7CBC4A8F_H__

#include "UIIMList.h"

const CDuiString MY_DISCUSSGROUP_ITEMID = _T("myDiscussGroup_id");
const CDuiString MY_GROUP_ITEMID = _T("mygroup_id");
#define FLASH_AVATAR_TIMER       UIMSG_USER+1

class GroupsListItemInfo final :public IMListItemInfo
{

};

/**
* The class <code>群、讨论组会话树形控件</code>
*
*/
class CGroupsTreelistUI final: public UIIMList
{
public:
	
	CGroupsTreelistUI(CPaintManagerUI& paint_manager);

	Node* AddNode(const GroupsListItemInfo& item, Node* parent = NULL);//增加群
	Node* UpdateNode(const GroupsListItemInfo& groupInfo);//更新群信息
	BOOL UpdateItemBySId(const std::string& sId);

	void FlashAvatar(IN const int sId);
	void StopFlashAvatar(IN const int sId);
};
/******************************************************************************/
#endif// UIGROUPSTREELIST_a6a09709-9687-4b3d-8924-25ca7cbc4a8f_H__