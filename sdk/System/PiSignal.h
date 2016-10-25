#pragma once
#include "PiTypeDef.h"
class CPiSignal
{
public:
	CPiSignal(void);
	~CPiSignal(void);
public:
	bool Init(UNINT nInitial = 0, UNINT nMax = 0);
	bool UnInit();
	bool Add(UNINT nCount = 1);
	bool SetSign(bool bRet = true);
	bool WaitSign();
private:
	HANDLE m_hSemaphore;
	bool		m_bWaitRet;
};

