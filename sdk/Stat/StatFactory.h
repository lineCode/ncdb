#pragma once
#include "BYStatHeader.h"
#include "IByStat.h"
//class IByStat;
class DLL_CLASS CStatFactory
{
public:
	CStatFactory(void);
	~CStatFactory(void);
public:
	static ns_by_stat::IByStat*	MakeByStat();
	static void Clear(ns_by_stat::IByStat* pStat);
};
