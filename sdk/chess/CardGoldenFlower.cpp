#include "stdafx.h"
#include "CardGoldenFlower.h"
#define CHANGE_CARD_VALUE(n) (n == 1 ? 14 : n)
#define RECOVER_CARD_VALUE(n) (n == 14 ? 1 : n)
#define NUM_CARD_COUNT (3)

bool ComparePointAsce (tagFGFCard& c1, tagFGFCard& c2) 
{ 
	return (c1.unPokerVal < c2.unPokerVal); 
}

bool CompareColorAsce (tagFGFCard& c1, tagFGFCard& c2) 
{ 
	return (c1.unPokerColor < c2.unPokerColor); 
}
tagFGFCard::tagFGFCard( long nVal, long nColor )
{
	unPokerVal = CHANGE_CARD_VALUE(nVal);
	unPokerColor = nColor;
}

byte tagFGFCard::ToID()
{
	//��ת�ɶ�Ӧ������id
	/************************************************************************
		����ɫ, ������
	//�� ( ��1-13, �� 14-26,÷ 27-39, ��40--52)
	************************************************************************/
	byte nID = 0;
	long nVal = unPokerVal;
	if (nVal > 14)
	{
		//��ֵ����
		return nID;
	}
	nVal = RECOVER_CARD_VALUE(nVal);
	nID = (byte)nVal + (13 * ((byte)this->unPokerColor - 1));
	return nID;
}

tagFGFCard& tagFGFCard::FromID( int nID )
{
	//��IDת�ɽṹ
	/************************************************************************
		����ɫ, ������
	//�� ( ��1-13, �� 14-26,÷ 27-39, ��40--52)
	************************************************************************/
	unPokerColor = (nID - 1) / 13 + 1;
	unPokerVal = CHANGE_CARD_VALUE(nID - (unPokerColor - 1) * 13);
	return *this;
}

std::string tagFGFCard::ToString()
{
	char szBuf[10] = {0};
	char* szColor[] = {"����", "����", "÷��", "����"};
	sprintf_s(szBuf, sizeof(szBuf), "%s%d", szColor[unPokerColor - 1], RECOVER_CARD_VALUE(unPokerVal));
	return szBuf;
}

bool tagHAND_CARD::operator>( tagHAND_CARD& tag )
{
	tagHAND_CARD hc1(*this);
	tagHAND_CARD hc2(tag);

	//ENUM_CARD_TYPE nTypeOther = tag.nCardType;


	if (hc1.nCardType != hc2.nCardType)
	{
		//235�ͱ��ӱȽ�
		if (ct_235 == hc1.nCardType)
		{
			if (ct_leopard == hc2.nCardType)
			{
				//235 > ����
				return true;
			}
			hc1.nCardType = ct_single;
		}
		else if (ct_235 == hc2.nCardType)
		{
			if (ct_leopard == hc1.nCardType)
			{
				//���� < 235
				return false;
			}
			hc2.nCardType = ct_single;
		}

		//������ͬ���ͱȽ�
		if (hc1.nCardType > hc2.nCardType)	
		{

			return true;
		}
	}
	if (hc1.nCardType == hc2.nCardType)	//��ͬ���ͱȽ�
	{
		return CompareSameCardType(hc1, hc2);
	}
	return false;
}

bool tagHAND_CARD::CompareSameCardType( tagHAND_CARD& hc1, tagHAND_CARD& hc2 )
{
	//û�б���
	//��, ����, �����һ�ſ�ʼ��
	//����, �ȶ��Ӵ�С, �����Ŵ�С
	//˳��, ˳��,AKQ> KQJ>��>32A
	//�Ƶ��СA> K >....3>2
	if (hc1.nCardType == ct_pair)
	{
		if(hc1.arrCard[1].unPokerVal > hc2.arrCard[1].unPokerVal)
		{
			return true;
		}
		else if(hc1.arrCard[1].unPokerVal == hc2.arrCard[1].unPokerVal)
		{
			//������ͬ, �ȵ�������
			long nThird1 = (hc1.arrCard[0].unPokerVal == hc1.arrCard[1].unPokerVal 
				? hc1.arrCard[0].unPokerVal : hc1.arrCard[2].unPokerVal);
			long nThird2 = (hc2.arrCard[0].unPokerVal == hc2.arrCard[1].unPokerVal 
				? hc2.arrCard[0].unPokerVal : hc2.arrCard[2].unPokerVal);
			return nThird1 > nThird2;
		}
		return false;
	}
	else if(ct_gold_flower == hc1.nCardType
		|| ct_single == hc1.nCardType)
	{
		//����A���Ƚ�
		//���������ſ�ʼ��, ��һ��û���ڶ��־�����
		for (int i = 2; i >=0; --i)	//�޷���һֱ>=0, 
		{
			if (hc1.arrCard[i].unPokerVal > hc2.arrCard[i].unPokerVal )
			{
				return true;
			}
			else if(hc1.arrCard[i].unPokerVal < hc2.arrCard[i].unPokerVal )
			{
				return false;
			}
		}
		return false;
	}
	else if(ct_straight_diff == hc1.nCardType
		|| ct_straight_same == hc1.nCardType)
	{
		bool bIsA1 = hc1.arrCard[0].unPokerVal == 2 && hc1.arrCard[2].unPokerVal == 14;	//A23����
		bool bIsA2 = hc2.arrCard[0].unPokerVal == 2 && hc2.arrCard[2].unPokerVal == 14;
		long nMax1 = ( bIsA1 ? hc1.arrCard[1].unPokerVal : hc1.arrCard[2].unPokerVal );
		long nMax2 = ( bIsA2 ? hc2.arrCard[1].unPokerVal : hc2.arrCard[2].unPokerVal );
		return nMax1 > nMax2;
	}
	return true;
}

std::string tagHAND_CARD::ToStringID()
{
	//����3���Ƶ��ַ������, "20,21,25"
	char szBuf[10] = {0};
	sprintf_s(szBuf, "%d,%d,%d", arrCard[0].ToID(), arrCard[1].ToID(), arrCard[2].ToID());
	return szBuf;
}

std::string tagHAND_CARD::ToStringCard()
{
	char szBuf[60] = {0};
	sprintf_s(szBuf, sizeof(szBuf), "%s,%s,%s", arrCard[0].ToString().c_str(), arrCard[1].ToString().c_str(), arrCard[2].ToString().c_str());
	return szBuf;
}


CCardGoldenFlower::CCardGoldenFlower(void)
{
}


CCardGoldenFlower::~CCardGoldenFlower(void)
{
	m_pocket.UnInit();
}

bool CCardGoldenFlower::Init()
{
	m_pocket.ReInit();
	

	//test
	if (0)
	{
		
		byte nID = 0;
		string strCard;
		tagHAND_CARD hc;
		ARR_CARD& arrCard = hc.arrCard;
		tagFGFCard pc;
		pc.FromID(30);
		arrCard.push_back(pc);
		strCard = pc.ToString();
		pc.FromID(9);
		arrCard.push_back(pc);
		strCard = pc.ToString();
		pc.FromID(27);
		arrCard.push_back(pc);
		strCard = pc.ToString();
		RecognizeCardType(hc);

		{
			//����
			tagFGFCard pc;
			string strCard;
			tagHAND_CARD hc1;
			ARR_CARD& arrCard1 = hc1.arrCard;
			pc.FromID(4);				//5
			arrCard1.push_back(pc);
			strCard = pc.ToString();
			pc.FromID(5);				//6
			arrCard1.push_back(pc);
			strCard = pc.ToString();
			pc.FromID(45);				//A
			arrCard1.push_back(pc);
			strCard = pc.ToString();
			RecognizeCardType(hc1);

			tagHAND_CARD hc2;
			ARR_CARD& arrCard2 = hc2.arrCard;

			pc.FromID(32);
			arrCard2.push_back(pc);
			strCard = pc.ToString();
			pc.FromID(20);
			arrCard2.push_back(pc);
			strCard = pc.ToString();
			pc.FromID(35);
			arrCard2.push_back(pc);
			strCard = pc.ToString();
			RecognizeCardType(hc2);
			bool bVector = hc1 > hc2;

			arrCard2.clear();
			pc.FromID(7);
			arrCard2.push_back(pc);
			strCard = pc.ToString();
			pc.FromID(40);
			arrCard2.push_back(pc);
			strCard = pc.ToString();
			pc.FromID(5);
			arrCard2.push_back(pc);
			strCard = pc.ToString();
			RecognizeCardType(hc2);
			bVector = hc1 > hc2;

			arrCard2.clear();
			pc.FromID(6);
			arrCard2.push_back(pc);
			strCard = pc.ToString();
			pc.FromID(40);
			arrCard2.push_back(pc);
			strCard = pc.ToString();
			pc.FromID(43);
			arrCard2.push_back(pc);
			strCard = pc.ToString();
			RecognizeCardType(hc2);	//ֵ����
			bVector = hc1 > hc2;

			bVector = false;
		}

		arrCard.push_back(pc.FromID(45));
		arrCard.push_back(pc.FromID(20));
		arrCard.push_back(pc.FromID(22));
		arrCard.clear();
		pc.unPokerVal = 7;
		pc.unPokerColor = 1;
		nID = pc.ToID();
		arrCard.push_back(pc);
		pc.unPokerVal = 7;
		pc.unPokerColor = 1;
		nID = pc.ToID();
		arrCard.push_back(pc);
		pc.unPokerVal = 7;
		pc.unPokerColor = 1;
		nID = pc.ToID();
		arrCard.push_back(pc);
		RecognizeCardType(hc);
		assert(hc.nCardType == ct_leopard);


		arrCard.clear();
		pc.unPokerVal = 3;
		pc.unPokerColor = 2;
		arrCard.push_back(pc);
		pc.unPokerVal = 4;
		pc.unPokerColor = 2;
		arrCard.push_back(pc);
		pc.unPokerVal = 5;
		pc.unPokerColor = 2;
		arrCard.push_back(pc);
		RecognizeCardType(hc);
		assert(hc.nCardType == ct_straight_same);
		string strNum = hc.ToStringID();
		arrCard.clear();
		pc.unPokerVal = 12;
		pc.unPokerColor = 2;
		nID = pc.ToID();
		arrCard.push_back(pc);
		pc.unPokerVal = 13;
		pc.unPokerColor = 2;
		nID = pc.ToID();
		arrCard.push_back(pc);
		pc.unPokerVal = 14;
		pc.unPokerColor = 2;
		nID = pc.ToID();
		arrCard.push_back(pc);
		RecognizeCardType(hc);
		assert(hc.nCardType == ct_straight_same);
		strNum = hc.ToStringID();

		arrCard.clear();
		pc.unPokerVal = 5;
		pc.unPokerColor = 2;
		arrCard.push_back(pc);
		pc.unPokerVal = 3;
		pc.unPokerColor = 2;
		arrCard.push_back(pc);
		pc.unPokerVal = 4;
		pc.unPokerColor = 2;
		arrCard.push_back(pc);
		RecognizeCardType(hc);
		assert(hc.nCardType == ct_straight_same);

		arrCard.clear();
		pc.unPokerVal = 3;
		pc.unPokerColor = 3;
		arrCard.push_back(pc);
		pc.unPokerVal = 2;
		pc.unPokerColor = 3;
		arrCard.push_back(pc);
		pc.unPokerVal = 7;
		pc.unPokerColor = 3;
		arrCard.push_back(pc);
		RecognizeCardType(hc);
		assert(hc.nCardType == ct_gold_flower);

		arrCard.clear();
		pc.unPokerVal = 8;
		pc.unPokerColor = 3;
		arrCard.push_back(pc);
		pc.unPokerVal = 9;
		pc.unPokerColor = 3;
		arrCard.push_back(pc);
		pc.unPokerVal = 7;
		pc.unPokerColor = 2;
		arrCard.push_back(pc);
		RecognizeCardType(hc);
		assert(hc.nCardType == ct_straight_diff);

		arrCard.clear();
		pc.unPokerVal = 7;
		pc.unPokerColor = 3;
		arrCard.push_back(pc);
		pc.unPokerVal = 8;
		pc.unPokerColor = 2;
		arrCard.push_back(pc);
		pc.unPokerVal = 9;
		pc.unPokerColor = 1;
		arrCard.push_back(pc);
		RecognizeCardType(hc);
		assert(hc.nCardType == ct_straight_diff);

		arrCard.clear();
		pc.unPokerVal = 7;
		pc.unPokerColor = 3;
		arrCard.push_back(pc);
		pc.unPokerVal = 9;
		pc.unPokerColor = 1;
		arrCard.push_back(pc);
		pc.unPokerVal = 8;
		pc.unPokerColor = 2;
		arrCard.push_back(pc);
		RecognizeCardType(hc);
		assert(hc.nCardType == ct_straight_diff);

		arrCard.clear();
		pc.unPokerVal = 11;
		pc.unPokerColor = 3;
		arrCard.push_back(pc);
		pc.unPokerVal = 11;
		pc.unPokerColor = 2;
		arrCard.push_back(pc);
		pc.unPokerVal = 9;
		pc.unPokerColor = 1;
		arrCard.push_back(pc);
		RecognizeCardType(hc);
		assert(hc.nCardType == ct_pair);

		arrCard.clear();
		pc.unPokerVal = 3;
		pc.unPokerColor = 3;
		arrCard.push_back(pc);
		pc.unPokerVal = 2;
		pc.unPokerColor = 4;
		arrCard.push_back(pc);
		pc.unPokerVal = 5;
		pc.unPokerColor = 4;
		arrCard.push_back(pc);
		RecognizeCardType(hc);
		assert(hc.nCardType == ct_single);

		arrCard.clear();
		pc.unPokerVal = 3;
		pc.unPokerColor = 2;
		arrCard.push_back(pc);
		pc.unPokerVal = 2;
		pc.unPokerColor = 3;
		arrCard.push_back(pc);
		pc.unPokerVal = 5;
		pc.unPokerColor = 1;
		arrCard.push_back(pc);
		RecognizeCardType(hc);
		assert(hc.nCardType == ct_235);

		{
			//���ƴ�С

			tagHAND_CARD hc1;
			tagHAND_CARD hc2;
			//���ź͵���
			hc1.arrCard.clear();
			pc.unPokerVal = 4;
			pc.unPokerColor = 3;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 2;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 5;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			RecognizeCardType(hc1);

			hc2.arrCard.clear();
			pc.unPokerVal = 3;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 7;
			pc.unPokerColor = 1;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 5;
			pc.unPokerColor = 4;
			hc2.arrCard.push_back(pc);
			RecognizeCardType(hc2);
			assert(hc1 < hc2);

			//���Ӻͽ�
			hc1.arrCard.clear();
			pc.unPokerVal = 5;
			pc.unPokerColor = 3;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 5;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 5;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			RecognizeCardType(hc1);

			hc2.arrCard.clear();
			pc.unPokerVal = 8;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 7;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 5;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			RecognizeCardType(hc2);
			assert(hc1 > hc2);

			//���źͶ���
			hc1.arrCard.clear();
			pc.unPokerVal = 6;
			pc.unPokerColor = 3;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 5;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 8;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			RecognizeCardType(hc1);

			hc2.arrCard.clear();
			pc.unPokerVal = 9;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 9;
			pc.unPokerColor = 2;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 5;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			RecognizeCardType(hc2);
			assert(hc1 < hc2);

			//���ź͵���
			hc1.arrCard.clear();
			pc.unPokerVal = 10;
			pc.unPokerColor = 3;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 8;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 13;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			RecognizeCardType(hc1);

			hc2.arrCard.clear();
			pc.unPokerVal = 12;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 10;
			pc.unPokerColor = 2;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 5;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			RecognizeCardType(hc2);
			assert(hc1 > hc2);

			//˳�Ӻ�С˳��
			hc1.arrCard.clear();
			pc.unPokerVal = 14;
			pc.unPokerColor = 3;
			nID = pc.ToID();
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 12;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 13;
			pc.unPokerColor = 2;
			hc1.arrCard.push_back(pc);
			RecognizeCardType(hc1);

			hc2.arrCard.clear();
			pc.unPokerVal = 14;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 2;
			pc.unPokerColor = 2;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 3;
			pc.unPokerColor = 1;
			hc2.arrCard.push_back(pc);
			RecognizeCardType(hc2);
			assert(hc1 > hc2);
			//235�ͱ���
			hc1.arrCard.clear();
			pc.unPokerVal = 5;
			pc.unPokerColor = 3;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 3;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 2;
			pc.unPokerColor = 2;
			hc1.arrCard.push_back(pc);
			RecognizeCardType(hc1);

			hc2.arrCard.clear();
			pc.unPokerVal = 11;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 11;
			pc.unPokerColor = 2;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 11;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			RecognizeCardType(hc2);
			assert(hc1 > hc2);

			//���Ӻ�235
			hc1.arrCard.clear();
			pc.unPokerVal = 4;
			pc.unPokerColor = 3;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 2;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			pc.unPokerVal = 2;
			pc.unPokerColor = 4;
			hc1.arrCard.push_back(pc);
			RecognizeCardType(hc1);

			hc2.arrCard.clear();
			pc.unPokerVal = 5;
			pc.unPokerColor = 1;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 2;
			pc.unPokerColor = 2;
			hc2.arrCard.push_back(pc);
			pc.unPokerVal = 3;
			pc.unPokerColor = 3;
			hc2.arrCard.push_back(pc);
			RecognizeCardType(hc2);
			assert(hc1 > hc2);
		}
	}
	

	return true;
}

tagHAND_CARD CCardGoldenFlower::GetHandCard()
{
	tagHAND_CARD hc;

	PokerCard* pCards = new PokerCard[NUM_CARD_COUNT];
	UINT nRemain = m_pocket.GetRemainCount();
	if (nRemain < NUM_CARD_COUNT)
	{
		m_pocket.ReInit();
	}
	UINT nCount = m_pocket.GetPokersTimes(NUM_CARD_COUNT, pCards);
	if (nCount != NUM_CARD_COUNT)
	{
		//ȡ�ƴ���
		return hc;
	}

	for (int i = 0; i < NUM_CARD_COUNT; ++i)
	{
		tagFGFCard card(pCards[i].unPokerVal, pCards[i].unPokerColor);
		card.unPokerVal = CHANGE_CARD_VALUE(pCards[i].unPokerVal);

		hc.arrCard.push_back(card);
	}
	delete []pCards;
	
	//ʶ������
	RecognizeCardType(hc);
	return hc;
}

bool CCardGoldenFlower::RecognizeCardType( tagHAND_CARD& hc )
{
	//��������������
	sort(hc.arrCard.begin(), hc.arrCard.end(), &ComparePointAsce);

	//ɢ��
	hc.nCardType = ct_single;

	ARR_CARD& arrCard = hc.arrCard;
	
	typedef vector<long>	ARR_INT;
	ARR_INT arrPoint;
	ARR_INT arrColor;


	for (size_t i = 0; i < arrCard.size(); ++i)
	{
		arrPoint.push_back(arrCard[i].unPokerVal);
		arrColor.push_back(arrCard[i].unPokerColor);
	}

	sort(arrColor.begin(), arrColor.end(), less<long>());

	int nCountSamePoint = count(arrPoint.begin(), arrPoint.end(), arrPoint[1]);
	if (nCountSamePoint == 3)
	{
		//����
		hc.nCardType = ct_leopard;
	}
	else if(nCountSamePoint == 2)
	{
		//����
		hc.nCardType = ct_pair;
	}
	else
	{
		bool bSeries = (arrPoint[0] + 2 == arrPoint[1] + 1
			&& arrPoint[1] + 1 == arrPoint[2]); //�ж��Ƿ�����
		bSeries = (!bSeries ? arrPoint[0] == 2 && arrPoint[1] == 3 && arrPoint[2] == 14 : bSeries);	//A23����
		int nCountSameColor = count(arrColor.begin(), arrColor.end(), arrColor[1]);
		if (bSeries)
		{
			if (3 == nCountSameColor)
			{
				//˳��
				hc.nCardType = ct_straight_same;
			}
			else
			{
				//˳��
				hc.nCardType = ct_straight_diff;
			}
		}
		else if(3 == nCountSameColor)
		{
			//��, ������, ��ɫ��ͬ
			hc.nCardType = ct_gold_flower;
		}
		else if (2 == arrPoint[0] && 3 == arrPoint[1] && 5 == arrPoint[2]
			&& 1 == nCountSameColor)
		{
			//235, ��ɫ����ͬ
			hc.nCardType = ct_235;
		}
		
	}
	return true;
}

ARR_HAND_CARD CCardGoldenFlower::GetHandCardArr(UINT nPlayerCount)
{
	ARR_HAND_CARD arr;
	UINT nRemain = m_pocket.GetRemainCount();
	if (nRemain < NUM_CARD_COUNT * nPlayerCount)
	{
		m_pocket.ReInit();
	}
	for (size_t i = 0; i < nPlayerCount; ++i)
	{
		arr.push_back(GetHandCard());
	}
	return arr;
}
