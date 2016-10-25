#include "stdafx.h"
#include "DataDeal.h"
#include "LogOpr.h"
#include "define.h"
#include "DealWithDB.h"
#include "functional.h"
#include <iostream>
#include "SingleLog.h"
#include "doString.h"
#include "ShowData.h"
static CLogOpr* g_pSingleLog;

using namespace std;
CDataDeal::CDataDeal(void* pAdd /*= NULL*/, CADODataBase* pDB /*= 0*/)
{
	m_pTInfo		= NULL;
	m_bPrivateDB	= false;

    m_pAdd = pAdd;
	m_pDB = pDB;
	if (!pDB)
	{
		m_bPrivateDB = true;
		m_pDB = new CADODataBase;
	}
	
	g_pSingleLog = CSinglePattern<CLogOpr>::GetObj();
}

CDataDeal::~CDataDeal(void)
{
	if (m_bPrivateDB && m_pDB)
	{
		delete m_pDB;
		m_pDB = NULL;
	}
	if (m_pTrackShow)
	{
		delete m_pTrackShow;
		m_pTrackShow = NULL;
	}
}

bool CDataDeal::Init()
{
	
    m_pTrackShow = new TrackShow;
    memset(m_pTrackShow, 0, sizeof(TrackShow));

	m_pDB->InitDB();
	
	return m_pDB->ConnectionDB();
}

bool CDataDeal::WriteLog()
{	
	/*CLogOpr log;
	log.OpenTodayLog("c:/log/");
	log.Write("111", 4);*/
	return false;
}

bool CDataDeal::SetDeviceLaneMap(CShowData* pSD)
{
	string strSql("select * from rfid_t_site_hair where PID = 100");
	_RecordsetPtr rs = m_pDB->ADOSelect(strSql);
	DeviceInfo di ;
	
	for (;!rs->adoEOF;rs->MoveNext())
	{
		wstring strSBBH		= rs->GetCollect("DDBH").bstrVal;
		wstring strSBDZ		= rs->GetCollect("NAME").bstrVal;
		wstring strIP		= rs->GetCollect("IP").bstrVal;
		byte bylaneCount	= rs->GetCollect("CDS").bVal;
		
		string strTemp	= ws2s(strSBBH);

		di.byLaneCount	= bylaneCount;
		di.strSBDZ		= ws2s(strSBDZ);
		di.strIp		= ws2s(strIP);

		pSD->SetDeviceShowLaneMap(strTemp);
		pSD->SetDeviceInfoMap(strTemp, di);
		//string strSBBH2 = (char*)rs->GetCollect("SBBH").pcVal;
	}
	
	return true;
}

bool CDataDeal::SetData(TrafficInfo* pData)
{
	m_pTInfo = pData;
	return true;
}

bool CDataDeal::DealData()
{

	BYTE loopIdPos = 2;//跳过一个方向的数据
	for (int i = 0;i<32;i += loopIdPos)
	{
		if(0 == m_pTInfo->info[i].loopId)
		{
			break;
		}

		SwitchData(i);
		if(InsertToDB(i))
		{
			ShowToUI();
		}
		
		
	}

	return true;
}

bool CDataDeal::InsertToDB(byte byCount)
{
	
	char cSql[1024 * 5] = {0};
	//string strNow = GetLocalDateTime("%04d%02d%02d%02d%02d%02d");
	string strNow = m_pTInfo->tim;
	StrReplace(strNow, " ");
	StrReplace(strNow, "-");
	StrReplace(strNow, ":");

	if(0 == m_pTInfo->info[byCount].loopId)
	{
		return false;
	}


	string pDeviceType = "deviceType"/*GetDeviceType()*/;

	sprintf_s(cSql, 
		sizeof(cSql), 
		//20个字段
		"insert into T_TFM_DETECTOR_DATA_I(\
        DETECTORID,  DETECTORTYPE, DETCODE,        DATETIME, TIMESPAN, VOLUME, SPEED, \
			TIMEOPY,HEADTIME,HEADDISTANCE,FLOWRATE,	    SATURATION,	DENSITY,AVGLENGTH,QUEUELENGTH,      VOLUME1,VOLUME3,VOLUME5, STATE) \
		VALUES('%s','%s','%s',     to_date(%s, 'yyyy-mm-dd hh24:mi:ss'), %3d, %s, %s,\
		%s,%s,%7.3f,%7.3f,    %7.3f,%7.3f,%7.3f,%7.3f,    %s,%s,%s,'%s')",

		m_pTrackShow->devsn, pDeviceType.c_str(), m_pTrackShow->loopId,
		strNow.c_str(), 60/*上报间隔*/, m_pTrackShow->carTotal, m_pTrackShow->avgSpeed,
		m_pTrackShow->userRatio, m_pTrackShow->avgDistance, 0.0, 0.0, 
		0.0, 0.0, 0.0, 0.0,
		m_pTrackShow->carNumS, m_pTrackShow->carNumM, m_pTrackShow->carNumB, "0"
	);
	
	g_pSingleLog->WriteLog(cSql);
	try
	{
		if(m_pDB->ADOUpdata(cSql))
		{
			g_pSingleLog->WriteLog("已写入数据库");
		}
	}
	catch(string& strErr)
	{
		if (strErr.find("中断") != string::npos)
		{
			g_pSingleLog->WriteLog(strErr.c_str());
			g_pSingleLog->WriteLog("尝试连接,并再次写入");
			m_pDB->ReSetCnState();

			if(m_pDB->ConnectionDB())
			{
				if(m_pDB->ADOUpdata(cSql))
				{
					g_pSingleLog->WriteLog("已写入数据库");
				}
			}
		}
		else
		{
			int a = 0;
			return false;
		}
		
	}
	return true;
}



bool CDataDeal::ShowToUI()
{
	//m_pTInfo
    CWnd* pMain = (CWnd*)m_pAdd ;
    pMain->SendMessage(UM_SHOW_DATA, (WPARAM)m_pTrackShow, (LPARAM)GetCurrentThreadId());
    return true;
}


string CDataDeal::GetDeviceType(const char* cFlag)
{
	string strSql("select * from T_RFID_VIDEODEVICE where SBBH = ");
	strSql += cFlag;
	strSql = m_pDB->GetSelectValueStr(strSql, "SBLX");

	return strSql;
}


bool CDataDeal::SwitchData(int loopID)
{

    memset(m_pTrackShow, 0, sizeof(TrackShow));
	
	if (m_pTInfo->info[loopID].loopId < 0 ||  m_pTInfo->info[loopID].loopId > 999)
	{
		memset(m_pTInfo, 0 , sizeof(TrafficInfo));
		m_pTInfo->info[loopID].loopId = -1;
	}

    memcpy_s(m_pTrackShow->tag, sizeof(m_pTrackShow->tag), m_pTInfo->tag, sizeof(m_pTInfo->tag));
    memcpy_s(m_pTrackShow->devsn, sizeof(m_pTrackShow->devsn), m_pTInfo->devsn, sizeof(m_pTrackShow->devsn) -1);
    memcpy_s(m_pTrackShow->tim, sizeof(m_pTrackShow->tim), m_pTInfo->tim, sizeof(m_pTInfo->tim));
	
	
    sprintf_s(m_pTrackShow->loopId,     sizeof(m_pTrackShow->loopId),       "%3d", m_pTInfo->info[loopID].loopId);
    sprintf_s(m_pTrackShow->avgSpeed,   sizeof(m_pTrackShow->avgSpeed),     "%7.3f", m_pTInfo->info[loopID].avgSpeed / 1000.0);
    sprintf_s(m_pTrackShow->userRatio,  sizeof(m_pTrackShow->userRatio),    "%5.3f", m_pTInfo->info[loopID].userRatio/ 1000.0);
    sprintf_s(m_pTrackShow->avgDistance, sizeof(m_pTrackShow->avgDistance), "%6.3f", m_pTInfo->info[loopID].avgDistance/1000.0);
    sprintf_s(m_pTrackShow->carNumB,    sizeof(m_pTrackShow->carNumB),      "%3d", m_pTInfo->info[loopID].carNumB);
    sprintf_s(m_pTrackShow->carNumM,    sizeof(m_pTrackShow->carNumM),      "%3d", m_pTInfo->info[loopID].carNumM);
    sprintf_s(m_pTrackShow->carNumS,    sizeof(m_pTrackShow->carNumS),      "%3d", m_pTInfo->info[loopID].carNumS);
    sprintf_s(m_pTrackShow->carTotal,   sizeof(m_pTrackShow->carTotal),     "%3d", m_pTInfo->info[loopID].carNumB + m_pTInfo->info[loopID].carNumM + m_pTInfo->info[loopID].carNumS);
    //memcpy_s(m_pTrackShow->loopId, sizeof(TrackShow), m_pTInfo->info[loopID].loopId);
    return true;
}