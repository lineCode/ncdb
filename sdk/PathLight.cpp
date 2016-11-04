#include "StdAfx.h"
#include "PathLight.h"
#include "functional.h"
#include <assert.h>
#include <shlwapi.h>
#include "PiTime.h"
#include <sys/stat.h>
#include <shlobj.h>
#include "PiString.h"
#include "FileOpr.h"
#include "MD5.h"
#include "Math/mathUnit.h"

Pi_NameSpace_Using
CPathLight::CPathLight()
{

}


CPathLight::CPathLight( tcpchar szPath)
{
    m_strPath = szPath;
    SyncCache();
}

CPathLight::CPathLight( const tstring& strPath )
    
{
    m_strPath = strPath;
    SyncCache();
    
}

CPathLight::~CPathLight(void)
{
}

bool CPathLight::IsFileExist( tcpchar szPath )
{
    return PathFileExists(szPath) == TRUE;
}

bool CPathLight::IsFileExist()
{
    return CPathLight::IsFileExist(m_strPath.c_str());
}

tstring CPathLight::GetParentPathFromPath( tcpchar szPath, bool bSlash /*= false*/)
{
	tstring strTemp(szPath);
	//去掉结尾斜杠
	::PathRemoveBackslash(&strTemp.at(0));
	strTemp = GetDirFromPath(strTemp.c_str());
	if (bSlash)
	{
		strTemp += _T("\\");
	}
	return strTemp;
}

tstring CPathLight::GetParentPathFromPath(bool bSlash /*= false*/)
{
	return GetParentPathFromPath(m_strPath.c_str(), bSlash);
}

tstring CPathLight::GetParentDirName(tcpchar szPath)
{
    return GetFileNameFromPath(GetParentPathFromPath(szPath).c_str());
}

tstring CPathLight::AddSubSession( tcpchar szTitle )
{
    //确定尾部有斜杠
    m_strPath.resize(MAX_PATH);
    
    PathAddBackslash(&m_strPath.at(0));
    
    tcpchar szTemp = m_strPath.c_str();
    m_strPath = szTemp;
    m_strPath += szTitle;
    return m_strPath;
}

tstring CPathLight::AddSubSession( const tstring& szTitle )
{
    return AddSubSession(szTitle.c_str());
}

tstring CPathLight::GetDirName( tcpchar szPath )
{
    tstring strPath(szPath);
    PathRemoveBackslash(&strPath.at(0));
    strPath = GetFileNameFromPath(strPath.c_str());
    
    return strPath;
}

const tstring& CPathLight::GetPath()
{
    return m_strPath;
}

bool CPathLight::SetPath( const tstring& strPath )
{
    m_strPath = strPath;
    SyncCache();
    return true;
}

bool CPathLight::GoParent()
{
    m_strPos = this->GetParentPathFromPath(m_strPos.c_str());
    return true;
}

bool CPathLight::ModifyCurDirName( const tstring& strNameNew )
{
    //获取剩下右边部分, 用来合成数据,3段, 左边, 当前目录, 右边
    tstring strTemp;
    strTemp = m_strPath.substr(m_strPos.length(), m_strPath.length() - m_strPos.length()); 
    m_strPath = this->GetParentPathFromPath(m_strPos.c_str());
    AddSubSession(strNameNew);
    m_strPath += strTemp;
    return true;
}

bool CPathLight::SyncCache()
{
    m_strPos = m_strPath;
    return true;
}

tstring CPathLight::GetCurPos()
{
    return m_strPos;
}

bool CPathLight::ModifyFileName( const tstring& strNewName )
{
    //tcpchar szPath = m_strPath.c_str();
    tstring strExt = this->GetExt();
    
    m_strPath = this->GetParentPathFromPath(m_strPath.c_str());
    AddSubSession(strNewName);
    m_strPath += strExt;
    return true;
}

tstring CPathLight::GetExt()
{
    tchar szExt[_MAX_FNAME] = {0};
	_tsplitpath_s(m_strPath.c_str(), NULL, 0, NULL, 0, NULL, 0, szExt, _MAX_FNAME);
    return szExt;
}

tstring CPathLight::GetExt( StringCRef strPath )
{
	tchar szExt[_MAX_FNAME] = {0};
	_tsplitpath_s(strPath.c_str(), NULL, 0, NULL, 0, NULL, 0, szExt, _MAX_FNAME);
	//_tsplitpath(strPath.c_str(), NULL, NULL, NULL, szExt);
	return szExt;
}

tstring CPathLight::GetFileTitleFromPath( tcpchar szPath )
{
    tchar szTitle[_MAX_FNAME] = {0};
	_tsplitpath_s(szPath, NULL, 0, NULL, 0, szTitle, _MAX_FNAME, NULL, 0);
    return szTitle;
}

CPathLight::operator tstring() const
{
    return m_strPath;
}

bool CPathLight::IsValidPath( tcpchar szPath )
{
    return true;
}



LST_STRING CPathLight::FileEnumeration( tcpchar lpPath, bool bRecursion, bool bEnumFiles)
{
	LST_STRING lstS;
	FileEnumeration(lpPath, bRecursion, bEnumFiles, false, CB_EnumerateFunc, &lstS);
	return lstS;
}

bool CPathLight::FileEnumeration( tcpchar lpPath, bool bRecursion, bool bEnumFiles, bool bFindRecursionDir, EnumerateFunc pFunc, void* pUserData)
{

    static bool s_bUserBreak = false;
    
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

	::PathRemoveBackslash(&strPath.at(0));
	tstring strFindFileFormat = ::PathFindFileName(strPath.c_str());
	tstring strDir(strPath);
	PathRemoveFileSpec(&strDir.at(0));
	strDir = strDir.c_str();

	tstring strFindName(strPath);
	if (bFindRecursionDir)
	{
		strFindName = strDir + _T("\\*");
	}

	bool bFinish = false;
    WIN32_FIND_DATA fd;
    HANDLE hFindFile = ::FindFirstFile(strFindName.c_str(), &fd);
    if(hFindFile == INVALID_HANDLE_VALUE)
    {
        bFinish = true;
    }
    //tstring strFilePath;
	tstring strNewPath;
	bool bUserReture = true;
    bool bIsDirectory;
	bool bHasRed = false;
    while(!bFinish)
    {
		//如果是.或.., 则继续下一个查找
		if( (_tcscmp(fd.cFileName, _T(".")) == 0 || _tcscmp(fd.cFileName, _T("..")) == 0) )
		{		
			bFinish = (::FindNextFile(hFindFile, &fd) == FALSE);
			continue;
		}
		
		OutInfo((strDir + _T("\\") + fd.cFileName).c_str());
		strNewPath = strDir;
		strNewPath += '\\';
		strNewPath += fd.cFileName;

        bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

        if(pFunc && bEnumFiles == !bIsDirectory	//判断是否为要查找的类型
			&& !bFindRecursionDir ) //如果是文件, 不能是在查找子目录的状态
        {
			
            //bUserReture = pFunc(strFilePath.c_str(), pUserData);
			bUserReture = pFunc(strNewPath.c_str(), pUserData);
            if(bUserReture == false)
            {
                s_bUserBreak = true;    //用户想退出, 不查找文件了
                ::FindClose(hFindFile); 
                return false;
            }
        }
		
        if(bIsDirectory && bRecursion) //是子目录 并且要递归查找
        {
			bHasRed = true;
			FileEnumeration( (strNewPath + _T("\\") + strFindFileFormat).c_str(), bRecursion, bEnumFiles, false, pFunc, pUserData);
        }

        bFinish = (FindNextFile(hFindFile, &fd) == FALSE);
	}

    ::FindClose(hFindFile);

	//-------------------------------------------------------------------------
    //
    if (bRecursion
		&& (!bFindRecursionDir )
		&& !bHasRed
		/*&& bEnumFiles
		&& */)
    {
		FileEnumeration(lpPath, bRecursion, bEnumFiles, true, pFunc, pUserData);
    }

	return true;
}

bool CPathLight::FileEnumeration( tcpchar lpPath, bool bRecursion, bool bEnumFiles, LST_STRING& lstS )
{
	return FileEnumeration(lpPath, bRecursion, bEnumFiles, false, CB_EnumerateFunc, &lstS);
}

bool CPathLight::CB_EnumerateFunc( tcpchar lpFileOrPath, void* pUserData )
{
    LST_STRING* pLst = reinterpret_cast<LST_STRING*>(pUserData);
    pLst->push_back(lpFileOrPath);
    return true;
}

tstring CPathLight::AddSub( tcpchar szDir, tcpchar szFile )
{
    CPathLight path(szDir);
    //TODO:判断是否是合法的guid格式
    return path.AddSubSession(szFile);  //guid目录
}

tstring CPathLight::GetExpandDir( tcpchar szVariant )
{
    tstring strTempFld;
    strTempFld.resize(MAX_PATH + 1);
    //TCHAR szBuffer[MAX_PATH];
    ExpandEnvironmentStrings(szVariant, &strTempFld.at(0), MAX_PATH);
    return strTempFld.c_str();
}

CPathLight::operator tcpchar()
{
	return m_strPath.c_str();
}

CPathLight CPathLight::operator+( tcpchar szPath )
{
	return operator + (tstring(szPath));
	/*CPathLight  path(*this);
	path.AddSubSession(tstring(szPath));
	return path;*/
}

CPathLight CPathLight::operator+( int nValue )
{
	TCHAR szBuf[20] = {0};
	_itot_s(nValue, szBuf, _countof(szBuf), 10);
	return operator + (szBuf);
}

CPathLight CPathLight::operator+( StringCRef szPath )
{
	CPathLight  path(*this);
	path.AddSubSession(szPath);
	return path;
}

CPathLight& CPathLight::operator+=( tcpchar szPath )
{
	AddSubSession(tstring(szPath));
	return *this;
}

CPathLight& CPathLight::operator+=( StringCRef szPath )
{
	AddSubSession(szPath);
	return *this;
}

CPathLight& CPathLight::operator+=( int nValue )
{
	return this->operator + (nValue);
}



tstring CPathLight::GetFileNameFromPath( tcpchar szPath )
{
	if (!szPath || !*szPath)
	{
		return _T("");
	}
	tstring strTemp(szPath);
	//去掉结尾斜杠
	::PathRemoveBackslash(&strTemp.at(0));
	return ::PathFindFileName(strTemp.c_str());
}

tstring CPathLight::GetFileName()
{
	return GetFileNameFromPath(m_strPath.c_str());
}

LLONG CPathLight::GetFileSize( tcpchar szPath )
{
	struct __stat64 fileInfo = {0};
	DWORD dwFileSize = 0;
	_tstat64(szPath, &fileInfo);
	dwFileSize  = fileInfo.st_size;

	return  dwFileSize;
}

tstring CPathLight::GetSuffixName(StringCRef strPath)
{
	tstring strSuffix(GetExt(strPath));
	if (strSuffix.empty())
	{
		return _T("");
	}
	return strSuffix.substr(1);
}


tstring CPathLight::GetSelfModuleFolder()
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //删除文件名，只获得路径
	tstring strPath =  szFilePath;
	return strPath;
}


tstring CPathLight::GetSelfModulePath()
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	tstring strPath =  szFilePath;
	return strPath;
}


tstring CPathLight::GetDirFromPath( tcpchar szPath )
{
	tstring strTemp(szPath);
	if(!PathRemoveFileSpec(&strTemp.at(0)))
	{
		strTemp = szPath;
	}
	tcpchar szTemp = &strTemp.at(0);
	strTemp = szTemp;
	return strTemp;
}

bool CPathLight::CreateMultipleDirectory( tcpchar strFileFolder )
{
	if (CPathLight::IsFileExist(strFileFolder))
	{
		return true;
	}

	// hWnd为NULL，返回值为ERROR_CANCELLED
	SHCreateDirectoryEx(NULL, strFileFolder, NULL);
	return true;
}

CPathLight& CPathLight::operator=( tcpchar szPath )
{
	m_strPath = szPath;
	SyncCache();
	return *this;
}

bool CPathLight::RenameTitle( StringCRef nameNew )
{
	tchar szExt[_MAX_EXT] = {0};
	_tsplitpath_s(m_strPath.c_str(), NULL, 0, NULL, 0, NULL, 0, szExt, _MAX_EXT);

	tstring strNew = GetDirFromPath(m_strPath.c_str());
	strNew += _T("\\");
	strNew += nameNew;
	strNew += szExt;
	bool bRet = (_trename(m_strPath.c_str(), strNew.c_str()) == 0);
	m_strPath = strNew;
	return bRet;
}

bool CPathLight::RenameName( StringCRef nameNew )
{
	if (m_strPath.empty())
	{
		return false;
	}
	if(CPiString(GetFileNameFromPath(m_strPath.c_str())).EqualNoCase(nameNew.c_str()) )
	{
		return true;
	}
	tstring strNew = GetDirFromPath(m_strPath.c_str());
	strNew += _T("\\");
	strNew += nameNew;
	if (IsFileExist(strNew.c_str()))
	{
		return true;
	}
	bool bRet = (_trename(m_strPath.c_str(), strNew.c_str()) == 0);
	m_strPath = strNew;
	return bRet;
}

tstring CPathLight::GetMD5()
{
	tstring strMD5;
	CFileOpr file;
	if(!file.Open(m_strPath))
	{
		return strMD5;
	}
	unsigned char* pBuf = NULL;
	UNLONG nMax = NUM_FILE_SIZE_1M * 200;
	if (file.GetFileSize() > nMax)
	{
		return strMD5;
	}
	
	UNLONG nRead = 0;
	if(!file.Read((void*&)pBuf, file.GetFileSize(), &nRead))
	{

	}
	strMD5.resize(100, 0);
	MD5String(pBuf, nRead, &strMD5.at(0));
	delete []pBuf;
	return strMD5.c_str();
}

tstring CPathLight::SetSuffixName( tcpchar szSuffix )
{
	tchar szFileName[_MAX_EXT] = {0};
	_tsplitpath_s(m_strPath.c_str(), NULL, 0, NULL, 0, szFileName, _MAX_EXT, NULL, 0);

	tstring strNew = GetDirFromPath(m_strPath.c_str());
	strNew += _T("\\");
	strNew += szFileName;
	strNew += _T(".");
	strNew += szSuffix;
	bool bRet = (_trename(m_strPath.c_str(), strNew.c_str()) == 0);
	m_strPath = strNew;
	return m_strPath;
}
