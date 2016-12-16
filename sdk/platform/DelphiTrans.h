#pragma once
#include "PiTypeDef.h"
class CDelphiTrans
{
public:
	CDelphiTrans();
	~CDelphiTrans();
public:
	static tcpchar FilterToMFC(tcpchar szFilter);
	static tcpchar FilterToWin32(tcpchar szFilter);
};

