//扑克牌类库

#pragma once
#include "stdafx.h"
#include <vector>
#include <algorithm>

using namespace std;

//每张牌的数据结构
typedef struct _PokerCard{
	long	unPokerVal;		//牌值 1-13（K）14,15表示小大王		
	long	unPokerColor;	//牌颜色 (1 黑,2红,3梅,4方,0王)
	_PokerCard()
	{
		unPokerVal = 0;
		unPokerColor = -1;
	}
	byte ToID();
	string ToString()
	{
		char szBuf[10] = {0};
		char* szColor[] = {"黑桃", "红心", "梅花", "方块"};
		sprintf_s(szBuf, size_t(szBuf), "%s%d", szColor[unPokerColor], unPokerVal);
		return "";
	}
}PokerCard,*lpPokerCard;

class Poker
{
public:
	Poker();
	~Poker();
	/******************************************
	**函数名：InitPokerSet
	**功能：使用前初始化牌池
	**参数1：unSet 几副牌 （1或者2副牌）
	**参数2：bJack 是否有大小王 (TRUE有/FALSE无) 
	*******************************************/
	void InitPokerSet(UINT unSet,BOOL bJack);	

	/******************************************
	**函数名：InitUsedPokerSet
	**功能：初始化将使用的发牌池
	*******************************************/
	long InitUsedPokerSet();

	/******************************************
	**函数名：UnInitUsedPokerSet
	**功能：清除发牌池
	*******************************************/
	void UnInitUsedPokerSet();

	/******************************************
	**函数名：GetPokers
	**功能：获取几张牌
	**参数1：unSize 牌数
	**参数2：出参 数组 pPoker = new OnePoker[unSize];
	**返回值：实际得到的牌数
	*******************************************/
	UINT GetPokers(UINT unSize, lpPokerCard pPoker);
	UINT GetPokersTimes(UINT unSize, lpPokerCard pPoker);

	/******************************************
	**函数名：UnInit
	**功能：清除所有数据
	*******************************************/
	void UnInit();
	bool ReInit();
	UINT GetRemainCount();

	UINT				m_nPokerSet;	//几副牌 1或者2副牌
	BOOL				m_bJack;		//是否有大小王(TRUE有/FALSE无) 
	UINT				m_unPokerCount;	//牌的总张数
	UINT				m_nCardEndIndex;	//当前剩余牌的结束位置

	vector<PokerCard>	m_vctPokerPool_1;	//牌池1（1副牌）(1-13黑)(14-26红)（27-39梅）(40-52方)（53、54大小王）
	vector<PokerCard>	m_vctPokerPool_2;	//牌池2（1副牌）(1-13黑)(14-26红)（27-39梅）(40-52方)（53、54大小王）

	vector<PokerCard>    m_vctUsePokers;	//每次发牌前,初始化将被使用的牌池
};
