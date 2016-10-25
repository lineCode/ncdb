#pragma once
#include "PiTypeDef.h"
class CUIDataChange
{
public:
	CUIDataChange(void);
	~CUIDataChange(void);
public:
	static float GetNumAmount( StringCRef strTemp );
	//È¥³ý×Ö·û´®×óÓÒÁ½±ßµÄ¿Õ¸ñ, »»ÐÐµÈ¿Õ×Ö·û
	static tstring TermStr(StringCRef strData);
	static double PercentSignToDecimal( StringCRef strTemp );
	static double PercentToDecimal( StringCRef strTemp );
};

