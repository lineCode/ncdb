#pragma once
#include "PiTypeDef.h"
#include "ResCommon.h"
Pi_NameSpace_Begin

//#define M_TP_XP_OK	//支持XP

typedef VOID (*CB_TimerCallback)(PVOID lpParameter,BOOLEAN TimerOrWaitFired);


typedef bool (*CB_TimerFunc)(HANDLE nTimerID, PVOID pAddi);


class CPiKernel
{
	typedef list<PTP_TIMER>	LST_TIMERID;
	typedef int TimerID;
	struct tagPTTimer
	{
		CB_TimerFunc funTimer;
		PVOID nData;
		CPiKernel* pTimerMng;
		bool bOneOff;	//是否是一次性的
		HANDLE hTimer;
		ULONG	nCircle;	//定时器周期, 单位毫秒
		TimerID nTimerID;	//用户指定标示
	};
	typedef map<TimerID,tagPTTimer*>	MAP_TIMER_PARAM;
	enum ENUM_TIMER_MODE{ 
		mode_callback,	//定时器周期到立即调用
		mode_queue		//定时器在上一个周期任务完成后再启动下一个周期
	};
public:
	CPiKernel(void);
	~CPiKernel(void);
public:
	bool Init();
	/*------------------------------------------------------------
			fun:	以指定的频率定时执行指定的函数
			ret:	返回定时器id
			param: cbTimer:回调函数,  nCircle, 间隔调用函数的时间, 单位毫秒. nData:附加数据, 作为回调函数的第二个参数. oneOff:true, 回调函数只调用一次后定时器就销毁, false:定时执行
			remark:	定时器时间调用回调函数时第一个参数为定时器的id, 如有需要可作为参数调用ReleaseTimer函数来取消定时器
	------------------------------------------------------------*/
	bool BeginTimer(CB_TimerFunc cbTimer, UNINT nCircle, PVOID nData = NULL, bool oneOff = false, TimerID nTimer = 0);

	bool CreateTimerQ();

	/*------------------------------------------------------------
			fun:	取消定时器
			ret:	
			param: nTimer:要取消的定时器id, bWait: true->等待所有定时器回调函数执行完再取消, false->直接取消, 不等待
			remark:	
	------------------------------------------------------------*/
	bool ReleaseTimer(TimerID nTimer, bool bClearMem = true);
	/*------------------------------------------------------------
			fun:	关闭所有的定时器
			ret:	
			param: 
			remark:	
	------------------------------------------------------------*/
	bool ReleaseAllTimer(bool bWait = true);

	
/*------------------------------------------------------------
		xp版

		typedef VOID (*CB_TimerCallback)(PTP_CALLBACK_INSTANCE Instance,  PVOID Context, PTP_TIMER Timer);
		typedef bool (*CB_TimerFunc)(PTP_TIMER nTimerID, PVOID pAddr);


	typedef list<PTP_TIMER>	LST_TIMERID;
	struct tagPTTimer
	{
	CB_TimerFunc funTimer;
	PVOID nData;
	CPiKernel* pTimerMng;
	bool bOneOff;	//是否是一次性的
	};

	PTP_TIMER BeginTimer(CB_TimerFunc cbTimer, UNINT nCircle, PVOID nData, bool oneOff = false);
	static VOID CALLBACK TimerCallback(PTP_CALLBACK_INSTANCE Instance,  PVOID Context, PTP_TIMER Timer);
	bool ReleaseTimer(PTP_TIMER nTimer, bool bWait = false);
	bool ReleaseAllTimer();
	LST_TIMERID		m_lstTimerID;

	------------------------------------------------------------*/

//#endif
	/*------------------------------------------------------------
		fun:	执行dos命令
		ret:	true: 执行成功,				
		param: 
			strOut: 执行返回数据,	dwWaitMillSec: 等待时间, 毫秒
		remark:	命令返回的数据长度不能超过16k
				如果超时时间到, 并且没收到命令返回的任何数据, 则返回false
	------------------------------------------------------------*/

	bool StartItem(LPTHREAD_START_ROUTINE cbFun, void* pData);

	static bool ExecuteDos(tcpchar szCmd, string& strOut, DWORD dwWaitMillSec = INFINITE);
	static bool ExecuteDos(tcpchar szCmd, tstring& strOut, DWORD dwWaitMillSec = INFINITE);
	static bool ExplorePath( tcpchar szPath, tcpchar szFocusFile = NULL);
	static DWORD RunBinExitCode(tcpchar szCmd);
	static bool RunBin(tcpchar szCmd);
	/*------------------------------------------------------------
			fun:	通过内存映射写数据
			ret:	
			param: 
			remark:	注意大小不能超过内部限制的大小, 1024
	------------------------------------------------------------*/
	static bool WriteMemMap( LPCTSTR szData, LPCTSTR szName );

	/*------------------------------------------------------------
			fun:	通过内存映射读取数据
			ret:	
			param: 
			remark:	注意大小不能超过内部限制的大小, 1024
	------------------------------------------------------------*/
	static tstring ReadMemMap( LPCTSTR szName );
protected:
	bool ReleaseTimer(MAP_TIMER_PARAM::iterator& itTimer, bool bClearMem = true);
	static VOID CALLBACK TimerCallback(PVOID lpParameter,BOOLEAN TimerOrWaitFired);
	bool SaveParam(TimerID nTimer, tagPTTimer* tagTimer );
	bool DealTimerCallback( PVOID lpParameter, BOOLEAN TimerOrWaitFired );
	bool IsExistTimer( TimerID pHTimer );
	tagPTTimer* GetParam( TimerID pHTimer );
	
private:
	MAP_TIMER_PARAM		m_mapTimerParam;	//不使用map, key值不能改变
	HANDLE			m_hTimerQueue;
	auto_ptr<CLock>				m_pLockTimerQueue;
	auto_ptr<CLock>				m_pLockMapList;
	auto_ptr<CLock>				m_pLockTimer;
	ENUM_TIMER_MODE				m_enTimerMode;
};

Pi_NameSpace_End
Pi_NameSpace_Using 