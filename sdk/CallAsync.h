#pragma once
#include "ThreadLight.h"
#include "boost\boostHeader.h"

template<typename T>
class CCallAsync
	:public CThreadLight
{
protected:
	CCallAsync(HWND	hWnd);
	~CCallAsync();
protected:
	virtual DWORD DoWork();
	virtual bool DoRunEnd(DWORD dwRet);
public:
	

	static bool Call(T f, HWND hWnd, int nNotifyMsg, void* nData);
protected:
private:
	T*				m_nBindFun;
	HWND			m_hWnd;
	int				m_nMsgId;
	void*				m_nData;
};

template<typename T>
bool CCallAsync<T>::Call( T f, HWND hWnd, int nNotifyMsg, void* nData)
{
	CCallAsync<T>* pCA = new CCallAsync<T>(hWnd);

	pCA->m_nBindFun = new T(f);
	//m_nBindFun = f;
	pCA->m_nMsgId = nNotifyMsg;
	pCA->m_nData = nData;

	pCA->DieKillThread(true);
	pCA->Init();
	pCA->Run();
	pCA->WaitInitDone();
	return true;
}

template<typename T>
bool CCallAsync<T>::DoRunEnd( DWORD dwRet )
{
	if (m_nMsgId)
	{
		::SendMessage(m_hWnd, m_nMsgId, dwRet, (LPARAM)m_nData);
	}
	return true;
}

template<typename T>
CCallAsync<T>::CCallAsync( HWND hWnd )
:m_nMsgId(NULL)
,m_nBindFun(NULL)
,m_nData(NULL)
{
	m_hWnd = hWnd;
}

template<typename T>
CCallAsync<T>::~CCallAsync()
{

}

template<class T>
DWORD CCallAsync<T>::DoWork()
{
	return (*m_nBindFun)();
}








template<class T>
struct tagPARAM_CALL
{
    bool bRet;      //调用成功失败标识
    T   tagParam;   //传入的附加数据
};
template<class Obj, class Func, class Param, class CBObj, class CBFun, class CBParam>
class CThreadCallAsync
    :public CThreadLight
{
public:
    CThreadCallAsync( Obj obj, Func func, Param pr, CBObj cbOjb, CBFun cbFun, CBParam cbParam);
    ~CThreadCallAsync(void){ /*NULL*/ }
public:
    virtual DWORD DoWork();
private:
    Obj         m_obj;
    Func        m_fun;
    Param       m_param;
    CBObj       m_cbObj;
    CBFun       m_cbFun;
    CBParam     m_CBParam;
};

template<class Obj, class Func, class Param, class CBObj, class CBFun, class CBParam>
CThreadCallAsync<Obj, Func, Param, CBObj, CBFun, CBParam>
    ::CThreadCallAsync( Obj obj, Func func, Param pr, CBObj cbOjb, CBFun cbFun, CBParam cbParam)
{
    m_obj       = obj;
    m_fun       = func;
    m_param     = pr;
    m_cbObj     = cbOjb;
    m_cbFun     = cbFun;
    m_CBParam   = cbParam;
}

template<class Obj, class Func, class Param, class CBObj, class CBFun, class CBParam>
DWORD CThreadCallAsync<Obj, Func, Param, CBObj, CBFun, CBParam>::DoWork()
{
    bool bRet = (m_obj->*m_fun)(m_param);
    tagPARAM_CALL<CBParam> pc;
    pc.bRet = bRet;
    pc.tagParam = m_CBParam;
    (m_cbObj->*m_cbFun)(pc);
    return 1;
}

template<class Obj, class Func, class Param, class CBObj, class CBFun, class CBParam>
bool CallASync( Obj obj, Func func, Param pr, CBObj cbObj, CBFun cbFun, CBParam cbPar)
{
    CThreadCallAsync<Obj, Func, Param, CBObj, CBFun, CBParam>* callAsync 
        = new CThreadCallAsync<Obj, Func, Param, CBObj, CBFun, CBParam>(obj, func, pr, cbObj, cbFun, cbPar);
    callAsync->Init();
    callAsync->SetAutoDestroy();
    callAsync->Run();
    return true;
}

