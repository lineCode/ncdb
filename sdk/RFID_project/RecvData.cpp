#include "stdafx.h"
#include "RecvData.h"
#include "DataDeal.h"
#include "TcpDB.h"
#include "define.h"
#include <iostream>
#include "RDIni.h"
#include "SinglePattern.h"
#include <string>
#include "functional.h"
//#include "LogOpr.h"
#include "SingleLog.h"
#include "ShowData.h"
#include "MainFrm.h"
#include "DealWithDB.h"

using namespace std;

CRecvData::CRecvData(void)
{
	m_pDataDeal = NULL;
	m_pTcp		= NULL;
	m_pShowData = NULL;
	m_pDB		= NULL;
	m_pAdd		= NULL;
}


CRecvData::~CRecvData(void)
{
	if (m_pTcp)
	{
		delete m_pTcp;
	}

	if(m_pDataDeal)
	{
		delete m_pDataDeal;
	}

	if (m_pShowData)
	{
		delete m_pShowData;
	}	
	if (m_pDB)
	{
		delete m_pDB;
	}
}

bool CRecvData::Init(void* pAdd /*= 0*/)
{
	CLogOpr* pSingleLog = CSinglePattern<CLogOpr>::GetObj();
	pSingleLog->OpenTodayLog();

	m_pAdd = (CWnd*)pAdd;
	pSingleLog->WriteLog("日志初始化完成");
	pSingleLog->WriteLog("可以log");
	//CIni ini(CONFIG_FILE);

	wstring strConfig = GetCurrentDirIniName();
	CRDIni ini(strConfig.c_str());
	DWORD dwTemp = sizeof(CIni);
	u_short uPort = _wtol(ini.ReadParam(L"config", L"localPort"));
	
	//全局唯一的数据库对象
	m_pDB = new CADODataBase;
	if(m_pDB->InitDB())
	{
		if(m_pDB->ConnectionDB())
		{
			pSingleLog->WriteLog("数据库初始化失败");
			return false;
		}
	}


	m_pDataDeal = new CDataDeal(pAdd, m_pDB);
	m_pDataDeal->Init();

    pSingleLog->WriteLog("数据库初始化完成");

	m_pTcp = new CTcpDB(0, this/*m_pDataDeal*/);
	if (!m_pTcp)
	{
		return false;
	}

	m_pTcp->Init();
	if(!m_pTcp->InitAll(true, uPort))
	{
		return false;
	}

	//显示数据对象
	
	
	m_pShowData = new CShowData(((CMainFrame*)pAdd)->GetListCtrl());
	m_pShowData->Init();
	m_pDataDeal->SetDeviceLaneMap(m_pShowData);

	m_pShowData->ShowTemplateData();
	m_pShowData->AdjustColumnWidth();

	m_pShowData->GetLineToShow("0571000000", 2);
	m_pShowData->GetLineToShow("0575600000", 3);

	return true;
}

bool CRecvData::UnInit()
{
	m_pTcp->UnInit();
	return true;
}

bool CRecvData::Work()
{
	/*TrafficInfo* pTInfo = new TrafficInfo;
	
	DWORD dwSize = sizeof(TrafficInfo);
	memset(pTInfo, 0 ,dwSize);*/
	/*if(!m_pDataDeal.Init())
	{
		return false;
	}*/

	//while(1)
	//{
	//	try
	//	{
	//		//memset(pTInfo, 0 ,sizeof(TrafficInfo));
	//		////cout<<"准备\n";
	//		////m_pTcp->Recv(pBuf, dwSize);
	//		////cout<<"接收到数据了";
	//		//m_pDataDeal->SetData((TrafficInfo *)pBuf);
	//		//m_pDataDeal->InsertToDB();
	//	}
	//	catch(...)
	//	{
	//		delete pTInfo;

	//		return false;
	//	}
	//	Sleep(5);

	//}
	return true;
}


bool CRecvData::ShowToUI(TrackShow* ts)
{
	m_pShowData->SetData(ts);
	DWORD dwLine = m_pShowData->GetLineToShow(ts->devsn, atoi(ts->loopId));

	return m_pShowData->ShowData(dwLine);
}	