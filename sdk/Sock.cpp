#include "StdAfx.h"
#include "Sock.h"
//#include "MsgHandle.h"
#include "define_gnl.h"
//#include <string>
//#include "doString.h"
using namespace std;
#define SEND_SIZE 1024 * 50
#define RECV_SIZE 1024 * 50

CSock::CSock(SOCKET sock /*= 0*/, void* pAdd /*= 0*/, const SOCK_PARAM* pSP /*= NULL*/)
{
    m_nErrCode      = 0;
	m_pAdd			= pAdd;
	m_sock			= sock;
	m_port			= 0;
    if (pSP)
    {
        m_sockParam = *pSP;
    }
}

CSock::CSock(const CSock& sock)
{
	m_pAdd			= sock.m_pAdd; 

	m_sock			= sock.m_sock;
	m_strLocalIp	= sock.m_strLocalIp;
	m_port			= sock.m_port;
    m_nErrCode      = 0;
    m_sockParam     = sock.m_sockParam;
    m_sockInfo      = sock.m_sockInfo;
}

CSock::~CSock(void)
{
	if(SOCKET_ERROR != closesocket(m_sock))
	{
		m_sock = NULL;
		//DEBUG_ERROR(L"closesocket");
	}
	//if(!WSACleanup())
	//{
	//	//DEBUG_ERROR(L"WSACleanup");
	//}
}


bool CSock::Init()
{
	if (0 != m_sock)
	{
		DEBUG_WARN("m_sock != 0,close");
		closesocket(m_sock);
		m_sock = 0;
	}
	//先初始化套接字库
	m_sock = socket(AF_INET, m_sockInfo.sockType, 0);
	if (INVALID_SOCKET == m_sock)
	{
        if(!CSock::InitSockLib(2, 0))
		{
			//DEBUG_ERROR(L"InitSockLib");
			return false;
		}

		m_sock = socket(AF_INET, m_sockInfo.sockType, 0);
		if (INVALID_SOCKET == m_sock)
		{
			return false;
		}
	}
	return true;
}

SOCK_INFO CSock::SetSockInfo(const SOCK_INFO& si)
{
    SOCK_INFO siRet = m_sockInfo;
    m_sockInfo = si;
    return siRet;
}

bool CSock::InitSockLib(byte majorVer, byte minorVer)
{
	
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( majorVer, minorVer );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return false;
	}

	if ( LOBYTE( wsaData.wVersion ) != majorVer ||
		HIBYTE( wsaData.wVersion ) != minorVer ) 
	{
		WSACleanup( );
		return false; 
	}
	
	return true;
}


bool CSock::Bind(u_short uPort)
{
	SOCKADDR_IN addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(uPort);
	//addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_addr.S_un.S_addr = ADDR_ANY;
	if(SOCKET_ERROR == ::bind(
		m_sock, 
		(sockaddr*)&addr,
		sizeof(SOCKADDR_IN)
		)
	)
	{
        m_nErrCode = WSAGetLastError();
        return false;
	}
	int iAddrSize = sizeof(struct sockaddr);
	memset(&addr, 0, iAddrSize);
	
	char bufHostname[40] = {0};
	if(gethostname(bufHostname, sizeof(bufHostname)))
	{
		//DEBUG_ERROR(L"gethostname");
		return false;
	}

	hostent* pInfo = gethostbyname(bufHostname);
	if (!pInfo)
	{
		int iError = WSAGetLastError();
		//DEBUG_ERROR(L"gethostbyname");
		return false;
	}
	string s;
	in_addr  ia = {0};
	char cIp[20] = {0};
	if(strlen((char*)pInfo->h_addr_list))
	{
		ia.S_un.S_addr = *(u_long*)pInfo->h_addr_list[0];
		m_strLocalIp = inet_ntoa(ia);
	}else
	{
		//没网卡？
		//DEBUG_ERROR(L"no have any network ip");
		return false;
	}
	

	if(getsockname(
		m_sock, 
		(sockaddr*)&addr,
		&iAddrSize)
	)
	{
		int iErr = WSAGetLastError();
		//DEBUG_ERROR(getsockname + _itow(iErr,));
		return false;
	}
	char* pLocalIp = inet_ntoa(addr.sin_addr);
	m_port = ntohs(addr.sin_port);
	wchar_t wcPort[20] = {0};
	swprintf_s(wcPort, 20, L"port = %d\n", m_port);
	////DEBUG_INFO((wchar_t*)wcPort);
	OutputDebugStringW(wcPort );

    //设置socket 参数
    if(m_sockParam.bEnableSendTimeout)
    {
        if(SOCKET_ERROR == setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (PCSTR)&m_sockParam.nSendTimeout, sizeof(int)))
        {
            m_nErrCode  = WSAGetLastError();
            DealSetSockOptFail();
        }
    }

    if(m_sockParam.bEnableRecvTimeout)
    {
        if(SOCKET_ERROR == setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (PCSTR)&m_sockParam.nRecvTimeout, sizeof(int)))
        {
            m_nErrCode  = WSAGetLastError();
            DealSetSockOptFail();
        }
    }

    if (m_sockParam.bSetSendBuf)
    {
        if(SOCKET_ERROR == setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (PCSTR)&m_sockParam.nSendBuf, sizeof(int)))
        {
            m_nErrCode  = WSAGetLastError();
            DealSetSockOptFail();
        }
    }
    
	return true;
}

SOCKET CSock::GetSock()
{
	return m_sock;
}

void CSock::SetSock(SOCKET sock)
{
	m_sock = sock;
}

const char* CSock::GetLocalIp()
{
	return m_strLocalIp.c_str();
}

u_short CSock::GetLocalPort()
{
	return m_port;
} 

bool CSock::Close()
{
	if(0 == closesocket(m_sock))
	{
		m_sock = 0;
		return true;
	}
	return false;
}


void CSock::SetErrDesc(string strErr)
{
    m_strErrDesc = strErr;
}

const string& CSock::GetErrDesc()
{
    return m_strErrDesc;
}

int CSock::GetErrCode()
{
    return m_nErrCode;
}

void CSock::SetErrCode(int nErrCode)
{
    m_nErrCode = nErrCode;
}

void CSock::DealSetSockOptFail()
{
    
}

//CSock* CSock::Clone()
//{
//	CSock* pNew = new CSock(*this);
//	return pNew;
//}