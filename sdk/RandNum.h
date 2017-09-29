#pragma once
/*------------------------------------------------------------
	file:���������	ģ��
------------------------------------------------------------*/
class CRandNum
{
public:
	//����nBegin-nEnd֮���������������
	CRandNum(int nBegin, int nEnd);
	CRandNum(int nBegin, int nEnd, int nPercent);
	//����1-nEnd֮���������������
	CRandNum(int nEnd = 1);
	~CRandNum(void);
public:
	// ��ȡ�����
	int GetNum();
	//��ȡ��������ж��Ƿ���������
	bool IsRandHit();
protected:
	UINT GetNumRange(int nBegin, int nEnd);
private:
	int		m_nBegin;
	int		m_nEnd;
	UINT	m_nPercent;		//����
};

