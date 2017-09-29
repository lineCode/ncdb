#pragma once
/*------------------------------------------------------------
	file:区间随机数	模块
------------------------------------------------------------*/
class CRandNum
{
public:
	//构造nBegin-nEnd之间的随机数闭区间间
	CRandNum(int nBegin, int nEnd);
	CRandNum(int nBegin, int nEnd, int nPercent);
	//构造1-nEnd之间的随机数闭区间间
	CRandNum(int nEnd = 1);
	~CRandNum(void);
public:
	// 获取随机数
	int GetNum();
	//获取随机数并判断是否在区间里
	bool IsRandHit();
protected:
	UINT GetNumRange(int nBegin, int nEnd);
private:
	int		m_nBegin;
	int		m_nEnd;
	UINT	m_nPercent;		//概率
};

