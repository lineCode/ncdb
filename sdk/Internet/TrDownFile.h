#pragma once
#include "threadlight.h"
#include "InetDef.h"
#include "LockData.h"
class CInternetHttp;
class CLock;
class CDowningInfoMng;
class CTrDownFile:
    public CThreadLight
{
public:
    typedef ns_down_file::CBDowningInfo CBDowningInfo;

    CTrDownFile(CallBackFun = NULL, void* pAdd= NULL );
    virtual ~CTrDownFile(void);

public:
    bool Init();
    bool UnInit();
    bool StopDown();
    bool ExitDown();
    bool AddDownTask(DownFileInfo dfi);
    bool AddDownTask(DownList dl);
    virtual DWORD DoWork();
    /************************************************************************
        fun:	下载指定URL的文件, 阻塞方式
        param:  
        ret:    
        remark: 
	/************************************************************************/
	bool DownFile( DownFileInfo& dInfo );
    void SetDowningInfoCB( CBDowningInfo pFun, void* pAdd );
    bool CanRetry( DownState ds);
    bool GetNextTask();
    void SetProxy( tcpchar szProxyName);
    void GetErrInfo( DownFileInfo& dfi );
    bool ResumeDown();
    bool SetHeader( LPCTSTR param1 );
    bool SetFileSizeLimit(UNLONG nSize);
    DownFileInfo* GetDownInfo(){ return m_pDownInfo; }
    bool IsUsedProxy();
    bool SetHasUseProxy();
    bool IsProxyErr( DownState ds );
    bool SaveErrInfo( DownFileInfo& dInfo, CInternetHttp* m_pInternet );
    bool DealDownFail( DownFileInfo it );
    bool RetryOut();
    bool IsReTryOK();
    bool ClearState();
    virtual bool InitInetObj();
    void DowningInfoMng(CDowningInfoMng* val) { m_pDowningInfoMng = val; }
    bool DealAllOk();
    bool GetAllFileSize(UNLONG& dwSize);
    bool SetShowTotalSize( UNLONG nSize );
    DownList::iterator GetFirstTask();
protected:
    auto_ptr<CInternetHttp>			m_pInternet;

private:
    DownList::iterator*     m_it; //保证迭代器有删除, 否则会引起访问异常

    DownList		m_lstToDown;		//下载列表
    UNLONG			m_dwSingleSizeLimit;	//单一文件下载大小限制
    UNINT           m_ServerCode;           //服务器状态码
    bool            m_bQuitDown;        //是否停止下载
    CBDowningInfo m_pCBDowningInfoFun;      //下载中显示进度,速度的回调函数
    auto_ptr<CLock>           m_lockDownList;

    //默认参数
    void*           m_pDownInfoCBAdd;   //下载中回调函数的用户参数
    DownFileInfo*           m_pDownInfo;
    
    CDowningInfoMng*       m_pDowningInfoMng;
    
    auto_ptr<CEventLight>           m_pEvPause; //暂停下载事件
    int             m_nCurRetryTimes;   //当前下载项重试次数
    tstring                 m_strProxyName;
    auto_ptr<CLock>           m_pLockProxy;
    bool        m_bUsedProxy;   //是否使用过了上次设置的代理
};

