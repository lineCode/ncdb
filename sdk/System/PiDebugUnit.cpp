#include "StdAfx.h"
#include "PiDebugUnit.h"
#include "..\PiTime.h"
#include <DbgHelp.h>  
#include <Strsafe.h>
#include "..\functional.h"

CPiDebugUnit::CPiDebugUnit(void)
{
}

CPiDebugUnit::~CPiDebugUnit(void)
{
}

void CPiDebugUnit::OutTime(tcpchar szMsg)
{
	OutInfo(CPiTime::GetSysTime().c_str(), tstring(_T(" ")) + szMsg);
}


//����DUMP�ļ�
int CPiDebugUnit::GenerateMiniDump(HANDLE hFile, PEXCEPTION_POINTERS pExceptionPointers, PWCHAR pwAppName)
{
	BOOL bOwnDumpFile = FALSE;
	HANDLE hDumpFile = hFile;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;

	typedef BOOL(WINAPI * MiniDumpWriteDumpT)(
		HANDLE,
		DWORD,
		HANDLE,
		MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION,
		PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION
		);

	MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
	HMODULE hDbgHelp = LoadLibrary(L"DbgHelp.dll");
	if (hDbgHelp)
		pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

	if (pfnMiniDumpWriteDump)
	{
		TCHAR szFileName[MAX_PATH] = { 0 };
		if (hDumpFile == NULL || hDumpFile == INVALID_HANDLE_VALUE)
		{
			TCHAR szPath[MAX_PATH] = { 0 };
			TCHAR* szAppName = pwAppName;
			TCHAR* szVersion = L"v1.0";
			TCHAR dwBufferSize = MAX_PATH;
			SYSTEMTIME stLocalTime;

			GetLocalTime(&stLocalTime);
			GetTempPath(dwBufferSize, szPath);

			StringCchPrintf(szFileName, MAX_PATH, L"%s%s", szPath, szAppName);
			CreateDirectory(szFileName, NULL);

			StringCchPrintf(szFileName, MAX_PATH, L"%s%s//%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
				szPath, szAppName, szVersion,
				stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
				stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
				GetCurrentProcessId(), GetCurrentThreadId());
			hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

			bOwnDumpFile = TRUE;
		}

		if (hDumpFile != INVALID_HANDLE_VALUE)
		{
			OutputDebugString(szFileName);
			ExpParam.ThreadId = GetCurrentThreadId();
			ExpParam.ExceptionPointers = pExceptionPointers;
			ExpParam.ClientPointers = FALSE;

			pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
				hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &ExpParam : NULL), NULL, NULL);

			if (bOwnDumpFile)
				CloseHandle(hDumpFile);
		}
		else
		{
			tstring strMsg(_T("sy dump �ļ�����ʧ��:"));
			strMsg += szFileName;
			OutputDebugString(strMsg.c_str());			
		}
	}

	if (hDbgHelp != NULL)
		FreeLibrary(hDbgHelp);

	return EXCEPTION_EXECUTE_HANDLER;
}


LONG WINAPI CPiDebugUnit::ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	if (IsDebuggerPresent())
	{
		OutputDebugString(_T("�е�����, �������쳣"));
		return EXCEPTION_CONTINUE_SEARCH;
	}
	OutputDebugString(_T("��׽���쳣"));

	return GenerateMiniDump(NULL, lpExceptionInfo, L"test");
}

bool CPiDebugUnit::SetDumpUnhandledException()
{
	SetUnhandledExceptionFilter(&CPiDebugUnit::ExceptionFilter);
	return true;
}



void CPiDebugUnit::OutFormat(const TCHAR* format,...)
{
	TCHAR *buff = (TCHAR*)0;
	va_list arglist;
	va_start(arglist, format);
	int len = _vsctprintf(format,arglist) + 1;
	buff = (TCHAR*)malloc(len * sizeof(TCHAR));
	if(buff)
	{
		memset(buff,0,len * sizeof(TCHAR));
		_vstprintf_s(buff,len,format,arglist);
		OutInfo(buff);
		free(buff);
	} 
	va_end(arglist);
}

tstring CPiDebugUnit::GetBuildDateTime()
{
	tstring strDT;
	strDT += _T(__DATE__);
	strDT += _T(" ");
	strDT += _T(__TIME__);
	return strDT;
}

void CPiDebugUnit::StartUpConsole()
{
	setlocale(LC_ALL, "CHS"); 
	int nCrt = 0;
	FILE* fp;
	AllocConsole();
	nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	fp = _fdopen(nCrt, "w+");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
}

void CPiDebugUnit::ColseConsole()
{
	FreeConsole();
}
