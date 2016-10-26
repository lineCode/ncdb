#pragma once
#include "EventLight.h"

#define UM_CB_CALL		WM_USER + 202
class CPiSemaphore
{
public:
	typedef bool (*TypeFuncWait)(void* pAdd);

	CPiSemaphore();
	~CPiSemaphore(void);
public:
	bool Init();
	/*------------------------------------------------------------
			fun:	设置等待n个事件完成后执行指定的操作
			ret:	
			param: 
			remark:	
	------------------------------------------------------------*/
	void SetWaitCount(UNINT nCount);
	void AddWaitDone(UNINT nCount = 1);
	bool IsArrive();
	void SetCB( TypeFuncWait pFun, void* pAdd );
	void Wait();
	virtual bool CallCB();
	void SetMsgNotify( HWND hW, int nMsg);
	bool ExecuteCB();
private:
	long					m_nCount;
	CEventLight				m_event;
	CLock					m_lock;
	TypeFuncWait			m_pFun;
	void*					m_pAdd;

	HWND					m_hWnd;
	int						m_nMsgID;
};
