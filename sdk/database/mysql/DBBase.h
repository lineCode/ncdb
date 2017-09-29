#pragma once
#include "Info\ServerCommonInfo.h"
#include "Info\protocolDef.h"
class CDBBase
{
public:
	CDBBase(void);
	~CDBBase(void);
public:
	//		获取玩家信息(玩家id)
	STUserInfo	GetUserInfo(UINT uUserId);

	//		判断验证码是否正确
	BOOL JudgeCheckCode(STJoinGame& stJoinGame);

	//		更新玩家在线信息(玩家ID，游戏ID，在线状态，初始金币，当前金币)
	BOOL	UpdateUserOnlineInfo(UINT uUserId,UINT uGameId,UINT uIsGame,tagMoney uInitGold,tagMoney uCurrentGold);

	//		更新玩家当前金币(玩家ID，当前金币)
	BOOL	UpdateUserCurGold(UINT uUserId,tagMoney uCurrentGold);

	//		玩家退出游戏更新状态(玩家ID，状态 0:正常退出  1:心跳退出)
	BOOL	UserExitUpdateType(UINT uUserId,UINT uType);
};

