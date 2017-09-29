#include "stdafx.h"
#include "DBBase.h"
#include "FGF\DataSql.h"
#include "..\..\Common\public\mysql\DBEngine.h"
#include "..\..\Common\public\CLASS\sqlfunc.h"

CDBBase::CDBBase(void)
{
}


CDBBase::~CDBBase(void)
{
}

//	获取玩家信息
STUserInfo CDBBase::GetUserInfo(UINT uUserId)
{
	char szSql[SQL_LEN];
	CDBRowset Rowset;
	STUserInfo stUserInfo;
	CDBMySqlPtr pMysql = CDBEngine::getInstance()->GetIdleConnect();
	sprintf_s(szSql, SQL_LEN, SQL_USER_INOF_INSERT, uUserId);
	BOOL bRet = CDBEngine::getInstance()->ExecSql(pMysql, szSql, &Rowset);
	if (bRet)
	{
		if(!Rowset.IsEOF())
		{
			stUserInfo.uUserID = uUserId;
			strcpy_s(stUserInfo.szNickName,Rowset.GetStringField("nick_name"));
			stUserInfo.bySex = Rowset.GetUintField("user_sex");
			stUserInfo.uUserIcon = Rowset.GetUintField("user_icon");
			stUserInfo.uExperience = Rowset.GetUintField("user_exp");
			stUserInfo.uGrade = Rowset.GetUintField("user_lv");
			stUserInfo.uVipExperience = Rowset.GetUintField("vip_exp");
			stUserInfo.uVipGrade = Rowset.GetUintField("vip_lv");
			stUserInfo.uMoney = tagMoney(Rowset.GetUintField("gold_num"), Rowset.GetUintField("gold_num_bilion"));

			stUserInfo.uBankMoney = tagMoney(Rowset.GetUintField("gold_bank_num"), Rowset.GetUintField("gold_bank_num_bilion"));
			strcpy_s(stUserInfo.szQQ,Rowset.GetStringField("qq_num"));
			stUserInfo.byBirthday = Rowset.GetUintField("birthday");
			stUserInfo.byUserJob = Rowset.GetUintField("user_job");
			stUserInfo.byUserJobOwne = Rowset.GetUintField("user_job_owne");
			stUserInfo.byUserCulture = Rowset.GetUintField("user_culture");
			stUserInfo.byUserPlace = Rowset.GetUintField("user_place");
			stUserInfo.bIsAI = Rowset.GetUintField("ai_user");
			stUserInfo.byUserType = Rowset.GetUintField("user_type");
			stUserInfo.byLoginTimes = Rowset.GetUintField("is_first_login");
			strcpy_s(stUserInfo.strURLIcon,Rowset.GetStringField("icon_url"));
			strcpy_s(stUserInfo.szAccount,Rowset.GetStringField("account"));
		}
	}
	return stUserInfo;
}


//	判断验证码是否正确
BOOL CDBBase::JudgeCheckCode( STJoinGame& stJoinGame )
{
	char szSql[SQL_LEN];
	CDBRowset Rowset;
	time_t t;
	time(&t);
	CDBMySqlPtr pMysql1 = CDBEngine::getInstance()->GetIdleConnect();
	sprintf_s(szSql, SQL_USER_CHECK_CODE_JUDGE, stJoinGame.uUserId, stJoinGame.szCheckCode);
	BOOL bRet = CDBEngine::getInstance()->ExecSql(pMysql1,szSql,&Rowset);
	if (bRet)
	{
		if(!Rowset.IsEOF())
		{
			UINT uNum = Rowset.GetUintField("num");
			if (uNum>0)
			{
				return TRUE;
			}
			else if (uNum==0)
			{
				UINT uCodeTime, uNowTime;
				char szCheckCode[128];
				uNowTime = static_cast<UINT>(time(0));
				strcpy_s(szCheckCode, Rowset.GetStringField("check_code"));
				uCodeTime = Rowset.GetUintField("add_date");
				if (0==strcmp(szCheckCode, stJoinGame.szCheckCode) && uCodeTime+MAX_ONE_DAY_SECOND>= uNowTime)
				{
					return TRUE;
				}
			}
			return FALSE;
		}
	}
	LOGERROR("CDBBase::JudgeCheckCode() DB error");
	return FALSE;
}

//		更新玩家在线信息(玩家ID，游戏ID，在线状态，初始金币，当前金币)
BOOL	CDBBase::UpdateUserOnlineInfo(UINT uUserId,UINT uGameId,UINT uIsGame,tagMoney uInitGold,tagMoney uCurrentGold)
{
	UINT	uTime = static_cast<UINT>(time(0));
	char szSql[SQL_LEN];
	CDBRowset Rowset;
	CDBMySqlPtr pMysql = CDBEngine::getInstance()->GetIdleConnect();
	sprintf_s(szSql, SQL_LEN, SQL_UPDATE_ONLINE_INFO, uUserId, uGameId, uTime, uIsGame, uInitGold.GetBilion(), uInitGold.GetBase()
		, uCurrentGold.GetBilion(), uCurrentGold.GetBase());
	BOOL bRet = CDBEngine::getInstance()->ExecSql(pMysql, szSql, &Rowset);
	if (bRet)
	{
		return TRUE;
	}
	LOGERROR("CDBBase::UpdateUserOlineInfo()");
	return FALSE;
}

//		更新玩家当前金币(玩家ID，当前金币)
BOOL	CDBBase::UpdateUserCurGold(UINT uUserId,tagMoney uCurrentGold)
{
	char szSql[SQL_LEN];
	CDBRowset Rowset;
	CDBMySqlPtr pMysql = CDBEngine::getInstance()->GetIdleConnect();
	sprintf_s(szSql, SQL_LEN, SQL_UPDATE_CUR_GOLD,uUserId,uCurrentGold.GetBilion(), uCurrentGold.GetBase());
	BOOL bRet = CDBEngine::getInstance()->ExecSql(pMysql, szSql, &Rowset);
	if (bRet)
	{
		return TRUE;
	}
	LOGERROR("CDBBase::UpdateUserCurGOld()");
	return FALSE;
}

//		玩家退出游戏更新状态(玩家ID，状态 0:正常退出  1:心跳退出)
BOOL	CDBBase::UserExitUpdateType(UINT uUserId,UINT uType)
{
	UINT	uTime = static_cast<UINT>(time(0));
	char szSql[SQL_LEN];
	CDBRowset Rowset;
	CDBMySqlPtr pMysql = CDBEngine::getInstance()->GetIdleConnect();
	sprintf_s(szSql, SQL_LEN, SQL_EXIT_GAME_UPDATE_TYPE,uUserId,uTime,uType);
	BOOL bRet = CDBEngine::getInstance()->ExecSql(pMysql, szSql, &Rowset);
	if (bRet)
	{
		return TRUE;
	}
	LOGERROR("CDBBase::UserExitUpdateType()");
	return FALSE;
}
