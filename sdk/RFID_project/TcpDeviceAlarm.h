#pragma once
#include <string>
#include <sstream>
#include "Lock.h"
#include "..\sdk\socktcp.h"

class CRFID_Data_ServerDlg;
struct DeviceAlarm
{
    string      strIp;          //设备ip
    bool        bOk;            //设备是否正常
    string      strOccureTime;  //设备状态发生事件
};
class CTcpDeviceAlarm :
    public CSockTcp
{
public:
    CTcpDeviceAlarm(void* pDlg);
    ~CTcpDeviceAlarm(void);

public:
    bool    Init();
    virtual bool	Connect(const char* ip = "", u_short port = 0);
    bool            TalkDeviceState(const DeviceAlarm& da);
    bool            TalkDeviceNormal(const string&);
    bool            TalkDeviceAbnormal(const string&);
    void            CombineXml(const DeviceAlarm&);
    
private:
    ostringstream       m_oss;
    string              m_strXml;
    CLock*              m_pLockSendState;
    CRFID_Data_ServerDlg*   m_pDlg;
    int                 m_nErrCode;
};
