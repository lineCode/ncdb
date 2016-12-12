#pragma once
#include "PiTypeDef.h"
class CDelphiTrans
{
public:
	CDelphiTrans();
	~CDelphiTrans();
public:
	static tstring FilterToMFC(tcpchar szFilter);
};

