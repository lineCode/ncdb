#pragma once

#include "PiTypeDef.h"
#include "PiProcessDef.h"

Pi_NameSpace_Begin

class CPiProcess
{
public:
    CPiProcess(void);
    ~CPiProcess(void);
public:
    static bool IsProcessNameExist( tcpchar szProcessName );
    static bool IsExistModule( tcpchar szModulePath );
    //枚举获取进程ID
    static bool EnumProcess(LST_HANDLE& pLstHandle);
	static bool KillProcess( tcpchar szProcessName, bool bKillAll );
	static bool IsProcessExistHandle( HANDLE hProcess , DWORD* pExitCode = NULL);
};

Pi_NameSpace_End
