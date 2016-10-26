#pragma once
#include <WinSock2.h>
#include "PiTypeDef.h"
typedef struct icmp_hdr  
{  
    unsigned char   icmp_type;      // 消息类型   
    unsigned char   icmp_code;      // 代码   
    unsigned short  icmp_checksum;  // 校验和   

    // 下面是回显头   
    unsigned short  icmp_id;        // 用来惟一标识此请求的ID号，通常设置为进程ID   
    unsigned short  icmp_sequence;  // 序列号   
    unsigned long   icmp_timestamp; // 时间戳   
} ICMP_HDR, *PICMP_HDR; 


USHORT checksum(USHORT* buffer,int size);

class CIcmp
{
public:
    CIcmp(void);
    ~CIcmp(void);
public:
    bool    IsDeviceNormal(const char* m_DeviceIp);
    void    SetRecvTimeout(UNINT nTimeout);
private:
    UNINT   m_nRecvTimeout;
};
