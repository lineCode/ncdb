#include "stdafx.h"
#include <fstream>
#include <shlwapi.h>
#include "PiClipboard.h"
using namespace DuiLib;
/////////////////////////////////////////////////////////////////////////////////////
//
//
CPiClipboard *CPiClipboard::s_pInstance = NULL;
CPiClipboard *CPiClipboard::getInstance()
{
	if (NULL == s_pInstance)
	{
		s_pInstance = new CPiClipboard();
	}
	return s_pInstance;
}
CPiClipboard::CPiClipboard()
{
}

CPiClipboard::~CPiClipboard()
{
}

BOOL CPiClipboard::CopyFilesToClipboard(std::vector<CDuiString> const & vecFiles)
{
	UINT  uDropEffect = RegisterClipboardFormat(_T("NAYCENTFILE"));
	HGLOBAL hGblEffect = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(DWORD));
	DWORD *pDWDropEffect = (DWORD*)GlobalLock(hGblEffect);
	*pDWDropEffect = DROPEFFECT_COPY;
	GlobalUnlock(hGblEffect);

	DROPFILES dropFiles;
	dropFiles.pFiles = sizeof(DROPFILES);
	dropFiles.pt.x = 0;
	dropFiles.pt.y = 0;
	dropFiles.fNC = FALSE;
	dropFiles.fWide = TRUE;

	UINT uFileListLen = 0;
	for (std::vector<CDuiString>::const_iterator it = vecFiles.begin(); it != vecFiles.end(); ++it)
	{
		uFileListLen += (it->GetLength() + 1);//尾部一个\0结束符
	}

	UINT uGblLen = sizeof(DROPFILES) + sizeof(TCHAR)* (uFileListLen + 1);
	HGLOBAL hGblFiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, uGblLen);
	BYTE *  pData = (BYTE*)GlobalLock(hGblFiles);
	memcpy(pData, (LPVOID)(&dropFiles), sizeof(DROPFILES));

	BYTE *pFileList = pData + sizeof(DROPFILES);

	for (std::vector<CDuiString>::const_iterator it = vecFiles.begin(); it != vecFiles.end(); ++it)
	{
		UINT uLen = (it->GetLength() + 1) * sizeof(TCHAR);
		memcpy(pFileList, (BYTE*)(LPCTSTR)(*it), uLen);
		pFileList += uLen;
	}

	GlobalUnlock(hGblFiles);

	if (OpenClipboard(NULL))
	{
		EmptyClipboard();
		SetClipboardData(CF_HDROP, hGblFiles);
		SetClipboardData(uDropEffect, hGblEffect);
		CloseClipboard();
		return TRUE;
	}
	else
	{
		::GlobalFree(hGblEffect);
		::GlobalFree(hGblFiles);
	}
	return FALSE;
}

std::vector<CDuiString> CPiClipboard::GetFilesFromClipboard()
{
	std::vector<CDuiString> vecPath;
	if (OpenClipboard(NULL))
	{
		HDROP hDrop = HDROP(GetClipboardData(CF_HDROP));
		if (hDrop)
		{
			WORD nFileCount = ::DragQueryFile(hDrop, -1, NULL, 0);
			TCHAR szFileName[_MAX_PATH] = _T("");
			DWORD dwAttribute;

			// 获取剪切板中的文件路径
			for (UINT i = 0; i < nFileCount; i++)
			{
				::DragQueryFile(hDrop, i, szFileName, sizeof(szFileName));
				dwAttribute = ::GetFileAttributes(szFileName);

				vecPath.push_back(szFileName);
			}
		}
		CloseClipboard();
	}
	return vecPath;
}

CDuiString CPiClipboard::CreateGUID()
{
	GUID guid;
	wchar_t guidString[39];
	if (CoCreateGuid(&guid))
	{
		return _T("");
	}
	StringFromGUID2(guid, guidString, 39);
	return guidString;
}

bool CPiClipboard::CreateTempFile(LPCTSTR filePath)
{
	ofstream fout(filePath);
	if (fout) { // 如果创建成功
		SetFileAttributes(filePath, FILE_ATTRIBUTE_HIDDEN);
		return true;
	}
	return false;
}

std::vector<CDuiString> CPiClipboard::GetLogicalDriver()
{
	std::vector<CDuiString> vecDrivers;
	DWORD dwNeed;
	DWORD dwBufferLength;
	TCHAR lpBuffer[128];
	LPTSTR lpDisk;
	UINT uType;

	dwNeed = 128;
	do
	{
		dwBufferLength = dwNeed;
		dwNeed = GetLogicalDriveStrings(dwBufferLength, lpBuffer);
		if (dwNeed == 0)
		{
			return vecDrivers;
		}
	} while (dwNeed>dwBufferLength);
	for (lpDisk = lpBuffer; *lpDisk; lpDisk += 4){
		vecDrivers.push_back(lpDisk);
	}
	return vecDrivers;
}
