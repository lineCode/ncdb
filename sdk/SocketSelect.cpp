#include "StdAfx.h"
#include "SocketSelect.h"
#include "AutoType.h"

//只在该文件中可访问
int g_nSelectErrCode          =           0;           //select 错误代码

SocketSelect::SocketSelect(void)
{
    
}

SocketSelect::~SocketSelect(void)
{
}

int SelectRead(SOCKET s, UNLONG millisecond)
{
    fd_set fds = {0};
    timeval tv = {0, millisecond * 1000};
    FD_ZERO(&fds);
    FD_SET(s, &fds);

    int iRet = select(0, &fds, NULL, NULL, &tv);
    if ( SOCKET_ERROR == iRet )
    {
        g_nSelectErrCode = WSAGetLastError();
        //CAutoType at(iErr);

    }
    return iRet;
}

int SelectWrite(SOCKET s, UNLONG millisecond)
{
    fd_set fds = {0};
    timeval tv = {0, millisecond * 1000};
    FD_ZERO(&fds);
    FD_SET(s, &fds);

    int iRet = select(0, NULL, &fds, NULL, &tv);
    if(iRet > 0)
    {
        return true;
    }
    else
    {

        int iErr = WSAGetLastError();
        CAutoType at(iErr);

        return false;
    }
}

int GetSelectError()
{
    return g_nSelectErrCode;
}