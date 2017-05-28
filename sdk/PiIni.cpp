#include "stdafx.h"
#include "PiIni.h"
#include "windows.h"
#include "PiTypeDef.h"

#include "define_gnl.h"
//#include "ResCommon.h"
#include <shlwapi.h>

Pi_NameSpace_Using

const int BUF_SIZE = 1000;


CPiIni::CPiIni( const tcpchar filename /*= NULL*/ )
:m_pBuf(NULL)
{
	if(filename)
	{
		m_strFilePath = filename;
	}
	m_strDefaultValue = _T("");
}

ns_PiPi::CPiIni::CPiIni( StringCRef filename )
:m_pBuf(NULL)
{
	m_strFilePath = filename;
	m_strDefaultValue = _T("");
}



CPiIni::~CPiIni(void)
{
	UnInit();
}

bool CPiIni::Init()
{
	m_pBuf = new tchar[BUF_SIZE + 1];
	tmemset(m_pBuf, 0, BUF_SIZE + 1);
	if(m_strFilePath.empty())
	{
		return false;
	}
	return true;
}

bool CPiIni::UnInit()
{
	delete [] m_pBuf;
	m_pBuf = NULL;
	return true;
}

bool CPiIni::Set(tcpchar cDefault)
{
	m_strDefaultValue = cDefault;
	return true;
}

tpchar CPiIni::ReadParam(tcpchar section, tcpchar key)
{
	UNLONG dwRet = 0;
	//memset(m_buf.m_pwBuf,0, BUF_SIZE * 2);
	dwRet = GetPrivateProfileString(section, key, m_strDefaultValue.c_str(), m_pBuf, 
		BUF_SIZE, m_strFilePath.c_str());	
	if (dwRet <= 0)
	{
		UNLONG dwInt = GetLastError();
		return m_pBuf;
	}
	return m_pBuf;
}

UNINT CPiIni::ReadParamNum( tcpchar section, tcpchar key, int nDefault /*= 0*/ )
{
    return GetPrivateProfileInt(section, key, nDefault, m_strFilePath.c_str());
}

bool CPiIni::ReadParamBool( tcpchar section, tcpchar key, int nDefault /*= 0*/ )
{
	return !GetPrivateProfileInt(section, key, nDefault, m_strFilePath.c_str()) == 0;	//0为false, 其他true
	
}

bool CPiIni::WriteParam(tcpchar section, tcpchar key, tcpchar value)
{
	if (!_tcslen(value))
	{
		//DEBUG_ERROR(L"WriteParam, value is empty");
		return false;
	}

	if(!WritePrivateProfileString(section, key, value, m_strFilePath.c_str()))
	{
		//DEBUG_ERROR(L"write_ini fail");
		return false;
	}
	return true;
}

bool CPiIni::WriteParam( tcpchar section, tcpchar key, int value )
{
	TCHAR szBuf[100] = {0};
	_itot(value, szBuf, 10);
	return WriteParam(section, key, szBuf);
}

bool CPiIni::WriteParam( tcpchar section, tcpchar key, UNINT value )
{
	TCHAR szBuf[100] = {0};
	_itot(value, szBuf, 10);
	return WriteParam(section, key, szBuf);
}

bool CPiIni::WriteParam( tcpchar section, tcpchar key, tstring value )
{
	return WriteParam(section, key, value.c_str());

}

bool CPiIni::DeleteKey(tcpchar section, tcpchar key)
{
	if(!WritePrivateProfileString(section, key, NULL, m_strFilePath.c_str()))
	{
		DWORD dwErr = GetLastError();
		return false;
	}
	return true;
}

tstring CPiIni::GetPath()
{
	return m_strFilePath;
}

LST_STRING ns_PiPi::CPiIni::GetAllKey( tcpchar section )
{
	
	LST_STRING lstKey;
	if(!IsValidConfig())
	{
		return lstKey;
	}
	tcpchar szData = ReadParam(section, NULL);
	if (!szData)
	{
		return lstKey;
	}
	
	while(*szData)
	{
		lstKey.push_back(szData);
		szData += _tcslen(szData) + 1;	//定位到下一个key的位置
	}
	return lstKey;
}

tstring ns_PiPi::CPiIni::ReadParamString( tcpchar section, tcpchar key )
{
	UNLONG dwRet = 0;
	//memset(m_buf.m_pwBuf,0, BUF_SIZE * 2);
	dwRet = GetPrivateProfileString(section, key, m_strDefaultValue.c_str(), m_pBuf, 
		BUF_SIZE, m_strFilePath.c_str());	
	if (dwRet <= 0)
	{
		UNLONG dwInt = GetLastError();
		return _T("");
	}
	return m_pBuf;
}

bool ns_PiPi::CPiIni::IsValidConfig()
{
	return PathFileExists(m_strFilePath.c_str()) == TRUE;
}
