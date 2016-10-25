#pragma once
#include "threadlight.h"
#include "InetDef.h"
#include <memory>

#include "PiTypeDef.h"
#include "PiThreadDef.h"

#include "DowningInfoMng.h"
#include "LockData.h"
using namespace std;

class CLock;
class CProxyPC;
class CDownFileManager :
	public CThreadLight
{
public:
	CDownFileManager(CallBackFun = NULL, void* pAdd = NULL);
	virtual ~CDownFileManager(void);
public:
	virtual bool Init();
protected:
	/************************************************************************
		new了新的下载对象CTrDownFile后调用
	************************************************************************/
	virtual bool DoNewDown( CTrDownFile* pDown );
	virtual bool InitData();
	virtual bool DoRunBegin();
	virtual DWORD DoWork();
	virtual CTrDownFile* CreateDownFileObj();
	virtual bool DealDownFail( CTrDownFile* pTrDown );

public:
	bool UnInit();

	bool AddDownItemTask(DownFileInfo& dInfo);
    bool AddDownItemTask( tcpchar szUrl);
    bool AddDownItemTask( tcpchar szUrl, tcpchar szSavePath);
    bool AddDownItemTask(DownList& lstDown);
    
    void SetDowningInfoCB(CBDowningInfo pFun, void* pAdd);
    /************************************************************************
        fun:	启用下载线程 开始下载
        param:  
        ret:    
        remark: 
	/************************************************************************/
    bool StartDown();
	/************************************************************************
        fun:	限制单一文件可下载大小的最大值
        param:  
        ret:    
        remark: 
	/************************************************************************/
	void SetSingleFileMaxLimit(UNLONG dwSize);
	
    /************************************************************************
        fun:	退出下载
        param:  
        ret:    
        remark: 
	/************************************************************************/
    void QuitDownFile();
    bool StopAllDown();
    bool ExitAllDown();
    void SetDefaultSavePath(tcpchar szPath);
    /************************************************************************
        fun:	清除下载列表
        param:  
        ret:    
        remark: 
	/************************************************************************/
	void ClearAllDownItem();
    /************************************************************************
        fun:	列表任务下载完后, 获取下一个任务
        param:  
        ret:    
        remark: 
    /************************************************************************/
    static bool CB_GetNextDownTask(CDownFileManager* pMng, CTrDownFile* pDown);
    bool GetNextTask(DownList::iterator& itL);
    UNINT GetDownThreadCount() const;
    bool StopRunNewTask();
    bool StartRunNewTask();
    bool SetShowDownInfo(bool bShow, void* Obj, CBDowningInfo func, UNINT nShowCircle = 500);
    UNINT GetDownCount();
    bool AsWhole() const { return m_bAsWhole; }
    void AsWhole(bool val) { m_bAsWhole = val; }
	UINT MaxDownObjAsync() const { return m_nMaxDownObjAsync; }
	void MaxDownObjAsync(UINT val) { m_nMaxDownObjAsync = val; }

protected:
    bool AddDownItem(DownFileInfo& dInfo);
    
    /************************************************************************
        fun:	创建多个线程对象下载文件
        param:  
        ret:    
        remark: 
    /************************************************************************/
    bool CreateMultiDownThread();

    

    bool DownFile( tcpchar szUrl);
    
    bool InitVariable();

    

    /************************************************************************
        fun:	底层下载完成后, 调用该接口通知上层管理类
        param:  
        ret:    
        remark: 
    /************************************************************************/
    static void CB_DownNotify(CDownFileManager* pMng, NotifyObj* pNo );

    //bool CanRetry( DownState );
    CDownFileManager* Clone();
    bool MoveToDone( DownList::iterator it );
    bool MoveToDone( ItemID nID);
    bool NewThreadDown();
    void ClearTaskState( DownFileInfo& dfi );
    ns_down_file::ItemID MakeTaskID();
    int CanCreateThreadCount();
    void AddDownObj( CTrDownFile* pDown );
    bool SetProxyServerList(const LST_STRING& lstProxy) ;
    bool SetAllDownProxy();
    bool StartAllDown();
    bool DealDownRet();
    bool DistributeTask( CTrDownFile* pDown );
    DownList GetTask( int param1 );
protected:
    DownList		m_lstToDown;		//下载列表
	DownList		m_lstDowned;		//已下载列表
	
 	UNLONG			m_dwSingleSizeLimit;	//单一文件下载大小限制
    bool            m_bQuitDown;        //是否停止下载
    auto_ptr<CLock>           m_lockDownList;
    auto_ptr<CLock>           m_lockDownObjList;
    auto_ptr<CLock>           m_lockID;

    auto_ptr<CDowningInfoMng>               m_pDownInfoMng;
    CBDowningInfo m_pCBDowningInfoFun;      //下载中显示进度,速度的回调函数
    
    //默认参数
    void*           m_pDownInfoCBAdd;   //下载中回调函数的用户参数
    tstring         m_strSavePathDefault;   //默认的保存路径
    static CLockData<int> nThreadCount;
    DownObjList             m_lstDownObj;
    LST_STRING      m_lstProxyServer;
    //CProxyPC*               m_proxyUse; //引用
    bool                    m_bStopDown;
    bool                    m_bAsWhole; //所有下载项是否作为一个整体
    UINT					m_nMaxDownObjAsync;	//最多同时下载对象
	
};