#pragma once

typedef int MoneyType;
#define NUM_1_YI	(100000000)
class tagMoney
{
public:
	typedef __int64 InnerMoneyType;
	InnerMoneyType		m_money;

	explicit tagMoney(MoneyType nMoney = 0, MoneyType nBilion = 0)
		:m_money(InnerMoneyType(nBilion) * NUM_1_YI + nMoney)//����, �ж�

	{

	}
	//��ȡ��������ֶ�
	_inline MoneyType GetBilion() const { return MoneyType(m_money / NUM_1_YI);}
	//��ȡ��һ�ֵ�ֶ�
	_inline MoneyType GetBase()const { return m_money % NUM_1_YI; }	//����, �ж�
	_inline InnerMoneyType GetVal()const { return m_money ; }
	operator InnerMoneyType()const {return m_money;}
	tagMoney& operator =(InnerMoneyType money)
	{
		m_money = money; 
		return *this;
	}
	tagMoney& operator +=(const InnerMoneyType& nMoney) { m_money += nMoney; return *this;}
	tagMoney& operator -=(const InnerMoneyType& nMoney) { m_money -= nMoney; return *this;}
	//bool operator >(const InnerMoneyType& nMoney)const { return m_money > nMoney;}
	//bool operator >=(const InnerMoneyType& nMoney)const { return m_money >= nMoney;}
	//bool operator <(const InnerMoneyType& nMoney)const { return m_money < nMoney;}
	//bool operator <=(const InnerMoneyType& nMoney)const { return m_money <= nMoney;}

};

