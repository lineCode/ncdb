#pragma once
#include "Poker.h"

/*------------------------------------------------------------
	file:	炸金花棋牌类
------------------------------------------------------------*/
// 牌值结构体
struct tagFGFCard{
	long	unPokerVal;		//牌值 2-14（14:A）
	long	unPokerColor;	//牌颜色 (1 黑,2红,3梅,4方,0王)
	tagFGFCard()
	{
		unPokerVal = 0;
		unPokerColor = -1;
	}
	tagFGFCard(long nVal, long nColor);
	byte ToID();
	tagFGFCard& FromID(int nID);
	string ToString();
};

typedef vector<tagFGFCard>	ARR_CARD;

//手牌结构体
struct tagHAND_CARD
{
public:

	ARR_CARD			arrCard;
	ENUM_CARD_TYPE		nCardType;		//牌型
public:
	tagHAND_CARD()
		: nCardType(ct_unknown)
	{

	}
public:
	bool operator >(tagHAND_CARD& tag);
	bool operator <(tagHAND_CARD& tag) {return !(*this > tag); }
	bool CompareSameCardType( tagHAND_CARD& hc1, tagHAND_CARD& hc2 );
	string ToStringID();
	string ToStringCard();
};
typedef vector<tagHAND_CARD>	ARR_HAND_CARD;

//炸金花类模块
class CCardGoldenFlower
{
public:
	CCardGoldenFlower(void);
	~CCardGoldenFlower(void);
public:
	bool Init();
	//获取一个手牌(3张)
	tagHAND_CARD GetHandCard();
	//识别手牌牌型
	static bool RecognizeCardType( tagHAND_CARD& hc );
	//获取N次手牌
	ARR_HAND_CARD GetHandCardArr(UINT nPlayerCount);
public:
private:
	Poker					m_pocket;

};

