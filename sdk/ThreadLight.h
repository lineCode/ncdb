#pragma once
#include "PiTypeDef.h"
#include "PiThreadDef.h"
#include <memory>
/************************************************************************
    fun:    线程类基类
    remark: 用法
                1. 调用Init();
                2. 实现DoWork函数, 线程执行该函数
                3. 安全退出线程 
                    设置等待时间SetDelayMax
                    等待线程 WaitTime();
                    判断是否取消退出 IsWillStopThread()
************************************************************************/

//参数, 1:传进来的参数,  2: NotifyObj*类型
typedef void (*CallBackFun)(void* pObj, void* pNodify);
typedef void (*FunLog)(tcpchar strMsg);


class CEventLight;
class CLock;

using namespace std;


class CThreadLight
{
public:
	CThreadLight(ProcThreadFun pFun = NULL, void* pAdd = NULL);
    virtual ~CThreadLight(void);
    
	bool DestroyThread();
	virtual bool ClearRes();
protected:
	virtual bool DoRunBegin();
	virtual unsigned ThreadRun( );

private:
	/********************************************************************
		fun:	子类重写, 实现线程任务
		param:	
		ret:	
	********************************************************************/
	virtual DWORD DoWork() ;
	virtual bool DoRunEnd(DWORD dwRet);
public:

	template <class T, class Param>
	static CThreadLight* Run(T, Param paramR, bool bAutoDestroy = false);

public:
	/************************************************************************
        fun:    退出线程. 等待某段时间(可设置)让线程退出, 如果超时则强制杀掉线程
        remark:
        ret:   
    ************************************************************************/
    bool ExitThread();

    HANDLE GetThreadHandle();
    void SetThreadName(tcpchar szName);
    bool Init();
    bool UnInit();
	bool Run();
    bool SetAutoDestroy();
	/************************************************************************
        fun:	主动通知外部的接口， 
        param:  pAdd-> 附加传递的额外信息
        ret:    
        remark: 
	/************************************************************************/
    bool IsThreadRunning();
	bool Notify(NotifyObj* pNt);
    void* GetObject();
    void SetWaitThreadTimeout(UNLONG nMilliSec);
    bool IsWillStopThread();
    bool WaitTime();
	bool WaitThreadExit();
    void SetDelayMax( int nPeriodMS );
    bool Reset();
	void SetFunLog(FunLog pfun) {m_funLog = pfun;}
	bool DieKillThread() const { return m_bDieKillThread; }
	void DieKillThread(bool val) { m_bDieKillThread = val; }
protected:
	bool InitVariable();
	
    bool StopThread();

protected:
	CallBackFun		m_CallBackFun;		//回调函数指针
	static FunLog			m_funLog;
	void*			m_pObject;			//外部设置的类对象， 传给回调函数的一个参数
	static unsigned __stdcall SProc(void* pAdd);

	virtual DWORD DoThread( ThreadParam* pThreadParam );

	bool ClearThread();
    bool Clear();
	/********************************************************************
		fun:	线程刚执行时调用, 做些初始化
		param:	
		ret:	
	********************************************************************/
    bool InitThread();
	static bool OutLog( tcpchar strMsg );
	bool WaitInitDone();
	DWORD           m_dwThreadID;
    HANDLE			m_hThread;
    UNLONG          m_dwWaitThread;      //等待线程结束的时间
    bool                    m_bStopThread;    //是否停止线程
	auto_ptr<CEventLight>                   m_pLockDetect;
	auto_ptr<CEventLight>                   m_pLockThreadInitDone;	//是否初始化完成
    auto_ptr<CLock>                 m_pLockThread;
    bool m_bAutoDestroy;        //在任务执行完后是否自动销毁
    tstring         m_strThreadName;
    bool                m_bInit;    //是否已经初始化

	bool					m_bDieKillThread;		
	
	ProcThreadFun			m_procThreadFun;	//自定义函数
	void*					m_paramR;			//自定义函数的参数
	
	//void*			m_p
};

template <class T, class Param>
CThreadLight* CThreadLight::Run( T pFun, Param paramR, bool bAutoDestroy /*= false*/)
{
	CThreadLight* pThread = new CThreadLight(pFun, (void*)paramR);
	if (bAutoDestroy)
	{
		pThread->SetAutoDestroy();
	}
	pThread->Init();
	pThread->Run();
	pThread->WaitInitDone();
	
	return pThread;
}

