#include "stdafx.h"   //该cpp文件 在Win32 dll插件中有用到， 在 MFC中也有用到，MFC需要包含stdafx.h,所以有两个文件
#include "windows.h"
#include "time.h"
#include "FileStrOp.h"
#include <string>
#include "StrCoding.h"
#include <assert.h>
//#include <sys/stat.h>
#include "functional.h"
#include "AutoType.h"
#include <io.h>
#include <errno.h>
#include <direct.h>
#include "PathLight.h"
#include "PiTime.h"
#include "Math/mathUnit.h"
#include "PiString.h"
using std::string;
using std::wstring;

Pi_NameSpace_Using

CFileStrOp::CFileStrOp(void)
{
}

CFileStrOp::~CFileStrOp(void)
{
}

string CFileStrOp::AddDirEndingChar(const string& strPath)
{
	if (strPath.length() < 1)
	{
		return "";
	}
	string strTemp(strPath);
	
	if(0 != strcmp(&strTemp.at(strTemp.length() - 1), "\\"))
	{
		strTemp += "\\";
	}
	return strTemp;
}

wstring CFileStrOp::AddDirEndingChar(const wstring& strPath)
{
	string strTemp = AddDirEndingChar(w2s(strPath));
	return s2w(strTemp);
}

string CFileStrOp::GetPostFix(const string& fileName)
{
	string strTemp = GetDotSuffix(fileName);
	if (strTemp.length())
	{
		return strTemp.c_str() + 1;
	}
	else
	{
		return "";	
	}
	
}


string CFileStrOp::GetDotSuffix(const string& fileName)
{
	/*string::size_type uPos = fileName.find_last_of('.');
	strTemp = (uPos == string::npos)?"":fileName.substr(uPos+1);*/

	string strTemp;
	string::size_type uPos = GetPostFixPos(fileName);
	if(string::npos != uPos )
	{
		strTemp = fileName.substr(uPos);
	}
	else
	{
		strTemp = "";
	}
	return strTemp;	
}
string::size_type CFileStrOp::GetPostFixPos(const string& fileName)
{
	string::size_type uDirPos = fileName.find_last_of('\\');//路径肯定有 “\” 字符
	uDirPos = (string::npos == uDirPos ? 0:uDirPos);

	string::size_type uPos = fileName.find_last_of('.');
	if(uPos!= string::npos && uPos>uDirPos)
	{
		return uPos;	
	}
	return string::npos;
}

bool CFileStrOp::CreateDeepDir( tcpchar pDir )
{
    //从后面开始创建
    //string strDir = ExtractFileDir(pDir);
    if(!IsExist(pDir))
    {
		assert(CreateDeepDir(CPathLight::GetDirFromPath(pDir).c_str()));

        if( -1 == _tmkdir(pDir))
    	{
    		return false;
    	}
    }
    return true;
}

bool CFileStrOp::IsExist( tcpchar pFile )
{
    if(-1 == _taccess(pFile, 06)) //06-rw
    {
        //no exist or no have the given mode
        if(EACCES == errno )
        {
            // Access denied
            return false;
        }
        return false;
    }
    return true;
}

string CFileStrOp::ExtractFileDir(const string& fileName)
{
	string::size_type uPos = fileName.find_last_of('\\', fileName.length() - 1 - /* 排除最后一个字符 */1);//路径肯定有 “\” 字符
	assert(uPos !=string::npos);

	return fileName.substr(0,uPos) + "\\";
	
}

tstring CFileStrOp::GetCurrentDir()
{
    
    tchar szExePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szExePath, MAX_PATH);
    return CPathLight::GetParentPathFromPath(szExePath);
    //return CFileStrOp::ExtractFileDir(tstring(szExePath));
}

string CFileStrOp::GetCurrentExeTitle()
{
    char szExePath[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, szExePath, MAX_PATH);
    return CFileStrOp::ExtractFileTitle(szExePath);
}

string CFileStrOp::ExtractFileName(const string& fileName)
{
	string::size_type uPos = fileName.find_last_of("\\");
	if( string::npos == uPos )
	{
		return fileName;
	}
	return fileName.substr(uPos +1 );
}

string CFileStrOp::ExtractFileTitle(const string& fileName)
{
	string strTemp = ExtractFileName(fileName);
	
	return strTemp.substr(0,GetPostFixPos(strTemp));
	
}


tstring CFileStrOp::FileSizeSmartSwitch( LLONG dwSize, tcpchar szUnit /*= NULL*/)
{
	/*
		根据^分割, 总共4段, byte^kb^MB^GB^
	*/
	
	tstring strUnit(_T("0# byte^0# KB^0# MB^0# GB"));	//小数点位数
	if (szUnit)
	{
		strUnit = szUnit;
	}
	LST_STRING arrFormat = CPiString::SpilitStr(strUnit, _T("^"));
	ARR_PAIR_STR arrPairStr = CPiString::SpilitStrToPairArray(arrFormat, _T("#"));
	//ARR_STRING arrPRECISION;

	tstring strSmart;
	tstring strFormat(30, 0);
	tchar cBuf[30] = {0};
	if (dwSize < NUM_FILE_SIZE_1K )
	{
		_stprintf_s(cBuf, _countof(cBuf), _T("%I64d%s"), dwSize, arrPairStr[0].second.c_str());
		strSmart = cBuf;
	} 
	else if(dwSize < NUM_FILE_SIZE_1M)//kb
	{
		_stprintf_s(&strFormat.at(0), strFormat.size(), _T("%%.%slf%%s"), arrPairStr[1].first.c_str());
		_stprintf_s(cBuf, _countof(cBuf), strFormat.c_str(), dwSize/1024.0, arrPairStr[1].second.c_str());
		strSmart = cBuf;
	}
	else if(dwSize < NUM_FILE_SIZE_1G)
	{
		_stprintf_s(&strFormat.at(0), strFormat.size(), _T("%%.%slf%%s"), arrPairStr[2].first.c_str());
		_stprintf_s(cBuf, _countof(cBuf), strFormat.c_str(), dwSize/1024.0/1024.0, arrPairStr[2].second.c_str());
		strSmart = cBuf;
	}
	else if(dwSize < NUM_FILE_SIZE_1T)
	{
		_stprintf_s(&strFormat.at(0), strFormat.size(), _T("%%.%slf%%s"), arrPairStr[3].first.c_str());
		_stprintf_s(cBuf, _countof(cBuf), strFormat.c_str(), dwSize/1024.0/1024.0/1024.0, arrPairStr[3].second.c_str());
		strSmart = cBuf;
	}
	else
	{
		strSmart = _T("不支持的大小");
	}
	return strSmart;
}

SHFILEINFOA CFileStrOp::GetFileIcon(const CHAR* path)
{
	/*HRESULT lRet = CoInitialize(NULL);
	if (lRet != S_OK)
	{
		int a = 0;
	}*/
	SHFILEINFOA fi = {0};
	UINT	uFlag =  SHGFI_ICON |SHGFI_SMALLICON | SHGFI_SYSICONINDEX /*| SHGFI_OPENICON*/;
	DWORD	dwFileAttri = 0;
	if ('.' == *path/*后缀名关联图标*/)
	{
		uFlag |= SHGFI_ICON | SHGFI_SYSICONINDEX |  SHGFI_USEFILEATTRIBUTES;
		dwFileAttri |= FILE_ATTRIBUTE_NORMAL;
	}
	DWORD dwRet = ::SHGetFileInfoA(path , dwFileAttri, &fi, sizeof(SHFILEINFOA), 
		 uFlag);
	
	return fi;
}

struct _stat CFileStrOp::GetFileStatusInfo(const char* pFile)
{
	//stat 不支持fat, 可用GetFileTime替代
	struct _stat fileInfo = {0};
	_stat(pFile,&fileInfo);
	return fileInfo;

};

tm CFileStrOp::GetFileTime(const char* pFile, TIME_TYPE type)
{
	struct _stat fi = GetFileStatusInfo(pFile);
	tm TM;

	switch(type)
	{
		case TYPE_CREATE:
			 localtime_s(&TM,&fi.st_mtime);
			break;
		case TYPE_ACCESS:
			 localtime_s(&TM,&fi.st_mtime);
			break;
		case TYPE_MODIFY:
			 
			break;
	}
	return TM;
};

SYSTEMTIME CFileStrOp::GetFileCreateTime(const char* pFile)
{
	return CPiTime::TM2ST(&GetFileTime(pFile, TYPE_CREATE));
};

SYSTEMTIME CFileStrOp::GetFileAccessTime(const char* pFile)
{
	return CPiTime::TM2ST(&GetFileTime(pFile, TYPE_ACCESS));
};

SYSTEMTIME CFileStrOp::GetFileModifyTime(const char* pFile)
{
	return CPiTime::TM2ST(&GetFileTime(pFile, TYPE_MODIFY));
};

string CFileStrOp::GetFileCreateTimeStr(const char* pFile)
{
	return CPiTime::TimeToString(&GetFileCreateTime(pFile),"%04d/%02d/%02d %02d:%02d:%02d");
}

DWORD CFileStrOp::GetFileSize(tcpchar szFile)
{
	struct _stat fileInfo = {0};
	DWORD dwFileSize = 0;
	_tstat(szFile,&fileInfo);
	dwFileSize  = fileInfo.st_size;

	return  dwFileSize;
}

string CFileStrOp::GetNextFileName(const char* pFile)
{
    /*string strFileName;
    string strTitle = CFileStrOp::ExtractFileTitle(pFile);
    string strSuffix = CFileStrOp::GetDotSuffix(pFile);
    string::size_type nPos = strTitle.find_last_of('_');

    CAutoType at;

    string strNewPart("0");//获取分割名

    if (nPos != string::npos)
    {
        
        strNewPart = strTitle.substr(nPos + 1).c_str();
        strTitle = strTitle.substr(0, nPos);
    }
    strNewPart = at.SetData(strNewPart.c_str()) + (LLONG)1;

    return strTitle.append("_") + strNewPart + strSuffix;*/
   return "";
}