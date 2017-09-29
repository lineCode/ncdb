#pragma once
#include "PiTypeDef.h"
#include "PiThreadDef.h"
#include <memory>
/************************************************************************
    fun:    �߳������
    remark: �÷�
                1. ����Init();
                2. ʵ��DoWork����, �߳�ִ�иú���
                3. ��ȫ�˳��߳� 
                    ���õȴ�ʱ��SetDelayMax
                    �ȴ��߳� WaitTime();
                    �ж��Ƿ�ȡ���˳� IsWillStopThread()
************************************************************************/

//����, 1:�������Ĳ���,  2: NotifyObj*����
Pi_NameSpace_Begin

typedef void (*CallBackFun)(void* pObj, void* pNodify);
typedef void (*FunLog)(tcpchar strMsg);


class CEventLight;
class CPiLock;

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
		fun:	������д, ʵ���߳�����
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
        fun:    �˳��߳�. �ȴ�ĳ��ʱ��(������)���߳��˳�, �����ʱ��ǿ��ɱ���߳�
        remark:
        ret:   
    ************************************************************************/
	bool ExitThread(UNLONG dwWaitThread = 1000);

    HANDLE GetThreadHandle();
    void SetThreadName(tcpchar szName);
    bool Init();
    bool UnInit();
	bool Run();
    bool SetAutoDestroy();
	/************************************************************************
        fun:	����֪ͨ�ⲿ�Ľӿڣ� 
        param:  pAdd-> ���Ӵ��ݵĶ�����Ϣ
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
	CallBackFun		m_CallBackFun;		//�ص�����ָ��
	static FunLog			m_funLog;
	void*			m_pObject;			//�ⲿ���õ������ �����ص�������һ������
	static unsigned __stdcall SProc(void* pAdd);

	virtual DWORD DoThread( ThreadParam* pThreadParam );

	bool ClearThread();
    bool Clear();
	/********************************************************************
		fun:	�̸߳�ִ��ʱ����, ��Щ��ʼ��
		param:	
		ret:	
	********************************************************************/
    bool InitThread();
	static bool OutLog( tcpchar strMsg );
	bool WaitInitDone();
	DWORD           m_dwThreadID;
    HANDLE			m_hThread;
    UNLONG          m_dwWaitThread;      //�ȴ��߳̽�����ʱ��
    bool                    m_bStopThread;    //�Ƿ�ֹͣ�߳�
	auto_ptr<CEventLight>                   m_pLockDetect;
	auto_ptr<CEventLight>                   m_pLockThreadInitDone;	//�Ƿ��ʼ�����
    auto_ptr<CPiLock>                 m_pLockThread;
    bool m_bAutoDestroy;        //������ִ������Ƿ��Զ�����
    tstring         m_strThreadName;
    bool                m_bInit;    //�Ƿ��Ѿ���ʼ��

	bool					m_bDieKillThread;		
	
	ProcThreadFun			m_procThreadFun;	//�Զ��庯��
	void*					m_paramR;			//�Զ��庯���Ĳ���
	
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


Pi_NameSpace_End
