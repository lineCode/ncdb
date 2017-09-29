#pragma once
#include "Poker.h"

/*------------------------------------------------------------
	file:	ը��������
------------------------------------------------------------*/
// ��ֵ�ṹ��
struct tagFGFCard{
	long	unPokerVal;		//��ֵ 2-14��14:A��
	long	unPokerColor;	//����ɫ (1 ��,2��,3÷,4��,0��)
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

//���ƽṹ��
struct tagHAND_CARD
{
public:

	ARR_CARD			arrCard;
	ENUM_CARD_TYPE		nCardType;		//����
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

//ը����ģ��
class CCardGoldenFlower
{
public:
	CCardGoldenFlower(void);
	~CCardGoldenFlower(void);
public:
	bool Init();
	//��ȡһ������(3��)
	tagHAND_CARD GetHandCard();
	//ʶ����������
	static bool RecognizeCardType( tagHAND_CARD& hc );
	//��ȡN������
	ARR_HAND_CARD GetHandCardArr(UINT nPlayerCount);
public:
private:
	Poker					m_pocket;

};

