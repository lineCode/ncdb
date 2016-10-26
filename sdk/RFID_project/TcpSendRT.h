#pragma once
#include "SockTcp.h"
#include <sstream>
using namespace std;
class CRFID_Data_ServerDlg;

class CTcpSendRT :
    public CSockTcp
{
public:
    CTcpSendRT(void* pDlg, const SOCK_PARAM&);
    CTcpSendRT(const CTcpSendRT& tcp);
    ~CTcpSendRT(void);
public:
    virtual void    DealReConnectFail();
    virtual CSock*  Clone();
    virtual bool    DealSendFail();    
private:
    ostringstream   m_oss;
    CRFID_Data_ServerDlg*       m_pDlg;
};
