#include "StdAfx.h"
#include "SockTcp.h"
//#include "MsgHandle.h"
#include "define_gnl.h"
#include <assert.h>
#include <process.h>
#include "SockClientManager.h"
#include "IDataDeal.h"
#include "Lock.h"
#include "Stlmath.h"
#include <sstream>
#include <algorithm>
#include "StlSupply.h"
#include <MSTcpIP.h>
//#include <string>
#include "doString.h"
#include <iostream>
using namespace std;
uintptr_t __stdcall WaitForClientMsgProc	(LPVOID lpPar);
uintptr_t __stdcall WaitForConnectProc		(LPVOID lpPar);

#define SEND_SIZE 1024 * 50
#define RECV_SIZE 1024 * 50

typedef     CSockTcp::CONNECT_RESULT        CONNECT_RESULT;
typedef     CSock::SEND_STATE               SEND_STATE;
typedef     CSock::RECV_STATE               RECV_STATE;

using namespace ns_sock_tcp;

CSockTcp::CSockTcp(SOCKET sock /*= 0*/, void* pAdd /*= 0*/, const SOCK_PARAM* pSP /*= NULL*/)
:CSock(sock, pAdd, pSP)
{
	//调用一个公用的借口来初始化变量
    InitVariable();
}


CSockTcp::CSockTcp(const CSockTcp& tcp)
:CSock(tcp)
{
	//只拷贝基类部分
    InitVariable();
	m_pSockClientManager = tcp.m_pSockClientManager;
	if (tcp.m_pDataDeal)
	{
		m_pDataDeal = tcp.m_pDataDeal->Clone();     //创建一个新的数据处理对象
	}

    //保存客户端地址和Ip
    m_strClientIp   = tcp.m_strLastClientIp; 
    m_uClientPort   = tcp.m_uClientPort;
    SetSock(tcp.m_sockLastRecv);

}   
void CSockTcp::InitVariable()
{
    m_bCnServer             = false;
    m_bNeedRebind           = false;

    m_pDataDeal             = NULL;
    m_pThreadArgu			= NULL;
    m_hListenThread			= 0;

    m_strServerIp			= "";
    m_pDataLock             = NULL;
    m_pSockClientManager	= NULL;
    m_LastRecvSize          = 0;
    m_uServerPort			= 0;
    m_bCnServer					= false;
    m_bStopClientRecv       = false;

    m_iParam.bServer        = false;
    m_iParam.uPort          = 0;
    m_iParam.bDisFilter     = false;
    m_iParam.bKeepalive     = true;
    m_iParam.nKeepalivetime = NUM_TCP_KEEPALIVE_TIMEOUT;

}
//CSockTcp& CSockTcp::operator =(const CSockTcp& tcp)
//{
//
//}

CSockTcp::~CSockTcp(void)
{
	//终止监听线程,以后修改为
	//TerminateThread(TerminateThread, 0);
    if (m_iParam.bServer)
    {
        if (m_hListenThread)
        {
            CloseHandle(m_hListenThread);
            m_hListenThread = NULL;
        }
        
        //关闭所有客户端连接， 以及相应的线程
        //m_pSockClientManager->CloseAll();
    }
    else
    {
        //释放该客户socket from ClientList
        //m_pSockClientManager->Delete(this->GetSock());
    }
	
	
	if (m_pThreadArgu)
	{
		delete m_pThreadArgu;
		m_pThreadArgu = NULL;
	}
	if (m_pDataDeal)
	{
		delete m_pDataDeal;
		m_pDataDeal = NULL;
	}
    if (m_pDataLock)
    {
        delete m_pDataLock;
    }
	//关闭每个对象
	//m_pVtClientsSock.
}


bool CSockTcp::Init()
{
    SOCK_INFO si = {SOCK_STREAM};
    SetSockInfo(si);	

    if(!CSock::Init())
    {
        SetErrDesc("InitSockLib");
        return false;
    }
    if (m_iParam.bServer)
    {
        //客户管理类
        m_pSockClientManager = new CSockClientManager;

        CSockClientManager::INIT_PARAM iParam;
        iParam.bDisFilter   = m_iParam.bDisFilter;

        m_pSockClientManager->SetInitParam(iParam);
        m_pSockClientManager->Init();

        if (!m_pDataLock)
        {
            SetDataLock();
            //[ERROR] || dataLock is null!
        }
        
    }

    if(!Bind(m_iParam.uPort))
    {
        return false;
    }

    
	return true;
}

bool CSockTcp::UnInit()
{
    if(m_iParam.bServer)
    {
        m_pDataLock->Lock();
        m_bStopListen = true;
        m_pDataLock->UnLock();

        if( WAIT_TIMEOUT != WaitForSingleObject(m_hListenThread, 1000) )
        {
            m_pSockClientManager->UnInit();
        }
        else
        {
            ostringstream oss;
            oss<<INFO_WARNING<<"等待监听线程关闭超时";
            OutputDebugStringA(oss.str().c_str());
        }

        if (m_pSockClientManager)
        {
            delete m_pSockClientManager;
        }

        
    }

	return true;
}


bool CSockTcp::Close()
{
    if(CSock::Close())
    {
        m_bCnServer = false;
    }
    return true;
}


bool CSockTcp::SetInitParam(const INIT_PARAM& iParam)
{
    m_iParam = iParam;
    return true;
}

bool CSockTcp::Bind(u_short port /*= 0*/)
{
    if(!CSock::Bind(port))
	{
        ostringstream           oss;
        oss<<"绑定端口 "<<port <<"失败,错误代码为"<<GetErrCode();
        SetErrDesc(oss.str());
		return false;
	}

    //设置socket 参数

	return true;
}

bool CSockTcp::StartListen()
{
    if (!m_iParam.bServer)
    {
        return false;
    }

    if(!WaitConnect())
    {
        return false;
    }
    return true;
}

void CSockTcp::SetDataLock(CLock* pLock /*= NULL*/)
{
    if (!pLock)
    {
        m_pDataLock = new CLock;
    }
}

bool CSockTcp::SetMaxCnCount(DWORD dwCount)
{
	m_dwMaxCnCount = dwCount;
	return true;
}

CSockClientManager* CSockTcp::GetClientManager()
{
    return m_pSockClientManager;
}


bool CSockTcp::WaitConnect()
{
    assert(m_iParam.bServer);
    assert(m_pDataDeal);

    m_bStopListen = false;
	m_hListenThread = (HANDLE)_beginthreadex(
        NULL, 
		0,
		WaitForConnectProc, 
		this,
		0,
		0
	);
	if (NULL == m_hListenThread)
	{
		return false;
	}

	return true;
}

bool CSockTcp::ReConnect()
{
    //排除禁止的情况
    if(m_iParam.bServer)
    {
        assert(0 && "server self can't connect to another");
        return false;
    }


    if (m_bCnServer)
    {
        return false;
    }

    if (m_bNeedRebind)
    {
        this->Close();
        CSock::Init();
        if(!this->Bind(m_iParam.uPort)) //按以前的端口0 或 指定端口来绑定
        {
            return false;
        }
    }
    
    bool bRet = this->Connect();
    
    return bRet;
}

bool CSockTcp::Connect(const char* ip /*= ""*/, u_short port /*= 0*/)
{

    SetNonBlockMode(true);

	SOCKET sock = CSock::GetSock();
	if (m_bCnServer)
	{
        this->Close();
		//DEBUG_INFO(L"already connect,new connect is undo");
		return true;
	}

	if (!strlen(ip))
	{
		ip = m_strServerIp.c_str();
	}
	port = (port == 0 ? m_uServerPort : port);

	SOCKADDR_IN addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip);

    bool bCnSuccess     = true;
	if(SOCKET_ERROR == ::connect(sock, (sockaddr*)&addr, sizeof(SOCKADDR_IN)))
	{
        int nErr = WSAGetLastError();
        this->SetErrCode(nErr);

        m_bNeedRebind   = false;    //重连失败不需要重新绑定端口

        switch(nErr)
        {
        case WSAEWOULDBLOCK:
            //判断超时是否发生
            
            if(SelectWrite(sock, m_iParam.nCnTimeout))
            {
                //连接上了
                int i = 0;
            }
            else
            {
                /************************************************************************/
                /* 超时, 连接失败, 先不关闭socket,  如果说连接上的那刻, 刚好发生超时, 
                    那可能需要关闭socket, 不然下次再进行连接, 变成在一个已连接上的socket上 再次connect
                /************************************************************************/
                this->SetErrCode(err_connect_timeout);
                //int idfdf = WSAGetLastError();
                bCnSuccess  = false;
            }
            break;
        case WSAEISCONN:
            //由于之前超时, 但之后已经连接上了
            break;

        //由于设置超时, 连接超时返回后,再次调用connect, 但之前的连接还没有完成
        case WSAEALREADY:
        case WSAEINVAL:
            this->SetErrCode(err_connect_last_no_completion);
        default:
            bCnSuccess = false;
        }
	}

    SetNonBlockMode(false);
    //没连接上就直接返回
    if (!bCnSuccess)
    {
        m_bCnServer     = false;
        return bCnSuccess;
    }



	int iAddrSize = sizeof(SOCKADDR_IN);
	memset(&addr, 0, iAddrSize);
	if(getsockname(
		sock , 
		(sockaddr*)&addr,
		&iAddrSize)
		)
	{
		int iErr = WSAGetLastError();
		//DEBUG_ERROR(getsockname + _itow(iErr,))
	}
	char* pLocalIp = inet_ntoa(addr.sin_addr);
	;
	u_short uPort = ntohs(addr.sin_port);
	wchar_t Buf[50] = {0};

	m_bCnServer           = true;
	return true;
}


CONNECT_RESULT CSockTcp::Accept(const char* ip /*= 0*/, u_short port /*= 0*/)
{
	SOCKET sock = CSock::GetSock();
	SOCKADDR_IN addrClient = {0};
	int nLen = sizeof(SOCKADDR_IN);

	memset(&addrClient, 0, sizeof(SOCKADDR_IN));
	nLen = sizeof(SOCKADDR_IN);
	SOCKET sockClient = ::accept(sock, (sockaddr*)&addrClient, &nLen);
	if (INVALID_SOCKET == sockClient)
	{
		int iErr = WSAGetLastError();
        this->SetErrCode(iErr);
        //throw(string("accept failed!"));            //<<throw>>
        return CN_ERROR;
		
	}

	//获取来源的地址信息
	m_strLastClientIp	= GetClientIp(sockClient);
	m_uLastClientPort   = GetClientPort(sockClient);
	m_sockLastRecv      = sockClient;
    //过滤和筛选客户的连接
    
    if (!m_pSockClientManager->Filter(CLIENT_ADDRESS(m_strLastClientIp, m_uLastClientPort) ))
	{
        //调用过滤接口
        DealFilter(m_strLastClientIp, m_uLastClientPort);
		closesocket(sockClient);
		return CN_FILTER;
	}
	else
	{
        m_strClientIp   = m_strLastClientIp;
        m_uClientPort   = m_uLastClientPort;
	}

    if (m_iParam.bKeepalive)
    {
        tcp_keepalive tcpKA;
        tcpKA.keepaliveinterval = 1 * 1000; 
        tcpKA.keepalivetime     = m_iParam.nKeepalivetime;
        tcpKA.onoff             = 1;

        char cBuf[300] = {0};
        DWORD dwRet = 0;

        if(SOCKET_ERROR == WSAIoctl(sockClient, SIO_KEEPALIVE_VALS, &tcpKA, sizeof(tcp_keepalive), cBuf, sizeof(cBuf), 
            &dwRet, NULL, NULL))
        {
            SetErrCode(WSAGetLastError());
            return CN_ERR_CLIENT;
        }
    }
	return CN_OK;
	//接收到后放入队列
}

bool CSockTcp::DealAcceptError()
{
    DEBUG_INFO("accept fail!");
    return true;
}

bool CSockTcp::DealCloseByClient()
{
    return true;
}

bool CSockTcp::DealSelectError(int nErr)
{
    return true;
}

void CSockTcp::AddFilter(const CLIENT_ADDRESS* pCaR)
{
    m_pSockClientManager->AddFilter(*pCaR);
}

void CSockTcp::AddDisFilter(const CLIENT_ADDRESS* pCaR)
{
    m_pSockClientManager->AddDisFilter(*pCaR);
}

bool CSockTcp::DealFilter(const string&, u_short uPort)
{
    return true;
}

RECV_STATE CSockTcp::Recv(char* &pBuf)
{
	//分包接受大小

	//先接受数据大小,在接受具体的数据
	DWORD dwDataSize = this->RecvDataSize();
	if(dwDataSize <= 0)
	{
		return RECV_ER_UNKNOWN;
	}
	DWORD dwRecv = 0;
	pBuf = new char[dwDataSize];//new,外部释放,处理大小为0的情况
	memset(pBuf, 0, dwDataSize);
	dwRecv = this->RecvData(pBuf, dwDataSize);
	if (dwRecv != dwDataSize)
	{
		//后续需改进, 也有可能是缓冲区不够,无法接受太多数据
		return RECV_ER_UNKNOWN;
	}
	m_LastRecvSize = dwDataSize;
	return RECV_OK;
}

DWORD CSockTcp::RecvData(const char* pBuf, u_long lSize)
{
	SOCKET sock = CSock::GetSock();
	int iRet = 0;
	u_long dwRecv = 0;
	u_long dwCanRecv = 0;
	while(dwRecv < lSize)
	{
		dwCanRecv = min(lSize - dwRecv, RECV_SIZE);
		iRet = ::recv(sock , (char*)(pBuf + dwRecv), dwCanRecv, 0);
		if (INVALID_SOCKET == iRet)
		{
            //异常关闭

			int iErr = WSAGetLastError();
			this->SetErrCode(iErr);
            switch(iErr)
            {
            case WSAECONNABORTED: //程序已经关闭了该socket
                this->Close();
                return 0;
            default:
                break;
            }
            return 0;
			//return dwRecv;
		}
        else if (0 == iRet )
		{
            //socket对等端被正常关闭了
            this->DealCloseByClient();

            //服务端也关闭掉
            this->Close();

			if(dwRecv < lSize)
			{
				return 0;
			}
			else
			{
				return dwRecv;
			}

            
		}

		dwRecv += iRet;
	}

	return dwRecv;
}

DWORD CSockTcp::RecvDataOneTime(char* pBuf, u_long lMaxSize)
{
	assert(lMaxSize <= RECV_SIZE);

	SOCKET sock = CSock::GetSock();
	int iRet = 0;
	iRet = ::recv(sock , (char*)pBuf, lMaxSize, 0);
	if (INVALID_SOCKET == iRet)
	{
		int iErr = WSAGetLastError();
		wchar_t cErr[10] = {0};
		_itow(iErr, cErr, 10);

		//DEBUG_ERROR(L"receive_failed! Err = " + cErr);
		return 0;
	}
	if (0 == iRet )
	{
		//对方关闭了socket
		return 0;
	}
	return iRet;
}

SEND_STATE CSockTcp::Send(const char* data, u_long lSize)
{
	//分段发送数据过去
	//先发送数据大小,在发送数据
    SEND_STATE ss = this->SendDataSize(lSize);
	if(SEND_OK != ss)
	{
		return ss;
	}
	return this->SendData(data, lSize);
	
}

SEND_STATE CSockTcp::SendData(const char* data, u_long lSize)
{
    if(!m_bCnServer)
    {
        return SEND_ER_CONNECT;
    }

	//char cSendBuf[200] = "dfdf";
	int iRet = 0;
	u_long dwSended = 0;
	u_long dwCanSend = 0;

	while(dwSended < lSize)
	{
		dwCanSend = min(lSize - dwSended, SEND_SIZE);
		iRet = ::send(CSock::GetSock(), data + dwSended, dwCanSend , 0);
        if(SOCKET_ERROR == iRet)
        {
            iRet = WSAGetLastError();
            SetErrCode(iRet);

            SEND_STATE ss;
            switch(iRet)
            {
                /************************************************************************/
                /*  该包数据的错误, 不会影响到后面的所有send操作
                /************************************************************************/
            case WSAEMSGSIZE:
            case WSAEHOSTUNREACH:
                ss = SEND_ER_CLIENT;
                break;
            

                /************************************************************************/
                /*  连续性错误, 会影响到后面的所有send操作
                /************************************************************************/

            case WSAETIMEDOUT:      //重连时需要重新绑定端口
                m_bNeedRebind   = true;
            default:
                m_bCnServer     = false;
                ss = SEND_ER_CONNECT;
                break;
            }
            m_bNeedRebind       = true;
            return ss;
        }
        

        //没错误发生
		dwSended += iRet;
	}

	return SEND_OK;
}

SEND_STATE CSockTcp::SendDataSize(DWORD dwSize)
{
	char cBuf[DATA_SIZE_DIGIT_NUMBER] = {0};//将发送的数据大小,必须跟接收端的大小一致
	//发送大小
	ultoa(dwSize, cBuf, 10);
	return this->SendData(cBuf, sizeof(cBuf));
}

DWORD CSockTcp::RecvDataSize()
{
	char cBuf[DATA_SIZE_DIGIT_NUMBER] = {0};//将接受的数据大小,必须跟发送端的大小一致
	if(this->RecvData(cBuf, sizeof(cBuf)))
    {
        return atol(cBuf);
    }
	//发送大小
	return 0;
}

bool CSockTcp::SetRemoteInfo(const char* ip, u_short port)
{
	m_strServerIp = ip;
	m_uServerPort = port;
	return true;
}

void CSockTcp::SetDeal(IDataDeal* pDeal)
{
	m_pDataDeal = pDeal;
}

bool CSockTcp::InitAdditionArgu()
{
	m_pThreadArgu = new THREAD_ADDITION_ARGU;
	memset(m_pThreadArgu, 0, sizeof(THREAD_ADDITION_ARGU));
	
	return true;
}

bool CSockTcp::DoDealDataRecv(void* pBuf, DWORD dwRecv)
{
	//每次收到数据后的处理
	//OutputDebugString(L"CSockTcp::DoDealDataRecv!!");

    if (m_pDataDeal)
    {
        m_pDataDeal->SetData(pBuf);
        m_pDataDeal->Work(this);
    }
    else
    {
        
    }
	

	return true;
}

CSock* CSockTcp::Clone()
{
	CSockTcp* pTcp		= new CSockTcp(*this);
	return pTcp;
}


bool CSockTcp::Listen()
{
	return Listen(2);
}

bool CSockTcp::IsStopListen()
{
    m_pDataLock->Lock();
    bool bRet = m_bStopListen;
    m_pDataLock->UnLock();
    return bRet;
}

bool CSockTcp::IsStopClientRecv()
{
    bool bRet = m_bStopClientRecv;
    return bRet;
}

bool CSockTcp::Listen(DWORD backlog )
{
	SOCKET sock = CSock::GetSock();
	if(0 != listen(sock, backlog))
	{
		//DEBUG_ERROR(L"listen failed");
		return false;
	}
	return true;
}

string CSockTcp::GetClientIp(SOCKET sock)
{
	SOCKADDR_IN addrClient = {0};
	int iAddrSize = sizeof(SOCKADDR_IN);

	if(getpeername(
		sock, 
		(sockaddr*)&addrClient,
		&iAddrSize)
		)
	{
		int iErr = WSAGetLastError();
		////DEBUG_ERROR(_itow(iErr,))
		//AfxMessageBox(L"getsockname");
	}
	return inet_ntoa(addrClient.sin_addr);
}

string CSockTcp::GetClientIp()
{
    return m_strClientIp;
}

u_short CSockTcp::GetClientPort(SOCKET sock)
{
	SOCKADDR_IN addrClient = {0};
	int iAddrSize = sizeof(SOCKADDR_IN);

	if(getpeername(
		sock, 
		(sockaddr*)&addrClient,
		&iAddrSize)
		)
	{
		int iErr = WSAGetLastError();
		WCHAR cTemp[30] = {0};
		_itow(iErr, cTemp, 10);
		//DEBUG_ERROR(wstring(L"getpeername:" ) + cTemp);
		//AfxMessageBox(L"getsockname");
	}

	u_short uPort = ntohs(addrClient.sin_port);
	return uPort;
}

bool CSockTcp::ExitRecvMsgThread(void* cInfo /*= NULL*/)
{
    DEBUG_INFO("Exit Thread: ClientMsgProcThread");
    return true;
}

bool CSockTcp::ExitWaitForConnectThread(void* cInfo /*= NULL*/)
{
    DEBUG_INFO("Exit Thread: WaitForConnectProc");
    return true;
}

bool CSockTcp::DealAcceptClientError()
{
    DEBUG_INFO("DealAcceptClientError");
    return true;
}

bool CSockTcp::AcceptSleep()
{
    Sleep(20);
    return true;
}

bool CSockTcp::IsConnect()
{
    return m_bCnServer;
}

bool CSockTcp::CanRecn()
{
    if( !(!m_bCnServer && m_iParam.bAutoRecn) )
    {
        return false;
    }
    int iCode = GetErrCode();
    /*switch(iCode)
    {
    case WSAETIMEDOUT:
        
        return true;

    default:
        return false;
        break;
    }*/
    return true;
}

UNINT CSockTcp::ReConnectSome(list<CSockTcp*>& lstTcp)
{
    UNINT nRecnSuccessCount = 0;
    for (LIST_TCP::iterator it = lstTcp.begin(); it != lstTcp.end();)
    {
        
        if( !(*it)->CanRecn())
        {
            ++it;
            continue;
        }


        if((*it)->ReConnect())
        {
            //重连成功后, 移出列表
            ++nRecnSuccessCount;
            //it = lstTcp.erase(it);
        }
        else
        {
            (*it)->DealReConnectFail();
            ++it;
        }
    }
    
    return nRecnSuccessCount;
}

bool CSockTcp::SetNonBlockMode(bool bBlock)
{
    SOCKET s = GetSock();
    //设置连接超时
    u_long nParam = (u_long)bBlock;
    if(SOCKET_ERROR == ioctlsocket(s, FIONBIO, &nParam))
    {
        int i = WSAGetLastError();
        return false;
    }
    return true;
}

void CSockTcp::DealReConnectFail()
{
    
}

string CSockTcp::GetServerIp()
{
    return m_strServerIp;
}

u_short CSockTcp::GetServerPort()
{
    return m_uServerPort;
}

bool CSockTcp::DealSendFail()
{
    return true;
}


unsigned  __stdcall WaitForConnectProc(LPVOID lpPar)
{
    typedef CSockTcp::CONNECT_RESULT    CONNECT_RESULT;

	CSockTcp* pCn = (CSockTcp*)(lpPar);
	if(!pCn->Listen())//子类可自己决定监听数
	{
		return 0;
	}

    ostringstream oss;
	SOCKADDR_IN addrClient = {0};
	int nLen = sizeof(SOCKADDR_IN);

    bool bStop = false;
    int nSelect = 0;

    try
    {
	    while(!bStop)
	    {
            nSelect = SelectRead(pCn->GetSock());
            bStop = pCn->IsStopListen();

            if (nSelect > 0)
            {
		        SOCKET sockClient = NULL;
                CONNECT_RESULT cr;
			    cr = pCn->Accept();

                switch(cr)
                {
                
                case CSockTcp::CN_ERROR:
                    //接受失败,套接字有问题,停止接收
                    pCn->DealAcceptError();
                    throw(string("accept fail!"));

                case CSockTcp::CN_ERR_CLIENT:
                    pCn->DealAcceptClientError();
                    continue;
                case CSockTcp::CN_FILTER:
    
                    //在这之前已经调用处理接口了
                    continue;

                case CSockTcp::CN_OK:
                    //正常的连接
                    break;

                default:
                    continue;
                }

		        CSockTcp* pSock = dynamic_cast<CSockTcp*>(pCn->Clone()); //继续把附加指针传递下去
		        if(!pSock->InitAdditionArgu())
                {
                    //跳过该数据
                    delete pSock;
                    continue;
                }

		        HANDLE hThreadClient = (HANDLE)_beginthreadex(
                    NULL, 
			        0, 
			        WaitForClientMsgProc, 
			        pSock, 
			        0,
			        NULL
			        );
		        if (!hThreadClient)
		        {
			        //DEBUG_ERROR(L"create client thread failed");
			        break;
		        }
                CSockClientManager::CLIENT_DATA ci = {0};
                ci.hThread          = hThreadClient;
                ci.pClientSockTcp   = pSock;

                pSock->m_pSockClientManager->Add(ci);
            }
            else if(nSelect < 0)
            {
                pCn->DealSelectError(GetSelectError());
                //select 发生错误
                break;
                //OutputDebugString(L"无新连接到来");
            }

            pCn->AcceptSleep(); //以后改为可由子类来定制
    	
        }
    }
    catch(const string& strErr)
    {
        //接收错误, 在抛出异常之前已做了处理
    }
	pCn->ExitWaitForConnectThread();
    _endthreadex(1);
	return 1;
}

unsigned  __stdcall WaitForClientMsgProc(LPVOID lpPar)
{
	//TwoPoint* pTwoPoint = (TwoPoint*)lpPar;
	//Sleep(5000);

	CSockTcp* pSock = (CSockTcp*)lpPar;//<delete> 线程结束需释放
	/*char cBuf[100] = {0};
	memset(cBuf, 0, sizeof(cBuf));*/

    int nSelect = 0;
	while(1)
	{
        nSelect = SelectRead(pSock->GetSock(), 1);
        if ( nSelect == 0)
        {
            continue;
        }
        else if ( nSelect < 0)
        {
            //select 出错, 停止接收
            break;
        }

		char* pBuf = NULL;//处理的地方负责释放
		DWORD dwRecv = 0;
		dwRecv = pSock->Recv((char*)pBuf);

        switch(dwRecv)
        {
        case CSockTcp::RECV_ER_UNKNOWN:
        case CSockTcp::RECV_ER_NET:    //接收出错, 结束接收
        case CSockTcp::RECV_ER_DATA:    //接收出错, 结束接收
                goto lbEndRecv;

        case CSockTcp::RECV_ER_NOCARE:     //不处理接收到的数据
                continue;
                break;

            default:
                break;
        }

		//让子类处理收到的数据
		pSock->DoDealDataRecv(pBuf, dwRecv);

		/*delete [] pBuf;
		pBuf = NULL;*/
	}

lbEndRecv:
    pSock->m_pSockClientManager->Delete(pSock);

	pSock->ExitRecvMsgThread();
    delete pSock;
    _endthreadex(1);
	return 1;
}


