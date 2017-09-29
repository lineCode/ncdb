#pragma once
#include <xfunctional>
#include <map>
#include "RangeNum.h"
using std::map;
using std::multimap;
using std::greater;
using std::less;
typedef CRangeNum<byte>			NumPercent;


//百分比动作模板类
template<class ElemType>
class CPiElemRandPercent
{
public:
	CPiElemRandPercent();
	~CPiElemRandPercent();
	typedef multimap<byte, ElemType, greater<byte>>		MAP_ADD;		//插入时从大概率到小概率顺序
	typedef map<NumPercent, ElemType, less<NumPercent>>	MAP_PROB;		//内部保存为升序, 低值(大概率)往高值(小概率)保存 

public:
	//增加动作对应的百分百概率
	void AddProb(byte nPercent, const ElemType& tag);
	void AddProb(const MAP_ADD& mapProb);
	ElemType GetProb();
	void Clear();
private:
	MAP_PROB				m_mapProb;
	NumPercent				m_nPercentTotal;								
};

template<class ElemType>
CPiElemRandPercent<ElemType>::CPiElemRandPercent()
	:m_nPercentTotal(0, ec_big)
{

}

template<class ElemType>
CPiElemRandPercent<ElemType>::~CPiElemRandPercent()
{

}

template<class ElemType>
void CPiElemRandPercent<ElemType>::AddProb( byte nPercent, const ElemType& tag )
{
	m_mapProb[m_nPercentTotal] = tag;
	m_nPercentTotal += nPercent;
}

template<class ElemType>
void CPiElemRandPercent<ElemType>::AddProb( const MAP_ADD& mapProb )
{
	byte byTotal = 0;
	for (auto& prob : mapProb)
	{
		AddProb(prob.first, prob.second);
		byTotal += prob.first;
	}
	assert(byTotal == 100);	//所有动作概率加起来要100%
}

template<class ElemType>
ElemType CPiElemRandPercent<ElemType>::GetProb()
{
	byte nPercent = CGlobalOpt::GetRand(NUM_PERCENT_FULL) - 1;
	ElemType tag;
	NumPercent nP(nPercent, ec_big);

	for (auto& aProb : m_mapProb)
	{
		if(nPercent >= aProb.first.GetVal())
		{
			tag = aProb.second;
			break;
		}
	}
	return tag;
}

