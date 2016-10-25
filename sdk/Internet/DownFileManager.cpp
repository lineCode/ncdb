#include "StdAfx.h"
#include <assert.h>
#include "DownFileManager.h"
#include <string>
#include "Stlmath.h"
#include "define_gnl.h"
//#include "FileStrOp.h"
#include "Math/mathUnit.h"
#include "ResCommon.h"
#include "Lock.h"
#include "AutoType.h"
#include "functional.h"
#include <LockData.h>
#include "TrDownFile.h"
#include "RAIILock.h"
//#include "ProxyPC.h"

const UNINT     NUM_MAX_DOWN_OBJECT_DEFAULT  = 5;
using std::string;
using namespace std::tr1;
Pi_NameSpace_Using

// #define FOR_EACH_DOWN_TASK(Cn, It) \
//     FOR_EACH_ITER(DownList, Cn, It)

const UNLONG NUM_SINGLE_FILE_SIZE_LIMIT_DEFAULT = NUM_FILE_SIZE_1M * 50;

CLockData<int> CDownFileManager::nThreadCount = 0;

CDownFileManager::CDownFileManager( CallBackFun pCB /*= NULL*/, void* pObject /*= NULL*/)
	:CThreadLight(pCB, pObject)
{
	InitVariable();
    SetDowningInfoCB(pCB, pObject);
}


CDownFileManager::~CDownFileManager(void)
{
    UnInit();
    
}

bool CDownFileManager::Init()
{
    CThreadLight::Init();
    Create_Auto_Ptr(m_lockDownList, CLock);
    Create_Auto_Ptr(m_lockDownObjList, CLock);
    Create_Auto_Ptr(m_lockID, CLock);
    //m_proxyUse = CProxyPC::GetInst();
    this->SetDefaultSavePath(GetSelfModuleFolder().c_str());
    CThreadLight::SetThreadName(_T("thread_CDownFileManager"));
	return true;
}

bool CDownFileManager::UnInit()
{
    ExitAllDown();
    //this->QuitDownFile();
	return true;
}

DWORD CDownFileManager::DoWork()
{
	/************************************************************************
		遍历下载列表中每个文件
	/************************************************************************/
    CreateMultiDownThread();
    return 1;
}



bool CDownFileManager::AddDownItem( DownFileInfo& dInfo )
{
    CRAIILock raii(m_lockDownList->Lock());
	m_lstToDown.push_back(dInfo);

//     CAutoType at(m_lstToDown.size());
//     OutputDebugString(_T("down list size: "));
    //DEBUG_INFO((tcpchar)at);
    //m_lockDownList->UnLock();
	return true;
}

bool CDownFileManager::AddDownItemTask( DownFileInfo& dfi )
{
    dfi.itemID = MakeTaskID();
    return AddDownItem(dfi);
}

bool CDownFileManager::AddDownItemTask( tcpchar szUrl )
{
    DownFileInfo dfi;
    dfi.strUrl = szUrl;
    dfi.strSavePath     = m_strSavePathDefault;
    dfi.bUseServerName   = true;
    return this->AddDownItemTask(dfi);
}

bool CDownFileManager::AddDownItemTask( tcpchar szUrl, tcpchar szSavePath )
{
    DownFileInfo dfi;
    dfi.strUrl = szUrl;
    dfi.strSavePath     = szSavePath;
    return this->AddDownItemTask(dfi);
}

bool CDownFileManager::AddDownItemTask( DownList& lstDown)
{
    FOR_EACH_DOWN_TASK(lstDown, it)
    {
        AddDownItemTask(*it);
    }
    return true;
}

bool CDownFileManager::InitVariable()
{
	m_dwSingleSizeLimit		= NUM_SINGLE_FILE_SIZE_LIMIT_DEFAULT;
    m_pDownInfoCBAdd        = NULL;
    m_pCBDowningInfoFun     = NULL;
    m_bStopDown             = false;
    m_bQuitDown             = false;
    m_bAsWhole              = false;

	m_nMaxDownObjAsync		= NUM_MAX_DOWN_OBJECT_DEFAULT;
	return true;
}

bool CDownFileManager::DownFile( tcpchar szUrl )
{
    assert(0);
    return true;
}

void CDownFileManager::ClearAllDownItem()
{
    CRAIILock raii(m_lockDownList->Lock());
	m_lstToDown.clear();
    TRACE(_T("down task clear\n"));
}

void CDownFileManager::SetSingleFileMaxLimit( UNLONG dwSize )
{
	m_dwSingleSizeLimit = dwSize;
}

bool CDownFileManager::StartDown()
{
    return this->Run();
    
}

void CDownFileManager::QuitDownFile()
{
    StopAllDown();
    //停止所有下载对象
    /*FOR_EACH_DOWN_TASK(m_lstToDown, it)
    {
        //it->bUseServerName/
    }*/

    /*m_bQuitDown = true;
    DEBUG_INFO("下载管理类 设置退出");
    m_pInternet->QuitDown();        //停止内部模块的下载*/
}

void CDownFileManager::SetDowningInfoCB( CBDowningInfo pFun, void* pAdd )
{
    m_pCBDowningInfoFun = pFun;
    m_pDownInfoCBAdd = pAdd;
}

void CDownFileManager::SetDefaultSavePath( tcpchar szPath )
{
    m_strSavePathDefault = szPath;
}

CDownFileManager* CDownFileManager::Clone()
{
    CDownFileManager* p = new CDownFileManager(m_pCBDowningInfoFun, m_pDownInfoCBAdd);
    return p;
}

bool CDownFileManager::GetNextTask( DownList::iterator& itL )
{
    CRAIILock raii(m_lockDownList->Lock());

    bool bHas = false;
    DownList::iterator it = m_lstToDown.begin();

    while(IsValidIter(it, m_lstToDown) 
        && (it->taskState != ts_new_add))
    {
        ++it;
    }

    if (IsValidIter(it, m_lstToDown))
    {
        it->taskState = ts_downing;
        itL = it;
        bHas = true;
        return bHas;
    }
    it = m_lstToDown.end();
    return bHas;
}

bool CDownFileManager::MoveToDone( DownList::iterator it )
{
    m_lstDowned.push_back(*it);

    CRAIILock raii(m_lockDownList->Lock());

    m_lstToDown.erase(it);
    return true;
}

bool CDownFileManager::MoveToDone( ItemID nID)
{
    bool bRet = true;
    m_lockDownList->Lock();
    FOR_EACH_DOWN_TASK(m_lstToDown, it)
    {
        if (it->itemID == nID)
        {
            bRet = MoveToDone(it);
            break;
        }
    }
    m_lockDownList->UnLock();
    return bRet;
}
#if 0

bool CDownFileManager::NewThreadDown()
{
    CDownFileManager* pNew = this->Clone();//克隆里面集成了构造函数的参数
    pNew->Init();
    /***************** 获取一个下载任务, 传给克隆出来的对象来下载 *****************/
    /* 待下载任务数 - 1*/
    
    //DownFileInfo dfi;
    DownList::iterator it;
    //it = GetNextTask();
    if(!IsValidIter(it, m_lstToDown))
    {
        return true;
    }
    DownFileInfo dfi = *it;
    ClearTaskState(dfi);
    pNew->AddDownItem(dfi);
    pNew->Run();
    return true;
}
#endif

void CDownFileManager::ClearTaskState( DownFileInfo& dfi )
{
    dfi.dState = err_unKnown;
}

bool CDownFileManager::CreateMultiDownThread()
{
    /***************** 创建多个下载对象, 并增加到列表中, 便于管理 *****************/
    int nThread = CanCreateThreadCount();
    //nThread = 5;
    
    FOR_COUNT(nThread, i)
    {
        DownList::iterator it;
        {
            //shared_ptr<CLock> raii(m_lockDownList.get(), FunUnLock);
            if (!GetDownCount())
            {
                return true;
            }
            /*if (!GetNextTask(it))
            {
                return true;
            }*/
        }

        CTrDownFile*        pDown = CreateDownFileObj();
        pDown->DowningInfoMng(m_pDownInfoMng.get());
        if(!pDown->Init())
        {
            return false;
        }

        //m_lockDownList->UnLock();
        //pDown->SetProxy(m_proxyUse->GetProxy().c_str());
        DoNewDown(pDown);
        pDown->SetFileSizeLimit(m_dwSingleSizeLimit);
        DistributeTask(pDown);
        //pDown->AddDownTask(*it);
        AddDownObj(pDown);
        pDown->Run();

        /*++nCount;
        CAutoType at(nCount);
        OutInfo(_T("total create thread:"), at);*/

    }
    return true;
}

bool CDownFileManager::CB_GetNextDownTask( CDownFileManager* pMng, CTrDownFile* pDown )
{
    //pMng->
//     DownList::iterator it = pMng->GetNextTask();
//     pDown->AddDownTask(*it);
    return true;
}

void CDownFileManager::CB_DownNotify( CDownFileManager* pMng, NotifyObj* pNo )
{
    if(pNo->nType == type_single_file)
    {
        CTrDownFile* pTrDown = static_cast<CTrDownFile*>(pNo->pAdd);
        DownFileInfo* pdfi = pTrDown->GetDownInfo();
        tstring strPath = pdfi->strSavePath;
        assert(strPath == pdfi->strSavePath);

        pMng->DealDownRet();
        switch(pdfi->dState)
        {
        case ok_completion:
            pMng->MoveToDone(pdfi->itemID);
            break;

        case err_server_403:
        case err_proxy:
            pMng->DealDownFail(pTrDown);
            //下载失败
            break;
        default:
            OutInfo(_T("down fail"));
            break;

        }
        assert(strPath == pdfi->strSavePath);
    }

    
    pMng->Notify((NotifyObj*)pNo);
}

ns_down_file::ItemID CDownFileManager::MakeTaskID()
{
    static ItemID nID = 0;
    CRAIILock raii(m_lockID->Lock());
    return nID++;
}

int CDownFileManager::CanCreateThreadCount()
{
    int nCount = 0;
    //可创建对象数 + 内部线程以结束的对象个数
    CRAIILock raii(m_lockDownObjList->Lock());
    nCount = max( m_nMaxDownObjAsync -  m_lstDownObj.size(), 0);

    for (DownObjList::iterator it = m_lstDownObj.begin(); it != m_lstDownObj.end(); )
    //FOR_EACH_ITER(DownObjList, m_lstDownObj, it)
    {
        CTrDownFile* pDown = (*it);
        if ( !pDown->IsThreadRunning() )
        {
            OutInfo(_T("delete CTrDownFile"));
            delete pDown;
            OutInfo(_T("delete CTrDownFile ok"));
            it = m_lstDownObj.erase(it);
            ++nCount;
        }
        else
        {
            ++it;
        }
        
    }
    return nCount;
}

void CDownFileManager::AddDownObj( CTrDownFile* pDown )
{
    CRAIILock raii(m_lockDownObjList->Lock());
    m_lstDownObj.push_back(pDown);
}

bool CDownFileManager::SetProxyServerList(const LST_STRING& lstProxy) 
{
    m_lstProxyServer = lstProxy;
    return true;
}

bool CDownFileManager::InitData()
{
    return true;   
}


bool CDownFileManager::DealDownFail( CTrDownFile* pTrDown )
{
    /*if (pTrDown->IsUsedProxy()/ *代理ip已经用过, * /)
    {
        m_proxyUse->GetNextProxy();
    }
    DownFileInfo dfi = *(pTrDown->GetDownInfo());
    dfi.dState = err_user_stop;

    pTrDown->SetProxy(m_proxyUse->GetProxy().c_str());
    pTrDown->AddDownTask(dfi);
    //AddDownItemTask();
    
    //停止所有下载
    StopAllDown();
    //为所有下载对象设置新的代理
    SetAllDownProxy();
    //开启所有下载
    StartAllDown();

    this->Run();*/
    return true;
}

bool CDownFileManager::StopAllDown()
{
    CRAIILock raii(m_lockDownObjList->Lock());    
    FOR_EACH_ITER(DownObjList, m_lstDownObj, it)
    {
        CTrDownFile* pDown = *it;
        pDown->StopDown();
    }
    return true;
}

bool CDownFileManager::ExitAllDown()
{
    CRAIILock raii(m_lockDownObjList->Lock());    
    CAutoType at;
    at.SetData(GetCurrentThreadId());
    //OutInfo(_T("CDownFileManager::ExitAllDown call ThreadID:"), at);

    FOR_EACH_ITER_NO_ADD(DownObjList, m_lstDownObj, it)
    {
        CTrDownFile* pDown = *it;
        pDown->ExitDown();
        //OutInfo(_T("--end pDown->ExitDown() "));
        ClearMem(pDown);
        //OutInfo(_T("--end ClearMem(pDown)"));
        it = m_lstDownObj.erase(it);
    }
    //OutInfo(_T("ExitAllDown ok"));
    return true;
}

bool CDownFileManager::StartAllDown()
{
    CRAIILock raii(m_lockDownObjList->Lock());    
    FOR_EACH_ITER(DownObjList, m_lstDownObj, it)
    {
        CTrDownFile* pDown = *it;
        pDown->ResumeDown();
    }
    return true;
}

bool CDownFileManager::DealDownRet()
{
    //OutInfo(_T("downThread: "), m_lstDownObj.size());
    return true;
}

CTrDownFile* CDownFileManager::CreateDownFileObj()
{
    return new CTrDownFile((CallBackFun)&CDownFileManager::CB_DownNotify, this);
}

UNINT CDownFileManager::GetDownThreadCount() const
{
    return m_lstDownObj.size();
}

bool CDownFileManager::DoRunBegin()
{
    return !m_bStopDown;
}

bool CDownFileManager::StopRunNewTask()
{
    m_bStopDown = true;
    return true;
}

bool CDownFileManager::StartRunNewTask()
{
    m_bStopDown = false;
    return true;
}

bool CDownFileManager::DoNewDown( CTrDownFile* pDown )
{
    return true;
}

bool CDownFileManager::SetShowDownInfo( bool bShow, void* Obj, CBDowningInfo func, UNINT nShowCircle /*= 500*/ )
{
    if (bShow)
    {
        m_pDownInfoMng = auto_ptr<CDowningInfoMng>(new CDowningInfoMng(func, Obj));
        m_pDownInfoMng->Init();
        m_pDownInfoMng->SetShowCircle(nShowCircle);
        m_pDownInfoMng->Run();
    }
    return true;
}

UNINT CDownFileManager::GetDownCount()
{
    CRAIILock raii(m_lockDownList->Lock());
    return m_lstToDown.size();
}

bool CDownFileManager::DistributeTask( CTrDownFile* pDown )
{
    DownList dl;
    if (m_bAsWhole)/* 视为单一团任务*/
    {
        dl = GetTask(-1);
    }
    else
    {
        dl = GetTask(1);
    }
    pDown->AddDownTask(dl);

    return true;
}

DownList CDownFileManager::GetTask( int param1 )
{
    CRAIILock raii(m_lockDownList->Lock());
    assert(m_lstToDown.size());

    int nSize = 0;
    if (param1 == -1)
    {
        nSize = m_lstToDown.size();
    }
    else
    {
        nSize = min(param1, m_lstToDown.size());
    }
    DownList dl;
    //dl.resize(nSize);

    
    DownList::iterator itEnd = m_lstToDown.begin();
    advance(itEnd, nSize);
    dl.splice(dl.begin(), m_lstToDown, m_lstToDown.begin(), itEnd);

    return dl;

}
