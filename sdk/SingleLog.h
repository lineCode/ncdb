#pragma once
#include "singlepattern.h"
#include "PiTypeDef.h"

#include "LogSpilit.h"
class CLogOpr;
class CLock;
Pi_NameSpace_Begin



class CSingleLog :
	public CSinglePattern<CLogSpilit>
{

public:
    typedef CLogSpilit::INIT_PARAM INIT_PARAM;

    static void         Init();
    static void         UnInit();
    static void         SetParam(const INIT_PARAM&);
	static bool			OpenTodayLog(tcpchar szPath);
    static DWORD	    WriteLog(tcpchar data);
	static DWORD        WriteLog(StringCRef data);
	static DWORD        WriteLog(int data);
    static DWORD        WriteLogPure(tcpchar data);
    static DWORD        WriteLogPure(StringCRef data);
protected:
 
private:
	CSingleLog(void);
	CSingleLog(CSingleLog&);
	 ~CSingleLog(void);

    static CLogSpilit* m_pstSingleLog;
    static CLock*   m_pLock;
    static string m_strDate;//保存date, 用来判断是否要开启一个新的Log
};

Pi_NameSpace_End
