#pragma once

typedef int MoneyType;
#define NUM_1_YI	(100000000)
class tagMoney
{
public:
	typedef __int64 InnerMoneyType;
	InnerMoneyType		m_money;

	explicit tagMoney(MoneyType nMoney = 0, MoneyType nBilion = 0)
		:m_money(InnerMoneyType(nBilion) * NUM_1_YI + nMoney)//负数, 判断

	{

	}
	//获取金币亿万字段
	_inline MoneyType GetBilion() const { return MoneyType(m_money / NUM_1_YI);}
	//获取金币基值字段
	_inline MoneyType GetBase()const { return m_money % NUM_1_YI; }	//负数, 判断
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

