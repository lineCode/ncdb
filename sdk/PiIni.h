#pragma once
#include "PiTypeDef.h"

/************************************************************************/
/* fun:		ini配置文件操作类,可支持 Key和value 有中文字符,(必须用宽字节的版本)
 * remark:	不区分大小写,值的引号可有可无
/************************************************************************/
Pi_NameSpace_Begin
class CPiIni
{
public:
	CPiIni(StringCRef filename);
	CPiIni(const tcpchar filename = NULL);
	~CPiIni(void);

public:
	//读取参数
	virtual tpchar  ReadParam(tcpchar section, tcpchar key);
	virtual tstring  ReadParamString(tcpchar section, tcpchar key);
	UNINT ReadParamNum(tcpchar section, tcpchar key, int nDefault = 0);
	bool ReadParamBool(tcpchar section, tcpchar key, int nDefault = 0);

	bool Init();
	bool Set(tcpchar cDefault);
	tstring FileName() const { return m_strFilePath; }
	void FileName(tcpchar val) { m_strFilePath = val; }

	//写入参数
	bool WriteParam(tcpchar section, tcpchar key, tcpchar value);
	bool WriteParam(tcpchar section, tcpchar key, tstring value);
	bool WriteParam(tcpchar section, tcpchar key, int value);
	bool WriteParam(tcpchar section, tcpchar key, UNINT value);

	bool DeleteKey(tcpchar section, tcpchar key);
	LST_STRING GetAllKey(tcpchar section);
	bool UnInit();
	bool IsValidConfig();
protected:
	/*union {
		char	m_pBuf[100];
		wchar_t m_pwBuf[100];
	} m_buf;*/
	tpchar			m_pBuf;
	tstring			m_strFilePath;
	
	//const tcpchar m_filenameW;
	//const char* m_filename;
	//tcpchar m_cDefault;
	tstring			m_strDefaultValue;
};

Pi_NameSpace_End