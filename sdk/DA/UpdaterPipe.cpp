#include "StdAfx.h"
#include "UpdaterPipe.h"
#include <functional.h>
#include <AutoType.h>


CUpdaterPipe::CUpdaterPipe(void)
{
    m_pObj = NULL;
    m_fun = NULL;
}


CUpdaterPipe::~CUpdaterPipe(void)
{
}


bool CUpdaterPipe::Write( const tagSTATE_NOTIFY& us )
{
    return CPipeManage::Write((void*)&us, sizeof(us));    
}

bool CUpdaterPipe::SetHwnd( HWND hWnd )
{
    m_hwnd = hWnd;
    return true;
}

bool CUpdaterPipe::DealReadData()
{
    if (m_pObj && m_fun)
    {
        m_fun(m_pObj, m_pBuf);
    }
    return true;
}


bool CUpdaterPipe::DealUpdateState()
{
    tagUPDATE_PERCENT* pUS = (tagUPDATE_PERCENT*)m_pBuf;
    CAutoType at(pUS->nPercent);
    OutInfo(_T("CUpdaterPipe::DealUpdateState():"), (__int64)at);
    return true;
}

bool CUpdaterPipe::DealStateNotify()
{
    tagSTATE_NOTIFY* pSN = (tagSTATE_NOTIFY* )m_pBuf;
    CAutoType at(pSN->nState);
    OutInfo(_T("CUpdaterPipe::DealUpdateState():"), (__int64)at);
    return true;
}

bool CUpdaterPipe::DealReadFail()
{
    bool bRet = false;
    if (ENUM_PIPE_TYPE_SERVER == m_enumPipeType)
    {
        OutInfo(_T("服务端管道读取失败, 重连"));
        DisconnectNamedPipe(m_hPipe);
        m_enumThreadState = ENUM_THREAD_STATE_WAIT;
        bRet = true;
    }
    else
    {
        OutInfo(_T("客户端管道读取失败"));
        m_enumThreadState = ENUM_THREAD_STATE_STOP;

        ClosePipe();
        /*Sleep(300);
        bRet = ConnectPipe(m_strPipeName.c_str());*/
        
    }
    return bRet;
}

bool CUpdaterPipe::DealRequestExit()
{
    OutInfo(_T("get msg: RequestExit"));
    return true;
}

bool CUpdaterPipe::WriteUpdateState( tagUPDATE_PERCENT tag )
{
    return Write(&tag, sizeof(tag));
}

bool CUpdaterPipe::WriteError( ENUM_UPDATER_ERROR_TYPE errType )
{
    tagUPDATE_ERROR tag;
    tag.nErrType = errType;
    return Write(&tag, sizeof(tag));
}

void CUpdaterPipe::SetCB(void* pObj, CB_READ_DATA pFunc)
{
    m_pObj = pObj;
    m_fun = pFunc;
}

ENUM_PIPE_DATA_TYPE CUpdaterPipe::GetDataType( void* pAdd )
{
    ENUM_PIPE_DATA_TYPE dt = *(ENUM_PIPE_DATA_TYPE*)pAdd;
    return dt;
}
