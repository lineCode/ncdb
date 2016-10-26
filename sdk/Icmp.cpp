#include "StdAfx.h"
#include "Icmp.h"
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

CIcmp::CIcmp(void)
{
    m_nRecvTimeout = 250;
}

CIcmp::~CIcmp(void)
{
}

void CIcmp::SetRecvTimeout(UNINT nTimeout)
{
    m_nRecvTimeout = nTimeout;
}
//检查设备状态是否正常
bool CIcmp::IsDeviceNormal(const char* m_DeviceIp)
{
    int         ReceivePacketNum=0;
    SOCKADDR_IN dest;  
    SOCKET      m_Socket;

    dest.sin_family = AF_INET;  
    dest.sin_addr.S_un.S_addr = inet_addr(m_DeviceIp);  


    m_Socket=::WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED);

    if(m_Socket==INVALID_SOCKET)
    {
        return FALSE;
    }
    else
    {
        int   Raw_Timeout = 250;
        ::setsockopt(m_Socket,SOL_SOCKET,SO_SNDTIMEO,(char*)&Raw_Timeout,sizeof(int));
        ::setsockopt(m_Socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&m_nRecvTimeout,sizeof(int));
    }

    // 创建ICMP封包   
    char buff[sizeof(ICMP_HDR) + 32]={0};  
    ICMP_HDR* pIcmp = (ICMP_HDR*)buff;  

    // 填充数据部分，可以为任意   
    memset(&buff[sizeof(ICMP_HDR)], 'E', 32);  

    // 开始发送和接收ICMP封包   
    USHORT      nSeq = 1;  
    char        recvBuf[1024]={0};  
    SOCKADDR_IN from;  
    int nLen = sizeof(from); 
    int nRet;

    for(int i=0;i<4;i++)
    {
        // 填写ICMP封包数据，请求一个ICMP回显 
        pIcmp->icmp_type = 8;      
        pIcmp->icmp_code = 0;  
        pIcmp->icmp_id = (USHORT)::GetCurrentProcessId();
        pIcmp->icmp_checksum = 0;   
        pIcmp->icmp_timestamp = (ULONG)(::GetTickCount());  
        pIcmp->icmp_sequence = (USHORT)nSeq++; 
        pIcmp->icmp_checksum = checksum((USHORT*)buff, sizeof(ICMP_HDR) + 32); 

        nRet = ::sendto(m_Socket, buff, sizeof(ICMP_HDR) + 32, 0, (SOCKADDR *)&dest, sizeof(dest));  

        if(nRet == SOCKET_ERROR || nRet<=0)  
        {  
            ReceivePacketNum--;
            continue;
        }  
        nRet = ::recvfrom(m_Socket, recvBuf, 1023, 0, (sockaddr*)&from, &nLen);

        if(nRet == SOCKET_ERROR || nRet<=0)  
        {
            int iErr = WSAGetLastError();
            ReceivePacketNum--;
            continue;
        } 
        if(nRet >= 64)
        {
            ICMP_HDR* pRecvIcmp=(ICMP_HDR*)(recvBuf+20);

            //if(pRecvIcmp->icmp_type == 3 ||pRecvIcmp->icmp_type == 4 || pRecvIcmp->icmp_type == 9 || pRecvIcmp->icmp_type == 10 || pRecvIcmp->icmp_type == 11)   //如果目标主机不可达，源抑制或是超时报文 icmp_code=1时表示主机不可达
            //{
            //	ReceivePacketNum--;
            //}
            if(pRecvIcmp->icmp_type != (USHORT)0)
            {
                ReceivePacketNum--;
                continue;
            }
            ReceivePacketNum++;	    
        }
    }
    if(ReceivePacketNum>=0)
    {
        closesocket(m_Socket);
        return TRUE;
    }
    closesocket(m_Socket);

    return FALSE;
}

//计算校验
USHORT checksum(USHORT* buffer,int size)
{
    unsigned long cksum=0;
    while(size>1)
    {
        cksum+=*buffer++;
        size-=sizeof(USHORT);
    }
    if(size)
    {
        cksum+=*(UCHAR*)buffer;
    }
    cksum=(cksum>>16)+(cksum&0xffff);
    cksum+=(cksum>>16);
    return (USHORT)(~cksum);
}