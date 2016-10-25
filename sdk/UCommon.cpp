#include "stdafx.h"
#include "UCommon.h"
#include "SysFilePathMgr.h"

#include <io.h>
#include "PathLight.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
//
BOOL CFileUtil::IsFileExist(CString strFilePath)
{
	// _access?unicode?????????,?????????????????CP_ACP?????,???CP_UTF8??,???-1.
	// ?????,????
	BOOL bExist = FALSE;

	int nLen = WideCharToMultiByte(CP_ACP,0,strFilePath.GetBuffer(),strFilePath.GetLength(),NULL,0,NULL,NULL);  
	char *ch = new char[nLen * sizeof(TCHAR)];  
	ZeroMemory(ch,nLen* sizeof(TCHAR));        
	int nWr = WideCharToMultiByte(CP_ACP,0,strFilePath.GetBuffer(),-1,ch,nLen,NULL,NULL);

	if (_access(ch, 0) != -1)
	{
		bExist = TRUE;
	}
	delete []ch;
	return bExist;
}

/****************************************************************************
//?????????
****************************************************************************/
bool CFileUtil::IsFileNameValid(const TCHAR* FileName)
{
	TCHAR SpecialCha[]={'/','\\','\"',':','*','?','<','>','|','\t'};
	for (int i=0; i< sizeof(SpecialCha)/sizeof(TCHAR)  ;i++)
	{
		if (NULL!=_tcschr(FileName,SpecialCha[i]))
			return false;
	}
	return true;
}

BOOL CFileUtil::EnumFolderFile(CString strPath, CArray<CEnumFileParam, CEnumFileParam&>& arrFileList)
{
	arrFileList.RemoveAll();

	CString File_name;

	CFileFind file;
	BOOL nContinue;
	nContinue = file.FindFile(strPath);
	if(!nContinue)
		return FALSE;

	while(nContinue)
	{ 
		nContinue = file.FindNextFile();
		if(file.IsDots())
			continue;
		else if(file.IsDirectory())
		{ 
			CString Cpath;
			int Index = strPath.ReverseFind('\\');
			Cpath = strPath.Left(Index);
			Cpath +=_T("\\")+file.GetFileName();
			Cpath += _T("\\*.*");
			EnumFolderFile(Cpath, arrFileList);    
		}
		else
		{
			//			AfxMessageBox(file.GetFileName());
			CString strFileName = file.GetFileName();
			CString strFilePath = file.GetFilePath();
			CEnumFileParam param;
			param.strName = strFileName;
			param.strPath = strFilePath;
			arrFileList.Add(param);
		}
	}

	return TRUE;
}

BOOL CFileUtil::EnumFolder(CString strPath, CArray<CEnumFileParam, CEnumFileParam&>& arrFileList)
{
	arrFileList.RemoveAll();

	CString File_name;

	CFileFind file;
	BOOL nContinue;
	nContinue = file.FindFile(strPath);
	if(!nContinue)
		return FALSE;

	while(nContinue)
	{ 
		nContinue = file.FindNextFile();
		if(file.IsDots())
			continue;
		else if(file.IsDirectory())
		{ 
			CString strFileName = file.GetFileName();
			CString strFilePath = file.GetFilePath();
			CEnumFileParam param;
			param.strName = strFileName;
			param.strPath = strFilePath;
			arrFileList.Add(param);
		}
	}

	return TRUE;
}

BOOL CFileUtil::CopyFile(CString& szSrcPath, CString& szDistPath)
{
	szSrcPath += '\0';
	szDistPath += '\0';

	SHFILEOPSTRUCT st;
	ZeroMemory(&st, sizeof(SHFILEOPSTRUCT));
	st.hwnd = NULL;
	st.wFunc = FO_COPY;
	st.pFrom = szSrcPath;
	st.pTo = szDistPath;
	st.fFlags = FOF_NO_UI ;
	int nRet = SHFileOperation(&st);
	if (nRet != 0)
		return FALSE;

	return TRUE;
}

BOOL CFileUtil::DeleteFile(CString& szSrcPath)
{
	szSrcPath += '\0';

	SHFILEOPSTRUCT st;
	ZeroMemory(&st, sizeof(SHFILEOPSTRUCT));
	st.hwnd = NULL;
	st.wFunc = FO_DELETE;
	st.pFrom = szSrcPath;
	st.fFlags = FOF_NO_UI;
	int nRet = SHFileOperation(&st);
	if (nRet != 0)
		return FALSE;

	return TRUE;
}

BOOL CFileUtil::Rename( CString szSrcPath, CString szDistPath )
{
//#include <Winerror.h>
#define DE_SAMEFILE 0x71
    szSrcPath += '\0';
    szDistPath += '\0';

    SHFILEOPSTRUCT st;
    ZeroMemory(&st, sizeof(SHFILEOPSTRUCT));
    st.hwnd = NULL;
    st.wFunc = FO_RENAME;
    st.pFrom = szSrcPath;
    st.pTo = szDistPath;
    st.fFlags = FOF_NO_UI;
    int nRet = SHFileOperation(&st);
    if (nRet != 0 && nRet != DE_SAMEFILE)
        return FALSE;

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
void CWebMgr::OpenURL(CString strURL)
{
	ShellExecute(NULL, _T("Open"), _T("iexplore.exe"), strURL, NULL, SW_SHOW);
}

CString CWebMgr::GetHtmlFilePath(LPCTSTR pszURL)
{
	CString strURL;

	TCHAR lpszPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL, lpszPath, MAX_PATH);
	TCHAR* pFind = _tcsrchr(lpszPath, '\\');
	if (pFind != NULL)
	{
		pFind[1] = 0;
	}

	if (_tcslen(lpszPath) != 0 && pszURL != NULL)
	{
		strURL = _T("file:///");
		_tcscat(lpszPath, pszURL);
		strURL.Append(lpszPath);
	}

	return strURL;
}

//////////////////////////////////////////////////////////////////////////
// 
CString CFolderUtil::GetExePath()
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	CString strPath =  szFilePath;
	return strPath;
}

CString CFolderUtil::GetExeFoloder()
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //?????,?????
	CString strPath =  szFilePath;
	return strPath;
}

CString CFolderUtil::GetPaUserMainFolder()
{
	CString strTempFld = CSysFilePathMgr::GetSysTempFolder();
	
	int nRefindIndex = strTempFld.ReverseFind('\\');
	if (nRefindIndex != -1)
		strTempFld = strTempFld.Left(nRefindIndex);

	strTempFld += _T("\\Unicorn\\PaintAid");

	return strTempFld;
}

CString CFolderUtil::GetPaTempFolder()
{
	CString strTempFld = CSysFilePathMgr::GetSysTempFolder();
	strTempFld += _T("\\Unicorn\\PaintAid");
	return strTempFld;
}

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	switch(uMsg)
	{
	case BFFM_INITIALIZED:    //?????
		//::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)"C:\\Program Files");   //???????? (???)
		::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);        //???????? (???,???lpData?????)

		break;
	case BFFM_SELCHANGED:    //??????,
		{
			WCHAR curr[MAX_PATH];   
			SHGetPathFromIDList((LPCITEMIDLIST)lParam,curr);   
			::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)curr);   
		}
		break;
	default:
		break;
	}

	return 0;   
}

CString CFolderUtil::BrowseFolder(HWND hOwner, CString strSetStartPath)
{
	TCHAR szDefaultDir[MAX_PATH];
	lstrcpy(szDefaultDir, strSetStartPath.GetBuffer(0));
	
	TCHAR szPath[MAX_PATH];
	BROWSEINFO br;
	ITEMIDLIST* pItem;

	br.hwndOwner = hOwner;
	br.pidlRoot = 0;
	br.pszDisplayName = 0;
	br.lpszTitle = _T("????");
	br.ulFlags = BIF_STATUSTEXT;
	br.lpfn = BrowseCallbackProc ;        //??CALLBACK??
	br.iImage = 0;
	br.lParam = long(&szDefaultDir);    //??????

	CString strSetFolder;

	pItem = SHBrowseForFolder(&br);
	if(pItem != NULL)
	{
		if(SHGetPathFromIDList(pItem,szPath) == TRUE)
		{
			strSetFolder = szPath;
		}
	}

	return strSetFolder;
}

CString CFolderUtil::BrowseFolder()
{
	TCHAR szPathName[MAX_PATH];
/*

	BROWSEINFO bi;
	bi.hwndOwner =NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPathName;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = _T("目录选择");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage =0;   //初始化入口参数bi结束
	LPITEMIDLIST pIDList =SHBrowseForFolder(&bi);//调用显示选择对话框

	CString strSetFolder;

	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, szPathName);
		//取得文件夹路径到path里
		CString strPathName =szPathName;

		strSetFolder = strPathName;
	}
	return strSetFolder;*/
    return szPathName;
}

//////////////////////////////////////////////////////////////////////////
//
bool CDirectoryUtil::CreateMultipleDirectory( tcpchar strFileFolder )
{
	// 已经存在
	if (CPathLight::IsFileExist(strFileFolder))
    {
		return true;
    }

	// hWnd为NULL，返回值为ERROR_CANCELLED
	SHCreateDirectoryEx(NULL, strFileFolder, NULL);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
BOOL CPowerBootMgr::PowerBoot(BOOL bPower, CString strExePath)
{
	LPCTSTR lpPath = (LPCTSTR)strExePath;

	CString strExeName = strExePath;
	int nFindIndex = strExeName.ReverseFind(_T('\\'));
	if (nFindIndex != -1)
		strExeName = strExeName.Right(strExePath.GetLength() - nFindIndex - 1);
	nFindIndex = strExeName.ReverseFind(_T('.'));
	if (nFindIndex != -1)
		strExeName = strExeName.Left(nFindIndex);

	HKEY hKey;
	DWORD cbData = MAX_PATH;

	CString strSubKey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\");
	if (bPower)
	{
		if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey, 0, KEY_ALL_ACCESS, &hKey))
		{
			return FALSE;
		}
		else
		{
			RegSetValueEx(hKey, strExeName, 0, REG_SZ, (CONST BYTE*)lpPath, cbData);
			return TRUE;
		}

		RegCloseKey(hKey);
	}
	else
	{
		if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey, 0, KEY_ALL_ACCESS, &hKey))
		{
			return FALSE;
		}
		else
		{
			RegDeleteValue(hKey, strExeName);
			return FALSE;
		}
	}

	return FALSE;
}