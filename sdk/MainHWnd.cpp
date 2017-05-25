#include "StdAfx.h"
#include "MainHWnd.h"



/******************************************************************** 
created:    2010/03/09 
created:    9:3:2010   10:10 
file base:  CWindowIterator 
file ext:   cpp 
author:     zhangwf 

purpose:    封装枚举窗口类 
*********************************************************************/  
//////////////////////////////////////////////////////////////////////////  
#include <Windows.h>  
#include "MainHWnd.h"
//////////////////////////////////////////////////////////////////////////  
// 构造函数  
CWindowIterator::CWindowIterator()  
{  
	// 初始化变量  
	m_hWndArray.clear();  
	m_nCurrentWndIndex = 0;  
}  

// 析构函数  
CWindowIterator::~CWindowIterator()  
{  

}  

//////////////////////////////////////////////////////////////////////////  
// 接口函数  
// 获得窗口总数  
unsigned long CWindowIterator::GetWndCounts()  
{  
	return m_hWndArray.size();  
}  

// 获得第一个窗口句柄  
HWND CWindowIterator::First()  
{  
	if (m_hWndArray.size() != 0)  
	{  
		m_nCurrentWndIndex = 0;  
		return m_hWndArray[m_nCurrentWndIndex];  
	}  

	return NULL;  
}  

// 获得下一个窗口句柄  
HWND CWindowIterator::Next()  
{  
	if (m_nCurrentWndIndex+1 < m_hWndArray.size())  
	{  
		m_nCurrentWndIndex += 1;  
		return m_hWndArray[m_nCurrentWndIndex];  
	}  

	return NULL;  
}  

// 根据索引获得窗口句柄  
HWND CWindowIterator::GetHWnd(unsigned long wndIndex)  
{  
	if (wndIndex < m_hWndArray.size())  
	{  
		return m_hWndArray[wndIndex];  
	}  

	return NULL;  
}  

void CWindowIterator::EnumWnds()  
{  
	// 清除上次内容  
	m_hWndArray.clear();  

	// 枚举所有窗口  
	::EnumWindows(EnumWndProc, (LPARAM)this);  
}  

//////////////////////////////////////////////////////////////////////////  
// 非接口函数  
// 枚举窗口回调函数  
BOOL CALLBACK CWindowIterator::EnumWndProc(HWND hwnd, LPARAM lp)  
{  
	return ((CWindowIterator*)lp)->AddWnd(hwnd);  
}  

// virtual enumerator  
BOOL CWindowIterator::AddWnd(HWND hwnd)  
{  
	if (IsTargetWnd(hwnd) == TRUE)  
	{  
		m_hWndArray.push_back(hwnd);          
	}  

	return TRUE;  
}  
//////////////////////////////////////////////////////////////////////////  
//////////////////////////////////////////////////////////////////////////  
// 派生出主窗口句柄类  
// 构造函数  
CMainHWnd::CMainHWnd()  
:CWindowIterator()  
{  
	m_pid = 0;  
	m_bTop = FALSE;
	m_strTitle.clear();
	m_strClassName.clear();
	m_bVisible = FALSE;
}  

// 带参构造函数  
CMainHWnd::CMainHWnd(unsigned long pid)  
:CWindowIterator()  
{  
	m_pid = pid;  
	m_strTitle.clear();
	m_strClassName.clear();
	m_bVisible = FALSE;
	m_bTop = FALSE;
	EnumWnds();  
}  

// 析构函数  
CMainHWnd::~CMainHWnd()  
{  

}  

//////////////////////////////////////////////////////////////////////////  
// 接口函数  
void CMainHWnd::EnumWndsByPid(DWORD pid, const BOOL& bVisible, const BOOL& top)  
{  
	m_pid = pid;  
	m_bVisible = bVisible;
	m_bTop = top;
	EnumWnds();  
}  

void CMainHWnd::EnumWndsByTitle(const std::wstring& title, const std::wstring& classname, const BOOL& bVisible, const BOOL& top)
{
	m_strTitle = title;
	m_strClassName = classname;
	m_bVisible = bVisible;
	m_bTop = top;
	EnumWnds();
}

//////////////////////////////////////////////////////////////////////////  
// 非接口函数  
BOOL CMainHWnd::IsTargetWnd(HWND hwnd)  
{  
	// 顶层且可见  
	if (((m_bTop && ::GetParent(hwnd)==NULL)||!m_bTop) && ((m_bVisible && ::IsWindowVisible(hwnd)==TRUE)||(!m_bVisible && IsWindow(hwnd))))   
	{        
		if (m_pid > 0)
		{
			DWORD pidwin;  
			::GetWindowThreadProcessId(hwnd, &pidwin);  
			if (pidwin == m_pid)  
			{  
				return TRUE;  
			}       
		}
		else if (m_strTitle.length() > 0)
		{
			DWORD pidwin = 0;  
			::GetWindowThreadProcessId(hwnd, &pidwin);  
			if (GetCurrentProcessId() == pidwin)
			{
				return FALSE;
			}
			TCHAR tszTitle[128] = {0}, tszClass[128] = {0};
			//if(::SendMessage(hwnd, WM_GETTEXT, _countof(tszTitle)-1, (LPARAM)tszTitle))
			if (::GetWindowText(hwnd, tszTitle, _countof(tszTitle)-1)
				&& ::GetClassName(hwnd, tszClass, _countof(tszClass)-1))
			{

				std::wstring title(tszTitle);
				if(title.find(m_strTitle)==0 && !lstrcmp(tszClass, m_strClassName.c_str())){
					return TRUE;  
				}
			}
		}
	}  

	// 不是目标窗口  
	return FALSE;  
}  