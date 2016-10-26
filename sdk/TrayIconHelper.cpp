#include "StdAfx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "TrayIconHelper.h"


TrayIconInfo::TrayIconInfo()
{
}

TrayIconInfo::TrayIconInfo( TrayIconInfo& obj )
{
	*this=obj;
}

TrayIconInfo& TrayIconInfo::operator=( TrayIconInfo& obj )
{
	bShow=obj.bShow;
	idCommand=obj.idCommand;
	hMainWnd=obj.hMainWnd;
	strFilePath=obj.strFilePath;
	strTip=obj.strTip;
	rcIcon=obj.rcIcon;

	return *this;
}

void TrayIconInfo::Dump()
{
#if 0
	TRACE("TrayIconInfo(0x%08X):\n",this);
	TRACE("strFilePath=%s\n",strFilePath);
	TRACE("strTip=%s\n",strTip);
	TRACE("bShow=%s\n",bShow?"TRUE":"FALSE");
	TRACE("hMainWnd=0x%08X\n",hMainWnd);
	TRACE("rcIcon=%d, %d, %d, %d\n",rcIcon.left, rcIcon.top, rcIcon.right, rcIcon.bottom);
	TRACE("idCommand=%08d\n",idCommand);
	TRACE("\n");
#endif
}

BOOL TrayIconInfo::operator==( TrayIconInfo& obj )
{
	return (strFilePath==obj.strFilePath && 
			strTip==obj.strTip && 
			idCommand==obj.idCommand);
}


typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

CTrayIconHelper::CTrayIconHelper(void)
{
}


CTrayIconHelper::~CTrayIconHelper(void)
{
}

BOOL CTrayIconHelper::IsWow64()
{
    BOOL bIsWow64 = FALSE;

    LPFN_ISWOW64PROCESS 
        fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
        GetModuleHandle(_T("kernel32")),"IsWow64Process");

    if (NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            // handle error
        }
    }
    return bIsWow64;
}


HWND CTrayIconHelper::GetTrayWnd()
{
    HWND hWnd = NULL;
    
    hWnd = FindWindow(_T("Shell_TrayWnd"), NULL);
    hWnd = FindWindowEx(hWnd, NULL, _T("TrayNotifyWnd"), NULL);
    hWnd = FindWindowEx(hWnd, NULL, _T("SysPager"), NULL);
    hWnd = FindWindowEx(hWnd, NULL, _T("ToolbarWindow32"), NULL);

    return hWnd;
}

HWND CTrayIconHelper::GetNotifyIconOverflowWindow()
{
    HWND hWnd = NULL;

    hWnd = FindWindow(_T("NotifyIconOverflowWindow"), NULL);
    hWnd = FindWindowEx(hWnd, NULL, _T("ToolbarWindow32"), NULL);

    return hWnd;
}

CArray<TrayIconInfo, TrayIconInfo> *CTrayIconHelper::GetTrayIconInfoArrayForWnd(HWND hWnd, BOOL bHiddenBar)
{
	CArray<TrayIconInfo, TrayIconInfo> *pArrayResult = new CArray<TrayIconInfo, TrayIconInfo>;
	TrayIconInfo iconInfo;

    DWORD dwProcessId = 0;
	HANDLE hProcess = NULL;
	LPVOID lAddress = NULL;
	LPVOID lpRect = NULL;
	
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	do 
	{
		hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, dwProcessId);
		if ( hProcess == NULL )
		{
			break;
		}

		// 分配虚拟内存
		lAddress = VirtualAllocEx(hProcess, 0, 4096, MEM_COMMIT, PAGE_READWRITE);
		if ( lAddress == NULL )
		{
			break;
		}

		lpRect = VirtualAllocEx(hProcess,NULL,sizeof(CRect),MEM_COMMIT, PAGE_READWRITE);
		if ( lpRect == NULL )
		{
			break;
		}

		// 得到图标个数
		int lButton = SendMessage(hWnd, TB_BUTTONCOUNT, 0, 0);

		for (int i = 0; i < lButton; i++) {
			BYTE buff[1024] = {0};
			HWND hMainWnd = NULL;
			CString strFilePath;
			CString strTip;
			int idCommand;
			CRect rcIcon;

			SendMessage(hWnd, TB_GETBUTTON, i, (LPARAM)lAddress);

			if (IsWow64()) // 任务栏是64位的
			{
				TBBUTTON64 TButton64;
				int nStrOffset = 24; 

				ReadProcessMemory(hProcess, (LPVOID)lAddress, &TButton64, sizeof(TBBUTTON64), NULL);

				// 获取命令ID
				idCommand = TButton64.idCommand;

				// 获取图标数据缓冲区内容
				ReadProcessMemory(hProcess, (LPVOID)TButton64.dwData, &buff, sizeof(buff), NULL);

				// 获取窗口句柄
				hMainWnd = (HWND)(*((DWORD*)buff));

				// 获取可行性文件路径
				strFilePath = (WCHAR *)buff + nStrOffset;

				// 获取Tip
				strTip = (WCHAR *)buff + nStrOffset + MAX_PATH;   
			}
			else
			{
				TBBUTTON32 TButton32;
				TRAYDATA32 TrayData;

				ReadProcessMemory(hProcess, (LPVOID)lAddress, &TButton32, sizeof(TBBUTTON32), NULL);

				// 获取命令ID
				idCommand = TButton32.idCommand;

				// 获取图标数据缓冲区内容
				ReadProcessMemory(hProcess, (LPVOID)TButton32.dwData, &TrayData, sizeof(TrayData), NULL);

				// 获取窗口句柄
				hMainWnd = TrayData.hWnd;

				// 获取可行性文件路径
				strFilePath = (WCHAR *)TrayData.szExePath;

				// 获取Tip
				strTip = (WCHAR *)TrayData.szTip;  
			}     

			// 获取图标矩形		
			SendMessage(hWnd, TB_GETITEMRECT, i, (LPARAM)lpRect);
			ReadProcessMemory(hProcess, lpRect, &rcIcon, sizeof(CRect), NULL);
			MapWindowPoints(hWnd, NULL, (LPPOINT)&rcIcon, 2);

			iconInfo.bShow = !bHiddenBar;
			iconInfo.idCommand = idCommand;
			iconInfo.hMainWnd = hMainWnd;
			iconInfo.strFilePath = strFilePath;
			iconInfo.strTip = strTip;
			iconInfo.rcIcon = rcIcon;
			pArrayResult->Add(iconInfo);
		}
	} while(0);

	if(lAddress != NULL && hProcess != NULL) 
		VirtualFreeEx(hProcess, lAddress, 4096, MEM_RELEASE);

	if(lpRect != NULL && hProcess != NULL) 
		VirtualFreeEx(hProcess, lpRect, sizeof(CRect), MEM_RELEASE);

	if(hProcess != NULL) 
		CloseHandle(hProcess);

	return pArrayResult;
}


CArray<TrayIconInfo, TrayIconInfo> *CTrayIconHelper::GetTrayIconInfoArray()
{
	CTrayIconHelper TrayIconHelper;

	CArray<TrayIconInfo, TrayIconInfo> * pArray1 = GetTrayIconInfoArrayForWnd(TrayIconHelper.GetTrayWnd(), FALSE);

	CArray<TrayIconInfo, TrayIconInfo> * pArray2 = GetTrayIconInfoArrayForWnd(TrayIconHelper.GetNotifyIconOverflowWindow(), TRUE);
	int count = pArray2->GetCount();
	for (int i=0; i<count; i++)
	{
		TrayIconInfo iconInfo = pArray2->ElementAt(i);
		pArray1->Add(iconInfo);
	}
	delete pArray2;

	return pArray1;
}

BOOL CTrayIconHelper::GetTrayIconRect(CString strTip, CRect &rcIcon, BOOL &bShow)
{
	BOOL bFound = FALSE;

	memset(&rcIcon, 0, sizeof(CRect));
	bShow = FALSE;

	CTrayIconHelper TrayIconHelper;

	CArray<TrayIconInfo, TrayIconInfo> * pArray = GetTrayIconInfoArray();
	int count = pArray->GetCount();
	for (int i=0; i<count; i++)
	{
		TrayIconInfo iconInfo = pArray->ElementAt(i);
 		if (iconInfo.strTip == strTip)
		{
			rcIcon = iconInfo.rcIcon;
			bShow = iconInfo.bShow;
			bFound = TRUE;
			break;
		}
	}
	delete pArray;

	return bFound;
}

BOOL CTrayIconHelper::GetTrayIconRectWithPath( CString strPath, CRect &rcIcon, BOOL &bShow )
{
    BOOL bFound = FALSE;

    memset(&rcIcon, 0, sizeof(CRect));
    bShow = FALSE;

    CTrayIconHelper TrayIconHelper;

    CArray<TrayIconInfo, TrayIconInfo> * pArray = GetTrayIconInfoArray();
    int count = pArray->GetCount();
    for (int i=0; i<count; i++)
    {
        TrayIconInfo iconInfo = pArray->ElementAt(i);
        if (iconInfo.strFilePath == strPath)
        {
            rcIcon = iconInfo.rcIcon;
            bShow = iconInfo.bShow;
            bFound = TRUE;
            break;
        }
    }
    delete pArray;

    return bFound;
}
