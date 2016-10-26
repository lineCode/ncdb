#include "StdAfx.h"
#include "PiDisk.h"
#include "define_gnl.h"
#include "doString.h"

#pragma comment(lib, "Version.lib")



CPiDisk::CPiDisk(void)
{
}


CPiDisk::~CPiDisk(void)
{
}

LST_STRING CPiDisk::GetValidDrive()
{
    LST_STRING lstDrive;
    const int nSize = 200;
    
    tchar szPath[nSize + 1] = {0};
    tcpchar pDrive = NULL;
    int nStrLen = 0;
    if(!GetLogicalDriveStrings(nSize, szPath))
    {
        return lstDrive;
    }

    pDrive = szPath;
    while(*pDrive)
    {
        lstDrive.push_back(pDrive);
        nStrLen = _tcslen(pDrive);
        pDrive = pDrive + nStrLen + 1;
    }
    return lstDrive;
}

MAP_STRING CPiDisk::GetDriveMapping( LST_STRING& lstDrive )
{
    MAP_STRING lstNUll;

    MAP_STRING lstMapping;
    const int nSize = 1024;
    TCHAR szMapping[nSize + 1] = {0};
    FOR_EACH_ITER(LST_STRING, lstDrive, it)
    {
		*it->rbegin() = '\0';
        //it->back() = '\0';
        DWORD dwRet = ::QueryDosDevice(it->c_str(), szMapping, nSize);
        if(!dwRet)
        {
            return lstNUll;
        }
        lstMapping.insert(make_pair(*it, szMapping));
    }

    return lstMapping;
}

bool CPiDisk::GetValidDriveMapping()
{
    LST_STRING lstDrive = CPiDisk::GetValidDrive();
    m_mapDrive = CPiDisk::GetDriveMapping(lstDrive);

    return m_mapDrive.size();
}

tstring CPiDisk::GetDosPathFromMapping( tcpchar szMapping )
{
    tstring strMp(szMapping);
    FOR_EACH_ITER(MAP_STRING, m_mapDrive, it)
    {
        size_t nPos = strMp.find(it->second);
        if( IsValidStrPos(nPos) && nPos == 0)
        {
            //ур╣╫ак
            StrReplace(strMp, it->second.c_str(), it->first.c_str(), 1);
            return strMp;
        }
    }
    
    return _T("");
}

bool CPiDisk::Init()
{
    return GetValidDriveMapping();
}


tstring CPiDisk::GetFileVersion( tcpchar path )
{
	size_t nCharLen = 100;
	tstring strVersion(nCharLen, 0);
	DWORD dwVerInfoSize = 0;
	DWORD dwVerHnd;
	VS_FIXEDFILEINFO * pFileInfo;

	dwVerInfoSize = GetFileVersionInfoSize(path, &dwVerHnd);
	if (dwVerInfoSize)
	{
		// If we were able to get the information, process it:
		HANDLE  hMem;
		LPVOID  lpvMem;
		unsigned int uInfoSize = 0;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(path, dwVerHnd, dwVerInfoSize, lpvMem);

		::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);

		int ret = GetLastError();
		WORD nProdVersion[4];

		// Product version from the FILEVERSION of the version info resource 
		nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS); 
		nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
		nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
		nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS); 

		_stprintf_s(&strVersion.at(0), nCharLen, _T("%d.%d.%d.%d"),nProdVersion[0],
			nProdVersion[1],nProdVersion[2],nProdVersion[3]);

		/*strVersion.Format(_T("%d.%d.%d.%d"),nProdVersion[0],
			nProdVersion[1],nProdVersion[2],nProdVersion[3]);*/

		GlobalUnlock(hMem);
		GlobalFree(hMem);

	}
	return strVersion.c_str();
}

