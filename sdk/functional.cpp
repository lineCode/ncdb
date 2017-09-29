#include "stdafx.h"

#include "doString.h"   

#include <tchar.h>
#include "functional.h"


#include "AutoType.h"  

#include "Math/PiMathUnit.h"
#include "Tlhelp32.h"
#include <shlwapi.h>
#include <assert.h>
#include <Psapi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ResCommon.h"
#include <atlenc.h>
#include <wininet.h>
#include "define_gnl.h"
#include "StrCoding.h"
#include "PathLight.h"
#include "FileOpr.h"
#pragma comment(lib, "Psapi.lib")

#pragma comment(lib, "shlwapi.lib")

//guid
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "Wininet.lib")



Pi_NameSpace_Using



string  GetLocalDateTime(const char* cFormat)
{
	//time();
	SYSTEMTIME st = {0};
	::GetLocalTime(&st);
	char cBuf[70] = {0};
	sprintf_s(cBuf, sizeof(cBuf), cFormat,
		st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute, st.wSecond);
	return cBuf;
}



void MemToString(string& strDist, void* pMem, UNINT nSize)
{
    strDist.resize(nSize + 1);
    memcpy(&strDist.at(0), pMem, nSize);
}



double IntResolveToFloat(int Num, UNCHAR decimal)
{
	//
	// a/10  = a/(8+2) = a << 
	double fRet = Num * 1.0;
	while(decimal--)
	{
		fRet = fRet/10.00f;
	}
	
	
	return fRet;
}

wstring GetCurrentDirIniName()
{
    wchar_t cBuf[300] = {0};
    GetCurrentDirectoryW(_countof(cBuf), cBuf);
    wstring strPath(cBuf);
    strPath += L"/";
    strPath += L"config.ini";
    return strPath;
}



string MemToHex(byte* pMem, UNLONG dwSize)
{
	char* cHex = new char[dwSize * 2 + 1];
	memset(cHex, 0, dwSize * 2 + 1);
	string strHex;
	byte byVal = 0;
	UNLONG dwPos = 0;
	for (UNLONG i = 0;i<dwSize;++i)
	{
		byVal = *(pMem + i) >> 4 ;//取得高4位的值
		if (byVal >= 0 && byVal <= 9)
		{
			*(cHex + dwPos) = byVal + '0';
		}
		else if (byVal >= 10 && byVal <= 15)
		{
			*(cHex + dwPos) = byVal - 0xA + 'A';
		}
		++dwPos;


		byVal = *(pMem + i) & 15 ;//取得低4位的值
		if (byVal >= 0 && byVal <= 9)
		{
			*(cHex + dwPos) = byVal + '0';
		}
		else if (byVal >= 10 && byVal <= 15)
		{
			*(cHex + dwPos) = byVal - 0xA + 'A';
		}
		++dwPos;
	}
    string strRet(cHex);
    delete []cHex;
	return strRet;
}


bool IsBigEndian()
{
    short i = 1;
    const byte* pLastPos = (byte*)(&i) + 1;
    return *pLastPos == 1;
};

void Delay(UINT nMilliSecond)
{
    vector<int> vtTimePass;

    DWORD dwBegin = GetTickCount();
    DWORD dwArr[10] = {0};
    DWORD dwEnd = dwBegin + nMilliSecond;
    DWORD dwCur = GetTickCount();

    while(dwCur < dwEnd )
    {
        dwCur = GetTickCount();
        Sleep(1);
        vtTimePass.push_back(dwCur);
    }
}



bool DecideInHex(const string strSource)
{
    UNLONG i = 0;
    for(;i < strSource.size();++i)
    {
        if(!IsHex(strSource[i]))
        {
            return false;
        }
    }
    return true;
}

bool IsHex(char cSource)
{
    if( ('0' <= cSource && cSource <= '9')
        || ('A' <= cSource && cSource <= 'F') ) 
    {
        //在16进制内
        return true;
    }
    else
    {
        //非16进制内
        return false;
    }
}



tstring GetSelfModuleName()
{
    tchar szBuf [ MAX_PATH + 1] = {0};
    if(GetModuleBaseName(::GetCurrentProcess(), NULL, szBuf, MAX_PATH))
    {
        return szBuf;
    }
    return _T("");
}

void* GetFileData( tcpchar szfilePath )
{
	char* szBuf = NULL;
	tstring strData;
	CFileOpr file;
	if(!file.Open(szfilePath))
	{
		return szBuf;
	}

	/*CFile file;
	if(!file.Open(szfilePath, CFile::modeRead))
	{
		return szBuf;
	}*/
	const DWORD dwFileSizeLimit = 1024 * 1024 * 20;
	size_t nSize = static_cast<size_t>(file.GetFileSize());
	if (nSize > dwFileSizeLimit)
	{
		return szBuf;
	}
	szBuf = new char[nSize + 1];
	memset(szBuf, 0, nSize + 1);

	file.ReadBuf(szBuf, nSize);
	return szBuf;
/*
#ifndef _CONSOLE
    CFile file;
    CFileException fileExct;
    if(!file.Open(szfilePath, CFile::modeRead | CFile::shareDenyWrite, &fileExct))
    {
        OutInfo(_T("Open file fail:"), szfilePath);
        return false;
    }
    UNINT nSizeTotal = file.GetLength() + 1;
    NewMemAndInit(pBuf, nSizeTotal);
    / *char* pBuf = new char[nSizeTotal];
    memset(pBuf, 0, nSizeTotal);* /
    file.Read(pBuf, file.GetLength());
    return pBuf;
#else
    return NULL;
#endif*/
	return NULL;

}




tstring GetSysTempFolder()
{
    TCHAR szBuffer[MAX_PATH + 1] = {0};
    ::ExpandEnvironmentStrings(_T("%TEMP%"), szBuffer, MAX_PATH);
    return szBuffer;
}

/************************************************************************
    进程, 线程等内核相关
************************************************************************/


bool IsProcessExist( tcpchar szProcessName )
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


bool FileEnumeration( tcpchar lpPath, bool bRecursion, bool bEnumFiles, EnumerateFunc pFunc, void* pUserData )
{

    static bool s_bUserBreak = false;
    try{
        //-------------------------------------------------------------------------
        if(s_bUserBreak)
        {
            return false;
        }
        int len = _tcslen(lpPath);
        if(lpPath == NULL || len<= 0)
        {
            return false;
        }

        tstring strPath(lpPath);

        if (*strPath.rbegin() != '\\')
        {
            strPath += '\\';    //追加"\" 标识
        }

        strPath += _T("*");

        WIN32_FIND_DATA fd;
        HANDLE hFindFile = ::FindFirstFile(strPath.c_str(), &fd);
        if(hFindFile == INVALID_HANDLE_VALUE)
        {
            ::FindClose(hFindFile);
            return false;
        }
        tstring strFilePath;
        bool bUserReture = TRUE;
        bool bIsDirectory;
        bool bFinish = FALSE;

        while(!bFinish)
        {
            strFilePath = lpPath;
            if (*strFilePath.rbegin() != '\\')
            {
                strFilePath += '\\';
            }
            strFilePath += fd.cFileName;

            bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

            //如果是.或.., 则继续下一个查找
            if( bIsDirectory
                && (_tcscmp(fd.cFileName, _T(".")) == 0 || _tcscmp(fd.cFileName, _T("..")) == 0)) 
            {		
                bFinish = (::FindNextFile(hFindFile, &fd) == FALSE);
                continue;
            }

            if(pFunc && bEnumFiles == !bIsDirectory) //判断是否为要查找的类型
            {
                bUserReture = pFunc(strFilePath.c_str(), pUserData);
                if(bUserReture == false)
                {
                    s_bUserBreak = true;    //用户想退出, 不查找文件了
                    ::FindClose(hFindFile); 
                    return false;
                }
            }

            if(bIsDirectory && bRecursion) //是子目录 并且要递归查找
            {
                FileEnumeration(strFilePath.c_str(), bRecursion, bEnumFiles, pFunc, pUserData);
            }

            bFinish = (FindNextFile(hFindFile, &fd) == FALSE);
        }

        ::FindClose(hFindFile);

        //-------------------------------------------------------------------------
    }
    catch(...)
    {
        assert(0); 
        return false; 
    }
    return true;
}

bool FileEnumeration( tcpchar lpPath, bool bRecursion, bool bEnumFiles, LST_STRING& lstFile)
{
    return FileEnumeration(lpPath, bRecursion, bEnumFiles, CB_EnumerateFunc, &lstFile);
}
void OutInfo( tcpchar sz1)
{
	OutInfo(sz1, _T(""));
    
}


void OutInfo( tcpchar sz1, tcpchar sz2 )
{
#ifdef M_DEBUG_INFO
    tstring str(sz1);
    str += sz2;
    str += _T("\n");
    OutputDebugString(str.c_str());
#endif
}

void OutInfo( tcpchar sz1, __int64 nValue )
{
    tchar cBuf[50] = {0};
    _i64tot_s(nValue, cBuf, _countof(cBuf), 10);
    //CAutoType at(nValue);
    OutInfo(sz1, (tcpchar)cBuf);
}

void OutInfo( tcpchar sz1, StringCRef sz2 )
{
	OutInfo(sz1, sz2.c_str());
}

TimeType GetFileTimeModify( tcpchar szPath )
{
    //struct __stat64
    struct __stat64 tStat = {0};
    if(_tstat64(szPath, &tStat))
    {
        //获取失败
        return 0;
    }
    return tStat.st_mtime;
}

bool CB_EnumerateFunc( tcpchar lpFileOrPath, void* pUserData )
{
    LST_STRING* pLst = reinterpret_cast<LST_STRING*>(pUserData);
    pLst->push_back(lpFileOrPath);
    return true;
}



bool DecodeUrl( tstring& strTime )
{
    TCHAR szUrl[256]    = {0};
    TCHAR szType[20]    = {0};
    TCHAR szHost[256]   = {0};
    TCHAR szBufUrl[256] = {0};

    URL_COMPONENTS uc = {0};
    uc.dwStructSize = sizeof(URL_COMPONENTS);
    uc.nPort                    = 1;
    uc.lpszUrlPath              = szUrl;
    uc.dwUrlPathLength          = _countof(szUrl);

    uc.dwHostNameLength         = _countof(szHost);
    uc.lpszHostName             = szHost;

    uc.dwSchemeLength           = _countof(szType);
    uc.lpszScheme               = szType;
    if(!InternetCrackUrl(strTime.c_str(), strTime.length(), ICU_ESCAPE, &uc))
    {
        return false;
    }

    return true;
}
tstring MakeGUID()
{
    GUID guid;
    CoCreateGuid(&guid);
    RPC_WSTR szGuid = {0};
    UuidToStringW(&guid, &szGuid);
    return (tpchar)szGuid;
}


/*
LST_STRING SpilitStr( const tstring& strSrc, tcpchar szSpilit )
{
    LST_STRING lstStr;
    if (strSrc.empty())
    {
        return lstStr;
    }

    size_t nPosBegin = 0;
    size_t nPos = strSrc.find(szSpilit);
    size_t nCount = -1;
    while(IsValidStrPos(nPos))
    {
        lstStr.push_back(strSrc.substr(nPosBegin, nPos - nPosBegin));
        nPosBegin = NextPos(nPos);
        nPos = strSrc.find(szSpilit, nPosBegin);
    }
    lstStr.push_back(strSrc.substr(nPosBegin, nPos));
    return lstStr;
}*/

/*
PAIR_STRING SpilitStrTwoPart( const tstring& strSrc, tcpchar szSpilit )
{
	PAIR_STRING pairS;
	if (strSrc.empty())
	{
		return pairS;
	}

	size_t nPosBegin = 0;
	size_t nPos = strSrc.find(szSpilit);
	size_t nCount = -1;
	if(IsValidStrPos(nPos))
	{
		pairS.first = strSrc.substr(nPosBegin, nPos - nPosBegin);
		nPosBegin = NextPos(nPos);
	}
	pairS.second = strSrc.substr(nPosBegin);
	return pairS;
}*/

/*
ARR_STRING SpilitStrToArray( const tstring& strSrc, tcpchar szSpilit )
{
	ARR_STRING lstStr;
    if (strSrc.empty())
    {
        return lstStr;
    }

    size_t nPosBegin = 0;
    size_t nPos = strSrc.find(szSpilit);
    size_t nCount = -1;
    while(IsValidStrPos(nPos))
    {
        lstStr.push_back(strSrc.substr(nPosBegin, nPos - nPosBegin));
        nPosBegin = NextPos(nPos);
        nPos = strSrc.find(szSpilit, nPosBegin);
    }
    lstStr.push_back(strSrc.substr(nPosBegin, nPos));
    / *if(!IsValidStrPos(nPos))
    strSrc* /
    return lstStr;
}*/

/*
MAP_STRING SpilitStrMap( const LST_STRING& lstSrc, tcpchar szSpilit )
{
    MAP_STRING mStr;
    FOR_EACH_ITER_CONST(LST_STRING, lstSrc, it)
    {
        //根据分隔符, 分成左右两部分
        size_t nPos = it->find(szSpilit);
        
        mStr.insert( make_pair(it->substr(0, nPos), it->substr(NextPos(nPos))) );
        //it->substr(NextPos(nPos));
    }
    return mStr;
}*/

/*
ARR_PAIR_STR SpilitStrToPairArray( const LST_STRING& lstSrc, tcpchar szSpilit )
{
	ARR_PAIR_STR mStr;
	FOR_EACH_ITER_CONST(LST_STRING, lstSrc, it)
	{
		//根据分隔符, 分成左右两部分
		size_t nPos = it->find(szSpilit);

		mStr.push_back( make_pair(it->substr(0, nPos), it->substr(NextPos(nPos))) );
		//it->substr(NextPos(nPos));
	}
	return mStr;
}*/

tstring GetSelfModuleTitle()
{
	
	return CPathLight::GetFileTitleFromPath(GetSelfModuleName().c_str());
}

POINT Client2Screen( HWND hwnd, long nlParam )
{
	POINTS ptPos = MAKEPOINTS(nlParam);
	//Client2Screen(m_hWnd)
	POINT ptTemp;
	ptTemp.x = ptPos.x;
	ptTemp.y = ptPos.y;

	::ClientToScreen(hwnd, (LPPOINT)&ptTemp);
	return ptTemp;
}

/*
POINT Client2Screen( HWND hwnd, CPoint nlParam )
{
	POINT ptTemp;
	ptTemp.x = nlParam.x;
	ptTemp.y = nlParam.y;

	::ClientToScreen(hwnd, (LPPOINT)&ptTemp);
	return ptTemp;
}*/


bool MoveInRect(const RECT& rtBig, RECT& rtSrc, const POINT& ptSpacing )
{
	RECT rtBigTemp = rtBig;
	InflateRect(&rtBigTemp, ptSpacing.x, ptSpacing.y);
	int nOffsetX = (rtSrc.left < rtBigTemp.left ? rtBigTemp.left - rtSrc.left 
		: ( rtSrc.right > rtBigTemp.right ? rtBigTemp.right - rtSrc.right: 0) );
	int nOffsetY = (rtSrc.top < rtBigTemp.top ? rtBigTemp.top - rtSrc.top 
		: ( rtSrc.bottom > rtBigTemp.bottom ? rtBigTemp.bottom - rtSrc.bottom: 0) );

	OffsetRect(&rtSrc, nOffsetX, nOffsetY);
	return true;
}

tstring GetFileDataStr( tcpchar szfilePath, bool bUtf8 /*= false*/ )
{
	tstring strData;
	CFileOpr file;
	if(!file.Open(szfilePath))
	{
		return strData;
	}
	const DWORD dwFileSizeLimit = 1024 * 1024 * 20;
	size_t nSize = static_cast<size_t>(file.GetFileSize());
	if (nSize > dwFileSizeLimit)
	{
		return strData;
	}
	char* szBuf = new char[nSize + 1];
	memset(szBuf, 0, nSize + 1);

	file.ReadBuf(szBuf, nSize);
	if (bUtf8)
	{
		strData = CStrCoding::Utf8ToTStr(szBuf);
	}
	else
	{
		strData = ps2t(szBuf);
	}
	delete [] szBuf;
	return strData;
}
