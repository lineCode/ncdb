#include "StdAfx.h"
#include "ShowData.h"
#include "define.h"
#include "doString.h"

using namespace std;

CShowData::CShowData(CWnd* pWnd)
{
	m_pWnd					= pWnd;
	m_pLineMap				= NULL;
	m_DeviceInfoMap			= NULL;
	m_pData					= NULL;
	m_iColumnCount			= 0;
}

CShowData::~CShowData(void)
{
	if (m_pLineMap)
	{
		delete m_pLineMap;
		m_pLineMap = NULL;
	}
	if (m_DeviceInfoMap)
	{
		delete m_DeviceInfoMap;
		m_DeviceInfoMap = NULL;
	}
	DeleteCriticalSection(&m_csDB);
}

bool CShowData::Init()
{
	InitializeCriticalSection(&m_csDB);

	m_pLineMap				= new LineMap;
	m_DeviceInfoMap			= new DeviceInfoMap;

	//显示控件为 CListView
	CListCtrl* pLT = (CListCtrl*)m_pWnd;
	wchar_t cColumnArr[][30] = {L"序号", L"设备编号",L"设备地址",L"Ip",L"车道数",
								L"车道ID",L"平均速",L"占有率",L"车头时距",
								L"车流量(大)",L"车流量(中)",L"车流量(小)",L"最后上传时间",};
	m_iColumnCount = _countof(cColumnArr);
	for (int i = 0;i < m_iColumnCount; ++i)
	{
		pLT->InsertColumn(i, cColumnArr[i], LVCFMT_RIGHT, 100);
	}

	//pLT->InsertItem(0, L" ");
	//pLT->InsertItem(1, L"item2");

	//pLT->SetItemText(0, 3, L"sbu1");

	//视图创建后,CListCtrl已经创建了,但可以用ModifyStyle修改样式
	pLT->ModifyStyle(0, LVS_REPORT);
	
	pLT->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_AUTOSIZECOLUMNS | LVS_EX_BORDERSELECT 
		 | LVS_EX_COLUMNSNAPPOINTS | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_HEADERINALLVIEWS
		 | LVS_EX_DOUBLEBUFFER);


	//初始化
	return true;
}

void CShowData::SetData(TrackShow* pTS)
{
	m_pData = pTS;

} 

bool CShowData::AdjustColumnWidth()
{
	CListCtrl* pLT = (CListCtrl*)m_pWnd;
	for (int i = 0; i < m_iColumnCount; ++i)
	{
		pLT->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}
	//pLT->SetColumnWidth(12, 200);
	return true;	
}

bool CShowData::ShowTemplateData()
{
	CListCtrl* pLT = (CListCtrl*)m_pWnd;
	
	UNINT iNum = 0;  
	int iRealIndex = -1;
	for (LineMap::iterator itLane = m_pLineMap->begin(); itLane != m_pLineMap->end();++itLane)
	{
		int iBebinPos = 1;
		WCHAR cTemp[5] = {0};
		++iNum;

		DeviceInfoMap::iterator itDevice = m_DeviceInfoMap->find(itLane->first);
		_itow(iNum, cTemp, 10);

		iRealIndex = pLT->InsertItem(itLane->second, cTemp);
		pLT->UpdateWindow();

		ShowSubTemplateData();

		pLT->SetItemText(iRealIndex, iBebinPos++, s2ws(itLane->first).c_str());
		pLT->SetItemText(iRealIndex, iBebinPos++, s2ws(itDevice->second.strSBDZ).c_str());
		pLT->SetItemText(iRealIndex, iBebinPos++, s2ws(itDevice->second.strIp).c_str());
		_itow(itDevice->second.byLaneCount, cTemp, 10);
		pLT->SetItemText(iRealIndex, iBebinPos++, cTemp);


		//显示id车道
		pLT->SetItemText(iRealIndex, iBebinPos, L"1");
		for (byte i = 1; i < itDevice->second.byLaneCount; ++i)
		{
			
			_itow(i + 1, cTemp, 10);
			iRealIndex = pLT->InsertItem(itLane->second + i, L" ");
			pLT->UpdateWindow();
			pLT->SetItemText(iRealIndex, iBebinPos, cTemp);
			pLT->UpdateWindow();
		}
		itLane->first;
	}
	return true;
}

bool CShowData::ShowSubTemplateData()
{
	return true;
}

bool CShowData::ShowData(DWORD dwLine)
{
	//EnterCriticalSection(&m_csDB);
	//TODO: Add your message handle code
	CListCtrl* pLT = (CListCtrl*)m_pWnd;
	static int i = 0;
	//显示数据到界面
	string strSBBH;
	string strLaneNum;
	string strSBDZ;
	string strIp;
	WCHAR cBuf[10] = {0};
	_ltow(i, cBuf,10);

	//pLT->ShowWindow(SW_HIDE);
	//pLT->DeleteItem(dwLine);

	//pLT->InsertItem(dwLine, L"问的");
	//pLT->SetItemText(dwLine, 2, L"hh");
	//pLT->SetItemText(dwLine, 3, L"hhfgfgfgf的");
	//pLT->SetItemText(dwLine, 4, L"hhfgf大幅度dfd34 sdsd");
	//pLT->SetItemText(dwLine, 5, cBuf);
	//pLT->SetItemText(dwLine, 6, L"hdfh");

	////pLT->ShowWindow(SW_SHOW);
	//++i;
	int iBeginPos = 5;
	//pLT->InsertItem(dwLine, s2ws(strSBBH).c_str());
	pLT->SetItemText(dwLine, iBeginPos++, s2ws(m_pData->loopId).c_str());
	pLT->SetItemText(dwLine, iBeginPos++, s2ws(m_pData->avgSpeed).c_str());
	pLT->SetItemText(dwLine, iBeginPos++, s2ws(m_pData->userRatio).c_str());
	pLT->SetItemText(dwLine, iBeginPos++, s2ws(m_pData->avgDistance).c_str());
	pLT->SetItemText(dwLine, iBeginPos++, s2ws(m_pData->carNumB).c_str());
	pLT->SetItemText(dwLine, iBeginPos++, s2ws(m_pData->carNumM).c_str());
	pLT->SetItemText(dwLine, iBeginPos++, s2ws(m_pData->carNumS).c_str());
	pLT->SetItemText(dwLine, iBeginPos++, s2ws(m_pData->tim).c_str());

	//LeaveCriticalSection(&m_csDB);
	return true;

}


DWORD CShowData::GetLineToShow(string strSBBH, byte laneId)
{
	LineMap::iterator itFind = m_pLineMap->find(strSBBH);
	DeviceInfoMap::iterator itDevice = m_DeviceInfoMap->find(strSBBH);
	DWORD dwLine = 0;
	DWORD laneCount = 0;
	const byte byLaneFromZero = 1;
	const byte byLineFromZero = 1;
	if (itFind != m_pLineMap->end())
	{
		// Finded!
		if(laneId > itDevice->second.byLaneCount)
		{
			throw(string("laneId > laneCount, invalid data"));
		}
		dwLine = itFind->second + laneId - byLaneFromZero;
	}
	else
	{
		throw(string("no find the strSBBH in map"));

		////insert to map
		//dwLine = (GetExistTotal() + 1 - byLineFromZero ) + (laneId - byLaneFromZero);
		//laneCount = 0;
		//ShowLine sl = {dwLine, laneCount};
		//LineMap::value_type vlpNew = LineMap::value_type(strSBBH, sl);
		//m_pLineMap->insert(vlpNew);
	}
	return dwLine;
}


bool CShowData::SetDeviceShowLaneMap(string strSBBH)
{
	LineMap::iterator itFind = m_pLineMap->find(strSBBH);
	DWORD dwNewShowLine = GetExistTotal();

	if (itFind != m_pLineMap->end())
	{
		itFind->second = dwNewShowLine;
	}
	else
	{
		//insert to map

		LineMap::value_type vlpNew = LineMap::value_type(strSBBH, dwNewShowLine);
		m_pLineMap->insert(vlpNew);
	}
	return true;
}

bool CShowData::SetDeviceInfoMap(string strSBBH, const DeviceInfo& di)
{
	DeviceInfoMap::iterator itDevice = m_DeviceInfoMap->find(strSBBH);

	if (itDevice != m_DeviceInfoMap->end())
	{
		// Finded!
		//itFind->second.laneCount = laneCount;
		itDevice->second = di;
	}
	else
	{
		//insert to map
		DeviceInfoMap::value_type vlpNew = DeviceInfoMap::value_type(strSBBH, di);
		m_DeviceInfoMap->insert(vlpNew);
	}
	return true;
}

DWORD CShowData::GetExistTotal()
{
	//获得Map里存在的所有对的值
	UNINT uTotal = 0;
	for (DeviceInfoMap::iterator it = m_DeviceInfoMap->begin(); it != m_DeviceInfoMap->end(); ++it)
	{
		uTotal += it->second.byLaneCount;
	}
	return uTotal;

}
