//�˿������

#pragma once
#include "stdafx.h"
#include <vector>
#include <algorithm>

using namespace std;

//ÿ���Ƶ����ݽṹ
typedef struct _PokerCard{
	long	unPokerVal;		//��ֵ 1-13��K��14,15��ʾС����		
	long	unPokerColor;	//����ɫ (1 ��,2��,3÷,4��,0��)
	_PokerCard()
	{
		unPokerVal = 0;
		unPokerColor = -1;
	}
	byte ToID();
	string ToString()
	{
		char szBuf[10] = {0};
		char* szColor[] = {"����", "����", "÷��", "����"};
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
	**��������InitPokerSet
	**���ܣ�ʹ��ǰ��ʼ���Ƴ�
	**����1��unSet ������ ��1����2���ƣ�
	**����2��bJack �Ƿ��д�С�� (TRUE��/FALSE��) 
	*******************************************/
	void InitPokerSet(UINT unSet,BOOL bJack);	

	/******************************************
	**��������InitUsedPokerSet
	**���ܣ���ʼ����ʹ�õķ��Ƴ�
	*******************************************/
	long InitUsedPokerSet();

	/******************************************
	**��������UnInitUsedPokerSet
	**���ܣ�������Ƴ�
	*******************************************/
	void UnInitUsedPokerSet();

	/******************************************
	**��������GetPokers
	**���ܣ���ȡ������
	**����1��unSize ����
	**����2������ ���� pPoker = new OnePoker[unSize];
	**����ֵ��ʵ�ʵõ�������
	*******************************************/
	UINT GetPokers(UINT unSize, lpPokerCard pPoker);
	UINT GetPokersTimes(UINT unSize, lpPokerCard pPoker);

	/******************************************
	**��������UnInit
	**���ܣ������������
	*******************************************/
	void UnInit();
	bool ReInit();
	UINT GetRemainCount();

	UINT				m_nPokerSet;	//������ 1����2����
	BOOL				m_bJack;		//�Ƿ��д�С��(TRUE��/FALSE��) 
	UINT				m_unPokerCount;	//�Ƶ�������
	UINT				m_nCardEndIndex;	//��ǰʣ���ƵĽ���λ��

	vector<PokerCard>	m_vctPokerPool_1;	//�Ƴ�1��1���ƣ�(1-13��)(14-26��)��27-39÷��(40-52��)��53��54��С����
	vector<PokerCard>	m_vctPokerPool_2;	//�Ƴ�2��1���ƣ�(1-13��)(14-26��)��27-39÷��(40-52��)��53��54��С����

	vector<PokerCard>    m_vctUsePokers;	//ÿ�η���ǰ,��ʼ������ʹ�õ��Ƴ�
};
