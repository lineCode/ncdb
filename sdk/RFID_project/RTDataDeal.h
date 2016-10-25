#pragma once
#include <string>
#include "RFID.h"
#include "GlobalFile.h"
#include "IDataDeal.h"
#include <sstream>

class CADODataBase;
struct TrafficInfo;
struct TrackShow;
class CLogOpr ;
class CShowData;
class CNativeDataDeal;
class CRFID_Data_ServerDlg;

using namespace std;
/************************************************************************/
/* fun: 数据处理类,负责把收到的数据放入到数据库中,和SockTcp类结合使用                                                                     */
/************************************************************************/

/************************************************************************
        fun:    原始数据处理类, 对数据做初步验证, 放到解析线程
        param:  
        ret:    
        remark: 
/************************************************************************/
class CRTDataDeal
	:public IDataDeal
{
public:
	CRTDataDeal(void* pAdd = 0);
	CRTDataDeal(CRTDataDeal& dd);
	~CRTDataDeal(void);

public:
	virtual bool Init(void*);
	virtual void SetData(void* pData);
	virtual IDataDeal* Clone();
	virtual bool Work(void*);

	bool WriteDataToLog();
	bool AnalyData();
	/*	fun: 放到ip对应的读卡器队列 */
	bool PushToList();
	bool SetDevicePos(const char* DeviceIp);

private:
	RECV_DATA_TCP_SET*	        m_pNativeData;
	CLogOpr*			        m_pLog;
	DWORD				        m_dwDevicePos;  //保存对应的全局device队列的Index

    TrackShow*      	        m_pTrackShow;   //<delete>显示到界面的结构体
	TagInfo*			        m_pTagInfo;     //<delete>
	CNativeDataDeal*	        m_NativeDataDeal;
    ostringstream               m_oss;
    CRFID_Data_ServerDlg*       m_pDlg;
};
