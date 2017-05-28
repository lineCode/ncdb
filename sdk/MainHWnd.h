#pragma once

#ifndef _CWINDOW_ITERATOR_ZWF_H_  
#define _CWINDOW_ITERATOR_ZWF_H_  
//////////////////////////////////////////////////////////////////////////  
#include <vector>  
using namespace std;  
//////////////////////////////////////////////////////////////////////////  
class CWindowIterator  
{  
public:  
	// 构造析构函数  
	CWindowIterator();  
	~CWindowIterator();  

public:  
	// 接口函数  
	void EnumWnds();                        // 枚举窗口  
	unsigned long GetWndCounts();           // 获得窗口总数  
	HWND First();                           // 获得第一个窗口句柄  
	HWND Next();                            // 获得下一个窗口句柄  
	HWND GetHWnd(unsigned long wndIndex);   // 根据索引获得窗口句柄         

protected:  
	vector<HWND> m_hWndArray;               // 存放枚举得到的窗口句柄  
	unsigned long m_nCurrentWndIndex;       // 当前窗口句柄  

	// 枚举窗口回调函数  
	static BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lp);  

	// virtual enumerator  
	virtual BOOL AddWnd(HWND hwnd);  

	// override to filter different kinds of windows  
	virtual BOOL IsTargetWnd(HWND hwnd) {return TRUE;}  

};  
//////////////////////////////////////////////////////////////////////////  
//////////////////////////////////////////////////////////////////////////  
// 派生出主窗口类（只获取进程的主窗口）  
class CMainHWnd : public CWindowIterator  
{  
public:  
	// 构造析构函数  
	CMainHWnd();  
	CMainHWnd(unsigned long pid);  
	~CMainHWnd();  

public:  
	// 接口函数  
	void EnumWndsByPid(DWORD pid, const BOOL& bVisible = TRUE, const BOOL& top = TRUE);  
	void EnumWndsByTitle(const std::wstring& title, const std::wstring& classname, const BOOL& bVisible = TRUE, const BOOL& top = TRUE);  
protected:  
	DWORD m_pid; 
	BOOL  m_bVisible, m_bTop;
	std::wstring m_strTitle;
	std::wstring m_strClassName;
	virtual BOOL IsTargetWnd(HWND hwnd);  

};  
//////////////////////////////////////////////////////////////////////////  
#endif
