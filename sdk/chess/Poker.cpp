//�˿������
#include "stdafx.h"
#include "Poker.h"

Poker::Poker()
{
	m_nPokerSet = 1;	//һ����
	m_bJack = TRUE;		//�д�С��
	m_unPokerCount = 13*4 + 2;
}

Poker::~Poker()
{
	UnInit();
}

void Poker::InitPokerSet(UINT unSet,BOOL bJack)
{
	m_nPokerSet = unSet;
	m_bJack = bJack;

	switch (unSet)
	{
	case 1:
		{
			if(m_bJack)
				m_unPokerCount = 13*4 + 2;	//�д�С��
			else
				m_unPokerCount = 13*4;

			//m_vctPokerPool_1
			for(UINT nCnt=0;nCnt<m_unPokerCount;nCnt++)
			{
				PokerCard pPoker;
				if(nCnt<13 && nCnt>=0)
				{
					pPoker.unPokerColor = 1;
					pPoker.unPokerVal = nCnt+1;
				}
				else if(nCnt<26 && nCnt>=13)
				{
					pPoker.unPokerColor = 2;
					pPoker.unPokerVal = (nCnt+1)-13;
				}
				else if(nCnt<39 && nCnt>=26)
				{
					pPoker.unPokerColor = 3;
					pPoker.unPokerVal = (nCnt+1)-26;
				}
				else if(nCnt<52 && nCnt>=39)
				{
					pPoker.unPokerColor = 4;
					pPoker.unPokerVal = (nCnt+1)-39;
				}
				else if(nCnt<54 && nCnt>=52)
				{
					pPoker.unPokerColor = 0;
					pPoker.unPokerVal = (nCnt+1)-39;//14 15��ʾ��С��
				}
				m_vctPokerPool_1.push_back(pPoker);
			}
		}
		break;
	case 2:
		{
			if(m_bJack)
				m_unPokerCount = (13*4 + 2)*2;	//�д�С��
			else
				m_unPokerCount = (13*4)*2;
		}
		break;
		default:
			
		break;
	}

	if(unSet>3)
	{
		AfxMessageBox(_T("���ܳ�ʼ�����Ƴ���2������,ֻ����1-2����!!"));
		return;
	}
}

long Poker::InitUsedPokerSet()
{
	long lRlt = 1;
	//m_vctUsePokers
	//m_unPokerCount
	if(m_nPokerSet == 1)
	{
		vector<int> poker;
		for(UINT i=0;i<m_unPokerCount;i++)  
		{
			poker.push_back(i); 
		}
		random_shuffle(poker.begin(),poker.end());

		for(UINT i=0;i<m_unPokerCount;i++) 
		{
			int nVal = poker[i];
			m_vctUsePokers.push_back(m_vctPokerPool_1[nVal]);
		}
	}
	else if(m_nPokerSet == 2)
	{

	}
	else
	{
		return lRlt = 0;
	}
	m_nCardEndIndex = m_vctUsePokers.size();
	return lRlt;
}

void Poker::UnInitUsedPokerSet()
{
	m_vctUsePokers.clear();
	m_nCardEndIndex = 0;
}

UINT Poker::GetPokers(UINT unSize,lpPokerCard pPoker)
{
	UINT unRlt = 0;
	UINT lCnt = m_vctUsePokers.size();
	if(lCnt>=unSize)
	{
		for (UINT i=0;i<unSize;i++)
		{
			pPoker[i].unPokerColor = m_vctUsePokers[i].unPokerColor;
			pPoker[i].unPokerVal = m_vctUsePokers[i].unPokerVal;
			unRlt += 1;
		}
	}
	else
	{
		unRlt = 0;
	}

	return unRlt;
}


void Poker::UnInit()
{
	UnInitUsedPokerSet();
	m_vctPokerPool_1.clear();
	m_vctPokerPool_2.clear();
}

UINT Poker::GetPokersTimes( UINT unSize, lpPokerCard pPoker )
{
	UINT nGetCount = 0;
	if (m_nCardEndIndex + 1 < unSize)
	{
		return nGetCount;
	}
	for (UINT nIndex = m_nCardEndIndex - 1; nIndex >= 0 && nGetCount < unSize; --nIndex)
	{
		pPoker[nGetCount].unPokerColor = m_vctUsePokers[nIndex].unPokerColor;
		pPoker[nGetCount].unPokerVal = m_vctUsePokers[nIndex].unPokerVal;
		nGetCount += 1;
	}

	m_nCardEndIndex -= nGetCount;
	return nGetCount;
}

UINT Poker::GetRemainCount()
{
	return m_nCardEndIndex + 1;
}

bool Poker::ReInit()
{
	UnInit();
	InitPokerSet(1, FALSE);
	InitUsedPokerSet();
	return true;
}

byte _PokerCard::ToID()
{
	//��ת�ɶ�Ӧ������id
	/************************************************************************
		����ɫ, ������
	//�� ( ��1-13, �� 14-26,÷ 27-39, ��40--52, 53С��, 54����)
	************************************************************************/
	byte nID = 0;
	if (unPokerVal >= 14 && unPokerVal <= 15 && unPokerColor != 0)
	{
		//��ֵ����
		return nID;
	}
	if (unPokerVal >= 14)
	{
		nID = (byte)(13 * 4 + unPokerVal - 13);		//��С��, ��
	}
	else
	{
		nID = (byte)(this->unPokerVal + (13 * this->unPokerColor - 1));
	}
	return nID;
}
