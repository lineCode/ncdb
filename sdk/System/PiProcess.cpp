#include "StdAfx.h"
#include "PiProcess.h"
#include <windows.h>
#include <Tlhelp32.h>
#include "define_gnl.h"
#include <Psapi.h>
#include <algorithm>
#include "PiString.h"
#include "PiDisk.h"

Pi_NameSpace_Using

CPiProcess::CPiProcess(void)
{
}


CPiProcess::~CPiProcess(void)
{
}

bool CPiProcess::IsProcessNameExist( tcpchar szProcessName )
{
    bool bExist = false;
    HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32 pe = {0};
    pe.dwSize = sizeof(PROCESSENTRY32);
    if(!::Process32First(hSnap, &pe))
    {
        return bExist;
    }

    if ( 0 == _tcscmp(pe.szExeFile, szProcessName))
    {
        bExist = true;
        return bExist;
    }

    while(::Process32Next(hSnap, &pe))
    {
        if (0 == ::_tcscmp(pe.szExeFile, szProcessName))
        {
            bExist = true;
            break;
        }
    }
    return bExist;
}


bool CPiProcess::EnumProcess( LST_HANDLE& pLstHandle )
{
    bool bExist = false;
    HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32 pe = {0};
    pe.dwSize = sizeof(PROCESSENTRY32);
    if(!::Process32First(hSnap, &pe))
    {
        return bExist;
    }
    pLstHandle.push_back((HANDLE)pe.th32ProcessID);
    
    while(::Process32Next(hSnap, &pe))
    {
        pLstHandle.push_back((HANDLE)pe.th32ProcessID);
    }
    return true;
}

bool CPiProcess::IsExistModule( tcpchar szModulePath )
{
    LST_HANDLE lstHandle;
    if(!EnumProcess(lstHandle))
    {
        return false;
    }
    HMODULE hMd = NULL;
    DWORD dwNeed = 0;
    tchar szBuf[MAX_PATH + 1] = {0};
    CPiDisk disk;
    disk.Init();
    FOR_EACH_ITER(LST_HANDLE, lstHandle, it)
    {
        HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD)*it);
        //CRAIIHandle raii(hPro);
        if (!hPro)
        {
			CloseHandle(hPro);
			continue;
        }

        DWORD nSize = MAX_PATH;
        if(!GetProcessImageFileName(hPro, szBuf, nSize))
        {
			CloseHandle(hPro);
			continue;
        }
        
        tstring strRealPath = disk.GetDosPathFromMapping(szBuf);

        CPiString strSearch(szModulePath);
        /*transform(strSearch.begin(), strSearch.end(), strSearch.begin(), tolower);

        transform(strRealPath.begin(), strRealPath.end(), strRealPath.begin(), tolower);*/

        if( strSearch.EqualNoCase(strRealPath.c_str()))
        {
			CloseHandle(hPro);
			return true;
        }
		CloseHandle(hPro);
	}
    return false;
}

bool CPiProcess::KillProcess( tcpchar szProcessName, bool bKillAll )
{
	PROCESSENTRY32* pPE = NULL;
	HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pe = {0};
	pe.dwSize = sizeof(PROCESSENTRY32);
	if(!::Process32First(hSnap, &pe))
	{
		return true;
	}
	pPE = &pe;

	while(pPE)
	{
		if ( CPiString(pPE->szExeFile).EqualNoCase(szProcessName) )
		{
			HANDLE hPro = OpenProcess(PROCESS_TERMINATE, FALSE, pPE->th32ProcessID);
			TerminateProcess((HANDLE)hPro, -1);
			CloseHandle(hPro);

			if (!bKillAll)
			{
				break;
			}
		}
		if (!::Process32Next(hSnap, &pe))
		{
			break;
		}
		pPE = &pe;
	}

	return true;
}

bool ns_PiPi::CPiProcess::IsProcessExistHandle( HANDLE hProcess , DWORD* pExitCode /*= NULL*/ )
{
	bool bExist = false;
	DWORD dwExit = 0;
	if(GetExitCodeProcess(hProcess, &dwExit))
	{
		bExist = ( dwExit == STILL_ACTIVE );
	}
	if (pExitCode)
	{
		*pExitCode = dwExit;
	}
	return bExist;
}

