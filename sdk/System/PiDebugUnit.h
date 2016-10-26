#pragma once
#include "..\PiTypeDef.h"
#include <windows.h>

class CPiDebugUnit
{
public:
	CPiDebugUnit(void);
	~CPiDebugUnit(void);
public:
	static void OutTime(tcpchar szS);
	static void OutFormat(const TCHAR* format,...);
	static bool SetDumpUnhandledException();
private:
	static LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo);
	static int GenerateMiniDump(HANDLE hFile, PEXCEPTION_POINTERS pExceptionPointers, PWCHAR pwAppName);
};
