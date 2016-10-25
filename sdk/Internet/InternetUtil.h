#pragma once
#include "PiTypeDef.h"
class CInternetUtil
{
public:
	CInternetUtil(void);
	~CInternetUtil(void);
	static bool OpenUrl( tcpchar szUrl );
	tstring GetPublicIP();
public:
	
};
