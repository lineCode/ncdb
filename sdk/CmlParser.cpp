#include "StdAfx.h"
#include "CmlParser.h"
#include "PiString.h"
#include "PathLight.h"

Pi_NameSpace_Using
CCmlParser::CCmlParser(void)
{
	m_strSpilit = _T("-");
}

CCmlParser::~CCmlParser(void)
{
}

bool CCmlParser::Parse()
{
	LPTSTR pszCmdLine = GetCommandLine(); //获取命令行参数 
	return Parse(pszCmdLine);
	
}

bool CCmlParser::IsHasParam( tcpchar szP )
{
	return m_mapParam.find(szP) != m_mapParam.end();
}

tstring CCmlParser::GetParamValue( tcpchar szP )
{
	return m_mapParam[szP];
}

tstring CCmlParser::GetCml()
{
	return m_strCml;
}

bool CCmlParser::Parse( StringCRef strCmd )
{
	//例子: "2.exe -d 11 -s 22"
	size_t nPos = strCmd.find(m_strSpilit.c_str());
	if (nPos == tstring::npos)
	{
		return true;
	}

	//跳过自身路径, 去掉参数值尾部的空格
	//strCmd += nPos;
	ARR_STRING lstParamAndValue = CPiString::SpilitStrToArray(strCmd.c_str() + nPos, _T(" "), _T("\""));	//"-d 11 " "-s 22"

	bool bIsFlag = false;
	tstring strFlagOrValue;
	for(int i = 0; i < lstParamAndValue.size(); ++i)
	{
		strFlagOrValue = lstParamAndValue[i];
		bIsFlag = strFlagOrValue.find(m_strSpilit.c_str()) != tstring::npos;

		if (bIsFlag && i < lstParamAndValue.size() - 1)	//TODO:考虑有给参数标识, 但没有给参数值的情况, 连续两个参数标识
		{
			m_mapParam[strFlagOrValue] = lstParamAndValue[++i];
		}
		else
		{
			m_mapParam[strFlagOrValue] = _T("");
		}
	}
	m_strCml = strCmd;
	return true;
}

tstring CCmlParser::operator[]( tcpchar szP )
{
	return GetParamValue(szP);
}
