#pragma once
#include "PiTypeDef.h"
/*------------------------------------------------------
	Fun:	参数解析类
	remark:	参数需要每个参数都跟参数值 -r [value] -d [value].  参数后面没其他字段了可以没有参数值, 如: "-r"  或者 "-r 1 -d"
	Authon: liwanliang
	Date:	2016/03/17
------------------------------------------------------*/
class CCmlParser
{
public:
	CCmlParser(void);
	~CCmlParser(void);
public:
	bool Parse();
	bool IsHasParam( tcpchar szP );
	tstring GetParamValue(tcpchar szP);
	tstring operator[](tcpchar szP);
	tstring GetCml();
	bool Parse( StringCRef strCmd );
	void SetParamFlag(tcpchar szFlag) { m_strSpilit = szFlag;}

public:
	map<tstring, tstring>			m_mapParam;
	tstring							m_strSpilit;
	tstring							m_strCml;
};
