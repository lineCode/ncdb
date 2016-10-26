#include "stdafx.h"
#include "SockUdp.h"
#include <assert.h>
#include <stdio.h>
const long RECV_SIZE = 1024 * 5 * 10;
const long SEND_SIZE = 1024 * 5 * 10;

const long UDP_SEND_BUF_SIZE = 1024 * 100;		//发送缓冲区大小
const long UDP_RECV_BUF_SIZE = 1024 * 1024;		//接受缓冲区大小

typedef CSock::RECV_STATE       RECV_STATE;
typedef CSock::SEND_STATE               SEND_STATE;

CSockUdp::CSockUdp( SOCKET sock /*= 0*/)
:CSock(sock)
{
	m_pLastRecvAddr	= NULL;
    m_nSendSizePack = SEND_SIZE;
}

CSockUdp::~CSockUdp(void)
{

}


bool CSockUdp::Init(u_short port /*= 0*/)
{
	m_pLastRecvAddr = new SOCKADDR_IN;
	memset(m_pLastRecvAddr, 0, sizeof(SOCKADDR_IN));

    SOCK_INFO si = {0};
    si.sockType = SOCK_DGRAM;
    SetSockInfo(si);
    if(!CSock::Init())
    {
        SetErrDesc("InitSockLib");
        return false;
    }

	if(!this->Bind(port))
    {
        SetErrDesc("Bind Port");
        return false;
    }
	//this->SetRecvBufSize();
	return true;
}


RECV_STATE CSockUdp::Recv(char* &pBuf)
{
    UNLONG lDataSize = this->RecvDataSize();

	if(lDataSize <= 0)
	{
		return RECV_ER_UNKNOWN;
	}
	DWORD dwRecv = 0;
	pBuf = new char[lDataSize];//new,外部释放,处理大小为0的情况
	memset(pBuf, 0, lDataSize);

	UNLONG lSizeRecved = this->RecvData(pBuf, lDataSize);
	if (lSizeRecved != lDataSize)
	{
		//实际接受大小跟期望接受的大小不一样
		return RECV_ER_UNKNOWN;
	}

    assert(0 && "未支持, 不知道接受字节数");
    return RECV_OK;

}

DWORD CSockUdp::RecvData(char* pBuf, u_long lSize)
{
	//分包接受大小

	int iRet = 0;
	u_long dwRecv = 0;
	u_long dwCanRecv = 0;
	char bufOpt[30] = {0};
	int iBufOptSize = sizeof(bufOpt);
	SOCKET sock = this->GetSock();
	getsockopt(sock , SOL_SOCKET, SO_RCVBUF, bufOpt, &iBufOptSize);

	int iRecvBuf = *(int*)bufOpt;
	while(dwRecv < lSize)
	{
		dwCanRecv = min(lSize - dwRecv, RECV_SIZE);
		
		//保存刚才发送消息来的客户
		
		int nLen = sizeof(SOCKADDR_IN);

		iRet = ::recvfrom(sock, (char*)(pBuf + dwRecv), dwCanRecv, 0, (sockaddr*)m_pLastRecvAddr, &nLen);
		if (INVALID_SOCKET == iRet)
		{
			int iErr = WSAGetLastError();
			SetErrCode(iErr);
			/*wchar_t cErr[10] = {0};
			_itow(iErr, cErr, 10);*/
			////DEBUG_ERROR(L"receive_failed! Err = " + cErr);
			return dwRecv;
		}
		dwRecv += iRet;
		/*wchar_t buf[20] = {0};
		wsprintf(buf, L"%d\n",dwRecv);
		OutputDebugStringW(buf);*/
	}

	return dwRecv;
}

DWORD CSockUdp::RecvDataSize()
{
	char cBuf[DATA_SIZE_DIGIT_NUMBER] = {0};
	//发送大小
	int iSize = sizeof(SOCKADDR_IN);//最大能保存长度为 int(4 byte)的最大值 :4G
	if(SOCKET_ERROR == ::recvfrom(this->GetSock(), cBuf, sizeof(cBuf), 0, (sockaddr*)m_pLastRecvAddr, &iSize))
	{
		return -1;
	}
	return atol(cBuf);
}

void CSockUdp::SetRemoteAddress( const char* ip,u_short port)
{
	SOCKADDR_IN addr = {0};

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip);

	*m_pLastRecvAddr = addr;
}

SEND_STATE CSockUdp::Send(const char* data, u_long lSize)
{
	//先发送数据大小, 以字符串的形式
    SEND_STATE ss = this->SendDataSize(lSize);
	if(SEND_OK != ss)
	{
		return ss;
	}

	//发送数据
	return this->SendData(data, lSize);
}

SEND_STATE CSockUdp::SendData(const char* data, u_long lSize)
{
	//一次的消息不能超过预定值 ，SO_MAX_MSG_SIZE

	//分段发送数据过去

	//char cSendBuf[200] = "dfdf";
	int iRet = 0;
	u_long dwSended = 0;
	u_long dwCanSend = 0;
	SOCKET sock = this->GetSock();
	while(dwSended < lSize)
	{
		dwCanSend = min(lSize - dwSended, m_nSendSizePack);
		iRet = ::sendto(sock, data + dwSended, dwCanSend , 0, (sockaddr*)m_pLastRecvAddr, sizeof(SOCKADDR_IN));
		if(SOCKET_ERROR == iRet)
		{
			int iErr = WSAGetLastError();
			SetErrCode(iErr);
			////DEBUG_ERROR(L"Send failed!");
			return SEND_ER_CONNECT;         //直接返回高级错误
		}
		dwSended += iRet;
	}
	return SEND_OK;
}

SEND_STATE CSockUdp::SendDataSize(DWORD dwSize)
{
	char cBuf[DATA_SIZE_DIGIT_NUMBER] = {0};
	//发送大小
	ultoa(dwSize, cBuf, 10);
	return this->SendData(cBuf, strlen(cBuf) + 1);
}

bool CSockUdp::DoDealDataRecv(void* pBuf, DWORD dwRecv)
{
    return true;
}

CSock* CSockUdp::Clone()
{
	assert(0 && "暂不支持克隆");
    CSockUdp* pUdp = new CSockUdp(*this);
    return pUdp;
}

bool CSockUdp::SetRecvBufSize()
{
	return SetSocketBuf(SO_RCVBUF, UDP_RECV_BUF_SIZE);
}

bool CSockUdp::SetSendBufSize(UNINT nSendSizePack)
{
    assert("还未完成");
	SetSocketBuf(SO_SNDBUF, nSendSizePack);
    m_nSendSizePack = nSendSizePack;
    return true;
    //需要获取下系统值， 以确定是设置成功了
}

bool CSockUdp::SetSocketBuf(int optname, int value)
{
	
	//char* pRecvBuf = (char*)&iRecvBuf;
	char RecvBuf[10] = {0};
	*(int*)RecvBuf = value;
	//发送缓冲区的设置暂时有问题，检查是否确实设置了值
	if(SOCKET_ERROR == setsockopt(this->GetSock(), SOL_SOCKET, optname, RecvBuf, 4))
	{	
		int iErr = WSAGetLastError();
		return false;
	}
	/*int iBufSize = sizeof(RecvBuf);
	memset(RecvBuf, 0, iBufSize);
	getsockopt(m_sock, SOL_SOCKET, optname, RecvBuf, &iBufSize);*/
	return true;
}