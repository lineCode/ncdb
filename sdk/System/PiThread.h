#pragma once
#include "threadlight.h"

class CPiThread :
	public CThreadLight
{
public:
	CPiThread(ProcThreadFun pFun = NULL, void* pAdd = NULL);


	//CPiThread(T* pObj,  ProcThreadFun pFun = NULL, void* pAdd = NULL);

	virtual ~CPiThread(void);
public:
protected:
	/*virtual bool DoRunEnd(DWORD dwRet );
	virtual unsigned ThreadRun( );
	static DWORD ProcObjCall(void* paramR);*/
private:
	HWND				m_hWnd;
	int					m_nMsgID;
};
