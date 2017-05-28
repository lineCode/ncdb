#pragma once

class CPIWindowBase
{
public:
	CPIWindowBase(HWND hWnd = NULL);
	~CPIWindowBase(void);
public:
	SIZE GetWindowSize();
	bool IsWindow();
	void SetWndowSize( SIZE szSize );
	void MoveWindow( int x, int y);
	operator HWND();
private:
	HWND			m_hWnd;
};
