#include "StdAfx.h"
#include "TcpSendRT.h"
#include "resource.h"
#include "RFID_Data_ServerDlg.h"
CTcpSendRT::CTcpSendRT(void* pDlg, const SOCK_PARAM& sParam)
:CSockTcp(NULL, NULL, &sParam)
{
    m_pDlg  = static_cast<CRFID_Data_ServerDlg*>(pDlg);
}

CTcpSendRT::CTcpSendRT( const CTcpSendRT& tcp )
:CSockTcp(tcp)
{
    m_pDlg  = tcp.m_pDlg;
}

CTcpSendRT::~CTcpSendRT(void)
{
}

void CTcpSendRT::DealReConnectFail()
{
    m_oss.str("");
#ifdef Release_Debug
    m_oss<<INFO_ERROR;
#else
    m_oss<<INFO_WARNING;
#endif
    m_oss<<"ReConnect server fail:"<< GetServerIp() <<":"<< GetServerPort()
        <<" code:"<< this->GetErrCode();
    m_pDlg->PutLog(m_oss.str());
}

CSock* CTcpSendRT::Clone()
{
    return new CTcpSendRT(*this);
}

bool CTcpSendRT::DealSendFail()
{
    m_oss.str("");
    m_oss<<"sendData fail,ip"<<this->GetServerIp()<< " code:"<<this->GetErrCode();
    m_pDlg->PutLog(m_oss.str());
    return true;
}

