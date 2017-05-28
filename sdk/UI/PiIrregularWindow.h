#pragma once
#include "PiTypeDef.h"

class CPiIrregularWindow
{
public:
	CPiIrregularWindow(void);
	~CPiIrregularWindow(void);
	/************************************************************************
			虚函数
	************************************************************************/
protected:
	/*------------------------------------------------------------
		fun:	显示前调用, 可使用m_hdcMem来绘制想要显示的内容
		ret:	
		param: 
		remark:	
	------------------------------------------------------------*/
	virtual bool OnBeginDrawLayer();

public:
	bool CreateIrregularWindow(tcpchar szPic);
	bool CreateIrregularWindow(UINT nCX, UINT nCY);
	bool DestroyIrregularWindow();
	bool UpdateLayeredWnd();
	bool Update();

	HDC HdcTrans() const { return m_hdcMem; }
	operator HWND(){return m_hWnd;}
protected:
	//返回false, 不传递消息给默认处理函数
	virtual DWORD HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandle);
	bool SetTransWnd( HWND hWnd, LPCTSTR szPic = NULL);
	bool SetLayeredStyle( HWND hWnd );
	bool BeginDrawLayer();
	bool CreateMemDC();
	virtual bool RegisterWindowClass();
	virtual bool _Create();
	static LRESULT CALLBACK  __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseEnter(WPARAM wp, LPARAM lp);
	virtual LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);

public:
protected:
	HWND		m_hWndDist;	//透明的窗体句柄
	HDC			m_hdcMem;	//要绘制的异形区域的dc
	
	HBITMAP		m_bmMem;

	
	SIZE		m_sizeWindow;	
	RECT		m_rcWindow;		
	tstring		m_strPicBk;			//背景图, 每次绘制都会先画这张

	HWND				m_hWnd;
	bool				m_bMouseEnter;
	bool				m_bCursorHand;
	Color				m_clText;
	Color				m_clNormal;
	Color				m_clEnter;
};
