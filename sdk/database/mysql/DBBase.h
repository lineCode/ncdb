#pragma once
#include "Info\ServerCommonInfo.h"
#include "Info\protocolDef.h"
class CDBBase
{
public:
	CDBBase(void);
	~CDBBase(void);
public:
	//		��ȡ�����Ϣ(���id)
	STUserInfo	GetUserInfo(UINT uUserId);

	//		�ж���֤���Ƿ���ȷ
	BOOL JudgeCheckCode(STJoinGame& stJoinGame);

	//		�������������Ϣ(���ID����ϷID������״̬����ʼ��ң���ǰ���)
	BOOL	UpdateUserOnlineInfo(UINT uUserId,UINT uGameId,UINT uIsGame,tagMoney uInitGold,tagMoney uCurrentGold);

	//		������ҵ�ǰ���(���ID����ǰ���)
	BOOL	UpdateUserCurGold(UINT uUserId,tagMoney uCurrentGold);

	//		����˳���Ϸ����״̬(���ID��״̬ 0:�����˳�  1:�����˳�)
	BOOL	UserExitUpdateType(UINT uUserId,UINT uType);
};

