#pragma once
#include "..\PiTypeDef.h"
#include <windows.h>

class CPiDebugUnit
{
public:
	CPiDebugUnit(void);
	~CPiDebugUnit(void);
public:
	static void OutTime(tcpchar szMsg);
	static void OutFormat(const TCHAR* format,...);
	static bool SetDumpUnhandledException();
    //???????
    void StartUpConsole()
    //???????
    void ColseConsole()
    /************************************************************************
		fun:	??????(PiDebugUnit.h)?????
		param:	
		memo:	
	************************************************************************/
	static tstring GetBuildDateTime();
private:
	static LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo);
	static int GenerateMiniDump(HANDLE hFile, PEXCEPTION_POINTERS pExceptionPointers, PWCHAR pwAppName);
};
