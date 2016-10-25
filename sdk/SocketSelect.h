#pragma once
#include <WinSock2.h>
#include "PiTypeDef.h"
class SocketSelect
{
public:
    SocketSelect(void);
    ~SocketSelect(void);
};

/************************************************************************/
/*  fun: 判断指定的socket 在指定ms内是否可读, 
    remark: 单位ms
/************************************************************************/
int SelectRead(SOCKET s, UNLONG timeOutMillisecond = 5);
int SelectWrite(SOCKET s, UNLONG timeOutMillisecond = 5);
int GetSelectError();