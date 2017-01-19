// DectoryWatch.cpp
#include "stdafx.h"
#include <tchar.h>
#include "DirectoryWatch.h"
#include "Drag\LogSystemDll.h"

#include <process.h>
#include <strsafe.h>
#include <Shlwapi.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Shlwapi.lib")

#define MAX_BUFFER_SIZE	(1024)

typedef enum _MyMSG
{
	MSG_SUCCESS = (WM_USER)+0x100,
	MSG_STARTWATCH,
	MSG_STOPWATCH,
	MSG_EXIT
};

CDirectoryWatch::CDirectoryWatch()
	: m_dwMSGTID(0)
	, m_hMSG(NULL)
{
	m_hMutex = CreateMutexW(NULL, FALSE, NULL);

	m_hMSG = (HANDLE)_beginthreadex(NULL, NULL, MSGThred, this, NULL, (unsigned int *)&m_dwMSGTID);
	if (NULL == m_hMSG)
		LogSystem::instance()->WriteLogToFileMsg(0, _T("<ERR> CDirectoryWatch()->Create MSGThred fail!"));
		//printf("<ERR> CDirectoryWatch()->Create MSGThred fail! ErrorCode:0x%X\r\n", GetLastError());

	printf("<DBG> CDirectoryWatch() success! Open the MSGThred...\r\n");
}


CDirectoryWatch::~CDirectoryWatch()
{
	// 释放资源
	if (!m_vWatchList.empty())
	{
		std::list<WATCH_PARAM*>::iterator iBegin = m_vWatchList.begin();
		std::list<WATCH_PARAM*>::iterator iEnd = m_vWatchList.end();
		while (iBegin != iEnd)
		{
			StopWatch((*iBegin++)->wzPath); // 停止监控
		}
	}

	PostThreadMessageW(m_dwMSGTID, MSG_EXIT, NULL, NULL); // 退出 MSGThred 线程
	WaitForSingleObject(m_hMSG, INFINITE);
	CloseHandle(m_hMSG);
	CloseHandle(m_hMutex);

	printf("<DBG> ~CDirectoryWatch() success! Close the MSGThred...\r\n");
}

void CDirectoryWatch::Lock()
{
	WaitForSingleObject(m_hMutex, INFINITE);
}

void CDirectoryWatch::UnLock()
{
	ReleaseMutex(m_hMutex);
}

BOOL CDirectoryWatch::StartWatch(LPWSTR wzPath, funNotifyAction lpfunNotifyAction)
{

	if (wzPath == NULL && wzPath[0] == 0 && lpfunNotifyAction == NULL && !PathFileExistsW(wzPath))
		return FALSE;

	// 排除重复
	Lock();
	std::list<WATCH_PARAM*>::iterator iBegin = m_vWatchList.begin();
	std::list<WATCH_PARAM*>::iterator iEnd = m_vWatchList.end();
	while (iBegin != iEnd)
	{
		if (wcscmp((*iBegin)->wzPath, wzPath) == 0)
		{
			//printf("<ERR> StartWatch() fail! %S 已经被监控！\r\n", wzPath);
			LogSystem::instance()->WriteLogToFileMsg(0, _T("<ERR> StartWatch() fail! 已经被监控！"));
			LogSystem::instance()->WriteLogToFileMsg(0, wzPath);
			UnLock();
			return FALSE;
		}
		++iBegin;
	}
	UnLock();

	MSG msg;
	GetMessageW(&msg, NULL, 0, 0);
	Sleep(20);
	if (!PostThreadMessageW(m_dwMSGTID, MSG_STARTWATCH, (WPARAM)wzPath, (LPARAM)lpfunNotifyAction))
	{
		DWORD error = GetLastError();
		LogSystem::instance()->WriteLogToFileMsg(0, _T("<ERR> StartWatch()->PostThreadMessageW() fail!"));
		//printf("<ERR> StartWatch()->PostThreadMessageW() fail! ErrorCode:0x%X\r\n", GetLastError());
		return FALSE;
	}

	return TRUE;
}


BOOL CDirectoryWatch::StopWatch(LPWSTR wzPath)
{
	if (wzPath == NULL && wzPath[0] == 0 && !PathFileExistsW(wzPath))
		return FALSE;

	BOOL bIsExist = FALSE;
	std::list<WATCH_PARAM*>::iterator iBegin = m_vWatchList.begin();
	std::list<WATCH_PARAM*>::iterator iEnd = m_vWatchList.end();
	while (iBegin != iEnd)
	{
		if (wcscmp((*iBegin)->wzPath, wzPath) == 0)
		{
			bIsExist = TRUE;
			break;
		}
		++iBegin;
	}

	if (!bIsExist)
	{
		//printf("<ERR> StopWatch() fail! %S 没有监控...\r\n", wzPath);
		LogSystem::instance()->WriteLogToFileMsg(0, _T("StopWatch() fail!没有监控..."));
		LogSystem::instance()->WriteLogToFileMsg(0, wzPath);
		return FALSE;
	}

	if (!PostThreadMessageW(m_dwMSGTID, MSG_STOPWATCH, (WPARAM)wzPath, NULL))
	{
		//printf("<ERR> StopWatch()->PostThreadMessageW() fail! ErrorCode:0x%X\r\n", GetLastError());
		LogSystem::instance()->WriteLogToFileMsg(0, _T("StopWatch()->PostThreadMessageW() fail! "));
		return FALSE;
	}

	return TRUE;
}

unsigned __stdcall CDirectoryWatch::MSGThred(LPVOID lpParam)
{
	CDirectoryWatch* This = (CDirectoryWatch*)lpParam;

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		switch (msg.message)
		{
			// 开启监控
		case MSG_STARTWATCH:
		{
			WATCH_PARAM* SwatchParam = new(std::nothrow) WATCH_PARAM;
			if (SwatchParam == 0)
			{
				//printf("<ERR> MSGThred()->New buffer fail!\r\n");
				LogSystem::instance()->WriteLogToFileMsg(0, _T("MSGThred()->New buffer fail "));
				break;
			}
			ZeroMemory(SwatchParam, sizeof(WATCH_PARAM));

			StringCchCopyW(SwatchParam->wzPath, MAX_PATH, (LPCWSTR)msg.wParam);
			SwatchParam->hFile = CreateFileW(
				SwatchParam->wzPath,
				FILE_LIST_DIRECTORY,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, // 注意这里的 异步标志
				NULL
				);
			if (SwatchParam->hFile == INVALID_HANDLE_VALUE)
			{
				DWORD error = GetLastError();
				//printf("<ERR> MSGThred()->CreateFileW fail! ErrorCode:0x%X\r\n", GetLastError());
				LogSystem::instance()->WriteLogToFileMsg(0, _T("MSGThred()->CreateFileW fail!"));
				LogSystem::instance()->WriteLogToFileMsg(0, SwatchParam->wzPath);
				delete[] SwatchParam;
				break;
			}
			SwatchParam->hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
			if (SwatchParam->hEvent == NULL)
			{
				//printf("<ERR> MSGThred()->CreateEventW fail! ErrorCode:0x%X\r\n", GetLastError());
				LogSystem::instance()->WriteLogToFileMsg(0, _T("MSGThred()->CreateEventW fail!"));
				CloseHandle(SwatchParam->hFile);
				delete[] SwatchParam;
				break;
			}
			SwatchParam->ol.hEvent = SwatchParam->hEvent;

			// 接收通知信息的缓存区
			SwatchParam->pBuffer = new(std::nothrow) BYTE[MAX_BUFFER_SIZE];
			if (SwatchParam->pBuffer == 0)
			{
				SwatchParam->pBuffer = new(std::nothrow) BYTE[MAX_BUFFER_SIZE];
				if (SwatchParam->pBuffer == 0)
				{
					//printf("<ERR> MSGThred()->New buffer fail!\r\n");
					LogSystem::instance()->WriteLogToFileMsg(0, _T("MSGThred()->New buffer fail!"));
					CloseHandle(SwatchParam->hEvent);
					CloseHandle(SwatchParam->hFile);
					delete[] SwatchParam;
					break;
				}
			}
			ZeroMemory(SwatchParam->pBuffer, MAX_BUFFER_SIZE);
			SwatchParam->dwBufferSize = MAX_BUFFER_SIZE;
			SwatchParam->NotifyAction = (funNotifyAction)msg.lParam;

			// 开启监控线程 每个文件都有一个
			SwatchParam->hWatch = (HANDLE)_beginthreadex(NULL, NULL, WatchThred, (LPVOID)SwatchParam, NULL, NULL);

			This->Lock();
			OutputDebugString(SwatchParam->wzPath);
			OutputDebugString(_T("***********************\n"));
			This->m_vWatchList.push_back(SwatchParam); // 添加到监控列表 便于控制
			This->UnLock();

			printf("<DBG> StartWatch success!-> %S\r\n", SwatchParam->wzPath);
			break;
		}

		// 停止监控
		case MSG_STOPWATCH:
		{
			WCHAR *wzPath = (WCHAR *)msg.wParam;

			This->Lock();
			std::list<WATCH_PARAM*>::iterator iBegin = This->m_vWatchList.begin();
			std::list<WATCH_PARAM*>::iterator iEnd = This->m_vWatchList.end();
			while (iBegin != iEnd)
			{
				if (wcscmp((*iBegin)->wzPath, wzPath) == 0)
				{
					break;
				}
				++iBegin;
			}
			(*iBegin)->bIsExit = TRUE;
			SetEvent((*iBegin)->hEvent); // 设置状态 不让 监控函数堵塞住
			WaitForSingleObject((*iBegin)->hWatch, INFINITE);

			CloseHandle((*iBegin)->hFile);
			CloseHandle((*iBegin)->hEvent);
			delete[](*iBegin)->pBuffer;

			printf("<DBG> StopWatch success!-> %S\r\n", (*iBegin)->wzPath);

			delete[](*iBegin);
			This->m_vWatchList.erase(iBegin);
			This->UnLock();

			break;
		}
		case MSG_EXIT:
		{
			_endthreadex(0);
		}
		default:
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			break;
		}

	}
	return 0;
}


unsigned __stdcall CDirectoryWatch::WatchThred(LPVOID lpParam)
{
	WATCH_PARAM* pSwatchParam = (WATCH_PARAM*)lpParam;

	DWORD dwByteRet = 0;
	while (true)
	{
		if (pSwatchParam->bIsExit) break; // 退出标记

		if (!ReadDirectoryChangesW(pSwatchParam->hFile,
			pSwatchParam->pBuffer,
			pSwatchParam->dwBufferSize,
			TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
			FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_LAST_ACCESS |
			FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY,
			&dwByteRet,
			&pSwatchParam->ol, // 异步结构
			NULL
			))
		{
			//printf("<ERR> WatchThred()->ReadDirectoryChangesW() fail! ErrorCode:0x%X path:%S\r\n", GetLastError(), pSwatchParam->wzPath);
			LogSystem::instance()->WriteLogToFileMsg(0, _T("WatchThred()->ReadDirectoryChangesW() fail!"));
			LogSystem::instance()->WriteLogToFileMsg(0, pSwatchParam->wzPath);
			continue;
		}
		if (!GetOverlappedResult(pSwatchParam->hFile, &pSwatchParam->ol, &dwByteRet, TRUE))
		{
			//printf("<ERR> WatchThred()->GetOverlappedResult() fail! ErrorCode:0x%X path:%S\r\n", GetLastError(), pSwatchParam->wzPath);
			LogSystem::instance()->WriteLogToFileMsg(0, _T("WatchThred()->GetOverlappedResult() fail!"));
			LogSystem::instance()->WriteLogToFileMsg(0, pSwatchParam->wzPath);
			continue;
		}

		FILE_NOTIFY_INFORMATION* pFileNotify = (FILE_NOTIFY_INFORMATION*)pSwatchParam->pBuffer;
		if (!pFileNotify->Action) continue;

		// 这里要注意的就是 FILE_NOTIFY_INFORMATION.FileName 不是以'\0'结尾
		WCHAR wzSrcFile[MAX_PATH] = { 0 },
			wzDestFile[MAX_PATH] = { 0 };
		//wmemcpy(wzSrcFile, pFileNotify->FileName, pFileNotify->FileNameLength);
		wcscat_s(wzSrcFile, pSwatchParam->wzPath);
		//wcscat_s(wzSrcFile, L"\\");
		wcscat_s(wzSrcFile, pFileNotify->FileName);
		//// 如果是重命名会将新的信息存储到下一个结构
		//if (pFileNotify->Action == FILE_ACTION_RENAMED_OLD_NAME)
		//{
		//	FILE_NOTIFY_INFORMATION* pNextFileNotify = (FILE_NOTIFY_INFORMATION*)((BYTE*)pFileNotify + pFileNotify->NextEntryOffset);
		//	wmemcpy(wzDestFile, pNextFileNotify->FileName, pNextFileNotify->FileNameLength);
		//}

		// 通用处理函数
		if (pFileNotify->Action == FILE_ACTION_ADDED)
			pSwatchParam->NotifyAction(wzSrcFile);

		ZeroMemory(pSwatchParam->pBuffer, MAX_BUFFER_SIZE);
	}

	_endthreadex(0);
	return 0;
}