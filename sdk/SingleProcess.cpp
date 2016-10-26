#include "StdAfx.h"
#include "SingleProcess.h"
#include <string>
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")
using std::string;

CSingleProcess::CSingleProcess(void)
{
    m_hMutex = NULL;
    m_bExist = false;
	m_bLocal = false;
}


CSingleProcess::~CSingleProcess(void)
{
    if (m_hMutex)
    {
        ::CloseHandle(m_hMutex);
        m_hMutex = NULL;
    }
}

bool CSingleProcess::CreateRunFlag( tcpchar szFlagName )
{
	tstring strName(_T("Global\\"));
	if (m_bLocal)
	{
		strName = _T("Local\\");
	}
	strName += szFlagName;
    m_hMutex = ::CreateMutex(NULL, FALSE, szFlagName);
    m_bExist = false;
    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        //has exist app instance
        m_bExist = true;
        //AfxMessageBox(L"已经开启了app");
        
    }
    return !m_bExist;
}

bool CSingleProcess::IsExistInst()
{
    return m_bExist;
}

