#include "StdAfx.h"
#include "DllManager.h"
#include <assert.h>

CDllManager::CDllManager(void)
{
    m_hDll = NULL;
}

CDllManager::~CDllManager(void)
{
	UnLoad();
}

bool CDllManager::Load(const char* szFile)
{
    m_hDll = LoadLibraryA(szFile);
	return m_hDll != NULL;
}

bool CDllManager::UnLoad()
{
    bool bRet = true;
    if (m_hDll)
    {
        try
        {
            bRet = (0 != FreeLibrary(m_hDll));
        }
        catch(...)
        {
            assert(0 && "freeLib fail");
            int a = 0;
        }
        m_hDll = NULL;
        
    }
    return bRet;
}

HMODULE CDllManager::GetHandle()
{
    return m_hDll;
}

void* CDllManager::GetProc( LPCSTR param1 )
{
	return GetProcAddress(m_hDll, param1);
}
