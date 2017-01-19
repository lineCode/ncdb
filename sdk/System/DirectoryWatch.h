// DirectoryWatch.h
#pragma once
#include <windows.h>
#include <list>

// 通用的通知处理函数
typedef void(*funNotifyAction)(LPWSTR wzSrcFile);

// 要监控的目录的信息
typedef struct _WATCH_PARAM
{
	_WATCH_PARAM()
	{
		hFile = INVALID_HANDLE_VALUE;
		hEvent = NULL;
		hWatch = NULL;
		pBuffer = NULL;
		bIsExit = FALSE;
		dwBufferSize = 0;
		NotifyAction = NULL;
		ZeroMemory(&wzPath, sizeof(WCHAR)*MAX_PATH);
		ZeroMemory(&ol, sizeof(OVERLAPPED));
	}
	WCHAR  wzPath[MAX_PATH]; // 路径
	HANDLE hFile;            // 文件句柄
	HANDLE hEvent;           // 事件句柄
	BYTE  *pBuffer;          // 缓存区
	DWORD  dwBufferSize;     // 缓存区大小
	OVERLAPPED ol;           // 异步结构

	HANDLE hWatch;           // 监控线程句柄
	BOOL   bIsExit;          // 监控线程是否退出

	funNotifyAction NotifyAction;

}WATCH_PARAM, *PWATCH_PARAM;

// 监控类
class CDirectoryWatch
{
public:
	CDirectoryWatch();
	virtual ~CDirectoryWatch();

public:
	BOOL StartWatch(LPWSTR wzPath, funNotifyAction lpfunNotifyAction);
	BOOL StopWatch(LPWSTR wzPath);

private:
	static unsigned __stdcall MSGThred(LPVOID lpParam);   // 消息线程
	static unsigned __stdcall WatchThred(LPVOID lpParam); // 监控线程

	void Lock();
	void UnLock();

private:
	std::list<WATCH_PARAM*> m_vWatchList; // 监控的列表
	HANDLE m_hMSG;                        // 消息线程句柄
	DWORD  m_dwMSGTID;                    // 消息线程ID
	HANDLE m_hMutex;                      // 互斥锁(list保护)
};