#include "stdafx.h"
#include "TcpDB.h"
#include "DataDeal.h"
#include "define.h"
#include "SingleLog.h"
#include "StlSupply.h"
#include "RecvData.h"
#include "SockClientManager.h"

static CLogOpr* g_pSingleLog;

CTcpDB::CTcpDB(SOCKET sock/* = 0*/, void* pAdd /*= 0*/)
:CSockTcp(sock, pAdd)
{
	g_pSingleLog = CSingleLog::GetObj();
}

CTcpDB::~CTcpDB(void)
{
}

bool CTcpDB::Listen()
{
    if(!CSockTcp::Listen(SOMAXCONN))
    {
        g_pSingleLog->WriteLog("监听失败");
        return false;
    }
    return true;
}

SOCKET CTcpDB::Accept(const char* ip /* = 0 */, u_short port /* = 0 */)
{
	SOCKET s = CSockTcp::Accept();
	if (s)
	{

		g_pSingleLog->WriteLog("收到连接了");
	}
	else
	{
		string strTemp("收到的连接异常");
	}
	return s;

}

DWORD CTcpDB::Recv(char* &pBuf)
{
	DWORD dwSize = sizeof(TrafficInfo);
	pBuf = new char[dwSize];
	memset(pBuf, 0, dwSize);
	dwSize = RecvData(pBuf, dwSize);
	if (dwSize)
	{
		g_pSingleLog->WriteLog("收到  数据");
	}
	return dwSize;
}

bool CTcpDB::DoDealDataRecv(void* pBuf, DWORD dwRecv)
{
	//OutputDebugString(L"recved data from client!!");
	try
	{
		CRecvData* pRD = (CRecvData*)m_pAdd;
		CDataDeal dataDeal(pRD->m_pAdd, pRD->m_pDB);
		dataDeal.Init();
		//CDataDeal* pDataDeal = (CDataDeal*)(this->m_pAdd);
		dataDeal.SetData((TrafficInfo*)pBuf);
		dataDeal.DealData();
		//pDataDeal->ShowToUI();
		//删掉元素 从监听socket 接收到客户连接的列表
		m_pSockClientManager->Delete(GetSock());
		
		this->Close();
	}
	catch(string& err)
	{
		g_pSingleLog->WriteLog(err.c_str());
	}
	catch(...)
	{
		g_pSingleLog->WriteLog("捕获到未知的错误");
	}
	
	return true;
}

CSock* CTcpDB::Clone()
{
	return new CTcpDB(*this);
	
}