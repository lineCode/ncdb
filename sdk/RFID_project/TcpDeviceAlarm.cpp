#include "StdAfx.h"
#include "TcpDeviceAlarm.h"
#include "GlobalFile.h"
#include "StlSupply.h"
#include "functional.h"
#include "resource.h"
#include "RFID_Data_ServerDlg.h"
//const 

bool PredFindIp(const ReaderInfo& ri, const string& strIp);

CTcpDeviceAlarm::CTcpDeviceAlarm(void* pDlg)
{
    m_pLockSendState        = NULL;
    m_pDlg                  = static_cast<CRFID_Data_ServerDlg*>(pDlg);
    m_nErrCode              = NULL;
}

CTcpDeviceAlarm::~CTcpDeviceAlarm(void)
{
}

bool CTcpDeviceAlarm::Init()
{
    if(!CSockTcp::Init())
    {
        return false;
    }
    m_pLockSendState        = new CLock;
   
    return true;
}

bool CTcpDeviceAlarm::Connect(const char* ip/* = ""*/, u_short port/* = 0*/)
{
    if(!CSockTcp::Connect(ip, port))
    {
        m_nErrCode = this->GetErrCode();
        m_oss.str("");
        m_oss<<INFO_ERROR "connect to alarm server fail, errCode = " << m_nErrCode;
        m_pDlg->PutLog(m_oss.str());
        return false;
    }
    return true;
}

void CTcpDeviceAlarm::CombineXml(const DeviceAlarm& da)
{
    string strType;
    string strDesc;
    if (da.bOk)
    {
        strType = "DEVICEOK";
        strDesc = "设备连接正常";
    }
    else
    {
        strType = "DEVICEERR";
        strDesc = "设备连接异常";
    }
    

    VT_READERINFO::iterator itRI;
    itRI = find_if(g_vtReaderInfo.begin(), g_vtReaderInfo.end(), PredFindIp, da.strIp);
    if (g_vtReaderInfo.end() == itRI)
    {
        if (da.bOk)
        {
            assert(0 && "no find in ReaderInfo with ip when device OK");
        }
        else
        {
            assert(0 && "no find in ReaderInfo with ip when device ERR");
        }
        
    }

    m_oss.str("");
    m_oss<<"<?xml version =\"1.0\" encoding = \"GB2312\"?>\r\n\
           <message System = \"ATMS\" Ver = \"1.0\">\r\n\
           <systemtype>RFID</systemtype>\r\n\
           <messagetype>ALARM</messagetype>\r\n\
           <sourceIP>127.0.0.1</sourceIP>\r\n\
           <targetIP></targetIP>\r\n\
           <user></user>\r\n\
           <password></password>\r\n\
           <alarm>\r\n";

    m_oss<<"<deviceid>" << itRI->strDDBH      << "</deviceid>\r\n\
           <alarmtype>" << strType          << "</alarmtype>\r\n\
           <time>"      << da.strOccureTime << "</time>\r\n\
           <desc>"      << strDesc          << "</desc>\r\n\
           </alarm>\r\n\
           </message>\r\n";
    m_strXml = m_oss.str();
}

bool CTcpDeviceAlarm::TalkDeviceState(const DeviceAlarm& da)
{
    m_pLockSendState->Lock();

    /************************************************************************/
    /* 如果上次发送失败, 则进行重连
    /************************************************************************/
    if (m_nErrCode > 0 )
    {
        //switch(m_nErrCode)
        //{
        ///*case WSAECONNRESET:
        //case WSAETIMEDOUT:
        //case WSAECONNABORTED:
        //case WSAENETDOWN:*/
        //    //重新连接

        //    //连接失败, 直接返回
        //    break;
        //}
        
        if(!this->ReConnect())
        {
            m_pLockSendState->UnLock();

            m_oss.str("");
#ifdef Release_Debug
            m_oss<<INFO_ERROR;
#else
            m_oss<<INFO_WARNING;
#endif
            m_oss<< "重连报警服务器失败,code = " << this->GetErrCode();
            m_pDlg->PutLog(m_oss.str());
            return false;
        }
        m_nErrCode = 0;
    }


    CombineXml(da); 
    if(SEND_OK != this->SendData(m_strXml.c_str(), m_strXml.length()))
    {
        //根据错误代码,看下下次发送是否需要重连
        m_oss.str("");
        m_nErrCode = this->GetErrCode();
        m_oss<<INFO_ERROR "send device alarm failed!, code"<<m_nErrCode;
        
        m_pLockSendState->UnLock();
        return false;
    }

    //发送成功, 写入log
    m_oss.str("");
    m_oss<<INFO_NORMAL <<"xml = "<< m_strXml;
    m_pDlg->PutLog(m_oss.str());
    m_pLockSendState->UnLock();

    /*this->Close();
    this->SetRemoteInfo("127.0.0.1", 3434);*/
    return true;
}

bool CTcpDeviceAlarm::TalkDeviceNormal(const string& strIp)
{   
    DeviceAlarm da;
    da.bOk              = true;
    da.strIp            = strIp;
    da.strOccureTime    = GetLocalDateTime();
    return TalkDeviceState(da);
}

bool CTcpDeviceAlarm::TalkDeviceAbnormal(const string& strIp)
{
    DeviceAlarm da;
    da.bOk              = false;
    da.strIp            = strIp;
    da.strOccureTime    = GetLocalDateTime();
    return TalkDeviceState(da);
}

bool PredFindIp(const ReaderInfo& ri, const string& strIp)
{
    return ri.strIP == strIp;
}