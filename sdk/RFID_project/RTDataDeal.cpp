#include "stdafx.h"
#include "RTDataDeal.h"
#include "LogOpr.h"
#include "RFID.h"
#include "DealWithDB.h"
#include "functional.h"
#include <iostream>
#include "doString.h"
#include "RFID_ShowData.h"
#include <sstream>
#include "SingleLog.h"
#include "SockTcp.h"
#include "TcpRFID.h"
#include "NativeDataDeal.h"
#include "resource.h"
#include "RFID_Data_ServerDlg.h"
extern CTimeLight g_at;

using namespace std;
CRTDataDeal::CRTDataDeal(void* pAdd /*= NULL*/)
{
    m_pDlg          = static_cast<CRFID_Data_ServerDlg*>(pAdd);
	m_pTrackShow	= NULL;
	m_pTagInfo		= NULL;
	m_pLog			= NULL;
	m_pNativeData	= NULL;
    m_NativeDataDeal = NULL;
}

CRTDataDeal::CRTDataDeal(CRTDataDeal& dd)
{
	m_pLog = dd.m_pLog;
    m_pDlg = dd.m_pDlg;

	m_pNativeData       = NULL;
    m_NativeDataDeal    = NULL;
	m_pTagInfo          = NULL;
	m_dwDevicePos       = 0;
	
}

CRTDataDeal::~CRTDataDeal(void)
{

	if (m_pTrackShow)
	{
		delete m_pTrackShow;
		m_pTrackShow = NULL;
	}
	if (m_pTagInfo)
	{
		delete m_pTagInfo;
		m_pTagInfo = NULL;
	}
    if (m_NativeDataDeal)
    {
        delete m_NativeDataDeal;
        m_NativeDataDeal = NULL;
    }
}

bool CRTDataDeal::Init(void* p)
{
	CLogOpr* pSingleLog = CSinglePattern<CLogOpr>::GetObj();
    m_pTrackShow = new TrackShow;
	m_pTagInfo = new TagInfo;
    memset(m_pTrackShow, 0, sizeof(TrackShow));


	CTcpRFID* pTcp	= static_cast<CTcpRFID*>(p);
	string strIp	= pTcp->GetClientIp();

	try
	{
		SetDevicePos(strIp.c_str());
	}
	catch(string& strErr)
	{
		CSingleLog::WriteLog(strErr.c_str());
        return false;
	}
	
	/*m_NativeDataDeal = new CNativeDataDeal(m_pDlg, NULL);
	m_NativeDataDeal->Init(&g_vtReaderInfo[m_dwDevicePos]);*/

	return true;
}

IDataDeal* CRTDataDeal::Clone()
{
	CRTDataDeal* pNew = new CRTDataDeal(*this);
	return pNew;
}

bool CRTDataDeal::Work(void* pParam)
{
    CAutoType at;
    CTimeLight timeTag;
    timeTag.Init();
    timeTag.Start();
	EnterCriticalSection(&g_cstTagInfo[m_dwDevicePos]);
    timeTag.Stop();
    
    
    //排除天线ID是否有错误
    for (UINT i = 0; i < m_pNativeData->tagData.byTagCount;++i)
    {
        if (m_pNativeData->tagData.pTddArr[i].byAntennaID > g_vtReaderInfo[m_dwDevicePos].nCDS )
        {
            m_oss.str("");
            m_oss<<INFO_ERROR <<g_vtReaderInfo[m_dwDevicePos].strIP<<" 原数据车道ID大于该设备的车道号"
                <<":"<<(int)m_pNativeData->tagData.pTddArr[i].byAntennaID;
            
            m_pDlg->PutLog(m_oss.str());

            //扔掉该错误数据
            LeaveCriticalSection(&g_cstTagInfo[m_dwDevicePos]);
            return false;
            
        }
    }
    
    g_vtNative[m_dwDevicePos].push_back(m_pNativeData);
    LeaveCriticalSection(&g_cstTagInfo[m_dwDevicePos]);

	return true;
}

void CRTDataDeal::SetData(void* pData)
{
	m_pNativeData = (RECV_DATA_TCP_SET*)pData;
}


bool CRTDataDeal::WriteDataToLog()
{
	/*CLogOpr* pSingleLog = CSinglePattern<CLogOpr>::GetObj();
	string strLog;
	char cBuf[100] = {0};
	itoa(m_pNativeData->byAntennaID, cBuf, 10);
	strLog += "antennaId = ";
	strLog += cBuf;

	strLog += "		EPC = ";
	strLog += ws2s(m_pTagInfo->strEPC.GetBuffer());

	strLog += "	epcRCnt = ";
	itoa(m_pNativeData->byReadCnt, cBuf, 10);
	strLog +=  cBuf;
	
	strLog += "	timestamp = ";
	strLog += ws2s(m_pTagInfo->RecvTime.GetBuffer());
	
	CSingleLog::WriteLog(strLog.c_str());*/
	return true;
}


bool CRTDataDeal::SetDevicePos(const char* DeviceIp)
{
	int pos = 0;
	bool bFind = false;
	for (vector<ReaderInfo>::iterator it = g_vtReaderInfo.begin();it != g_vtReaderInfo.end(); ++it)
	{
		if (it->strIP == DeviceIp)
		{
			bFind = true;
			break;
		}
		++pos;
	}

	/*test
	bFind = true;
	pos = 0;
	test*/
	if (!bFind)
	{
        throw(string("[WARNING] || 收到未知设备的数据:") + string(DeviceIp));
	}
	m_dwDevicePos = pos;
	return true;
}
