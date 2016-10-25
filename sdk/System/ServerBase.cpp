#include "ServerBase.h"
#include "EventLight.h"
#include "PiString.h"

static SERVICE_STATUS_HANDLE g_hServerReg;
static CEventLight		g_eventExit;
static SERVICE_STATUS	g_serverStutus = {0};
static CServerBase*		g_pServer;

Pi_NameSpace_Using

CServerBase::CServerBase(void)
{
	g_pServer = this;
	m_nPeriodMils = 2000;
	m_bWriteLog = true;
}

CServerBase::~CServerBase(void)
{
}

bool CServerBase::Init()
{
	g_eventExit.Init();
	if(m_bWriteLog)
	{
		CSingleLog::Init();
		tstring strTempFld;

		tstring strExpand(_T("%TEMP%"));
		TCHAR szBuffer[MAX_PATH];
		ExpandEnvironmentStrings(strExpand.c_str(), szBuffer, MAX_PATH);
		strTempFld = szBuffer;
		strTempFld += _T("\\PiLog");
		CSingleLog::OpenTodayLog(strTempFld.c_str());
	}

	

	return true;
}

bool CServerBase::Start()
{
	CSingleLog::Init();
	CSingleLog::WriteLog(_T("ServerBase模块开始执行服务"));
	SERVICE_TABLE_ENTRY DispatchTable[]={{const_cast<tpchar>(m_strServiceName.c_str()),ServiceMain}, {NULL, NULL}};  
	StartServiceCtrlDispatcher(DispatchTable);
	return true;
}


VOID WINAPI CServerBase::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	g_hServerReg = ::RegisterServiceCtrlHandler(const_cast<tpchar>(g_pServer->m_strServiceName.c_str()), ServerHandler);

	//初始化
	g_pServer->Init();
	CSingleLog::WriteLog(_T("ServiceMain start"));

	g_serverStutus.dwServiceType = SERVICE_WIN32_OWN_PROCESS /*| SERVICE_INTERACTIVE_PROCESS*/;

	g_serverStutus.dwCurrentState = SERVICE_START_PENDING;
	g_serverStutus.dwControlsAccepted = 0;
	g_serverStutus.dwWaitHint = 1000;
	if(!SetServiceStatus(g_hServerReg, &g_serverStutus))
	{
		CSingleLog::WriteLog(_T("SetServiceStatus start_pending fail"));
	}

	g_pServer->InitThread();
	CSingleLog::WriteLog(_T("ServiceMain init ok"));

	g_serverStutus.dwCurrentState = SERVICE_RUNNING;
	g_serverStutus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	SetServiceStatus(g_hServerReg, &g_serverStutus);

	//执行任务

	while(!g_eventExit.GetLock(g_pServer->m_nPeriodMils))
	{
		g_pServer->DoTask();
	}
	CSingleLog::WriteLog(_T("即将退出服务入口, 做反初始化操作"));
	g_pServer->OnExit();
	g_serverStutus.dwCurrentState = SERVICE_STOPPED;
	g_serverStutus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	if(!SetServiceStatus(g_hServerReg, &g_serverStutus))
	{
		CSingleLog::WriteLog(_T("SetServiceStatus stoped fail"));
	}
}


VOID WINAPI CServerBase::ServerHandler(DWORD fdwControl)
{
	CPiString str;
	str.Format(_T("服务收到控制码:%d"), fdwControl);
	CSingleLog::WriteLog(str);
	bool bExit = false;
	g_serverStutus.dwCurrentState = 0;
	switch(fdwControl)
	{
	case SERVICE_CONTROL_STOP:
		CSingleLog::WriteLog(_T("receive code: stop"));
		g_serverStutus.dwCurrentState = SERVICE_STOP_PENDING;
		g_serverStutus.dwControlsAccepted = 0;		//正在退出时不接受其他控制
		bExit = true;

		/*g_serverStutus.dwCurrentState = SERVICE_STOPPED;
		g_serverStutus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
		SetServiceStatus(g_hServerReg, &g_serverStutus);*/

		break;
	default:
		break;
	}
	
	if(!g_serverStutus.dwCurrentState)
	{
		return ;
	}
	g_serverStutus.dwWaitHint = 0;

	if(!SetServiceStatus(g_hServerReg, &g_serverStutus))
	{
		CSingleLog::WriteLog(_T("set status fail"));
	}

	if(bExit)
	{
		g_eventExit.SetEventSign();
	}

	return ;
}

bool CServerBase::DoTask()
{
	CSingleLog::WriteLog(_T("服务执行中"));		
	return true;
}

bool CServerBase::InitThread()
{
	throw std::exception("The method or operation is not implemented.");
}

void CServerBase::OnExit()
{
	
}
