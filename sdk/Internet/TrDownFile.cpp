#include "StdAfx.h"
#include "TrDownFile.h"
#include "InternetHttp.h"
#include "define_gnl.h"
#include "Lock.h"
#include "ResCommon.h"
#include "DownFileManager.h"
#include "functional.h"
#include "EventLight.h"
#include "RAIILock.h"
#include "AutoType.h"
#include <UCommon.h>
#include "StrCoding.h"
#include "InetDef.h"
#include "PiBPDown.h"
#include "FileOpr.h"
#include "k_STLString.h"
#define NUM_RETRY_TIMES_MAX     5   //一个文件失败最大可重试次数

#define     STRING_DOWING_FILE_SUFFIX_DEFAULT       _T("__pf")

CTrDownFile::CTrDownFile( CallBackFun pCB, void* pObject/*= NULL */ )
    :CThreadLight(pCB, pObject)
{
    SetDowningInfoCB(pCB, pObject);
    m_nCurRetryTimes        = 0;
    m_bUsedProxy            = false;
    m_bQuitDown             = false;
    m_dwSingleSizeLimit     = 0;
    m_pDowningInfoMng       = NULL;
    m_it                    = nullptr;
}


CTrDownFile::~CTrDownFile(void)
{
    UnInit();
    CAutoType at((UNLONG)this);
    //OutInfo(_T("CTrDownFile desc, this:"), at);

    if (m_pDowningInfoMng && m_pInternet.get())
    {
        m_pDowningInfoMng->EraseItem(m_pInternet->GetShowID());
    }
    m_lstToDown.clear();
}

bool CTrDownFile::Init()
{
    CThreadLight::Init();
    m_pInternet = auto_ptr<CInternetHttp>(new CInternetHttp);
    InitInetObj();
    
    Create_Auto_Ptr(m_pEvPause, CEventLight);
    Create_Auto_Ptr(m_lockDownList, CLock);
    Create_Auto_Ptr(m_pLockProxy, CLock);
    
    
    //m_pDowningInfoMng->Init();
    //m_pInternet->SetDowningInfoMng(m_pDowningInfoMng.get());
    this->SetWaitThreadTimeout(600);
    m_pEvPause->SetWaitTimeout(INFINITE);
    CThreadLight::SetThreadName(_T("thread_CTrDownFile"));
    //m_pDowningInfoMng->Run();

    return true;
}

void CTrDownFile::SetDowningInfoCB( CBDowningInfo pFun, void* pAdd )
{
    m_pCBDowningInfoFun = pFun;
    m_pDownInfoCBAdd = pAdd;
}
bool CTrDownFile::DownFile( DownFileInfo& dInfo )
{
	/************************************************************************
		调用底层 CInternetHttp 来下载
	/************************************************************************/
    //CRAIIHandle raii(m_pInternet.get());

    shared_ptr<CInternetHttp> raii(m_pInternet.get(), &CInternetHttp::Clear);

    tstring     strProxy;
    {
        //dInfo.strUrl = _T("http://zhidao.baidu.com/");
        CRAIILock raii(m_pLockProxy->Lock());
        strProxy = m_strProxyName;
        if(!m_pInternet->DownloadInit(dInfo.strUrl.c_str(), tstring(strProxy).c_str()))
        {
            return false;
        }
        SetHasUseProxy();
    }
	
 	
    UNLONG dwSize = 0;
    //m_pInternet->QueryFileSize();

    if (! m_pInternet->QueryFileSize( dwSize ) && (err_get_file_size != m_pInternet->GetErrType()))//忽略获取报头失败
	{
        SaveErrInfo(dInfo, m_pInternet.get());

		/*dInfo.nServerCode   = m_pInternet->GetServerState();
        dInfo.nErrCode      = m_pInternet->ErrCode();*/
		return false;
	}
    else if( dwSize > m_dwSingleSizeLimit )
 	{
 		dInfo.dState = err_too_big;
 		return false;
 	}

    
	//string strObject = m_pInternet->FindHttpObject(dInfo.strUrl.c_str());
	//strObject = "/file/6b1dceb2faa88f7e7dab9813c5c10628?fid=2449563447-250528-4153109198&time=1370076728&sign=FDTAR-DCb740ccc5511e5e8fedcff06b081203-3Svvb%2BD8fMxl6Q4yi0oN3Y%2Byq2c%3D&rt=sh&expires=8h&r=164699213&sh=1&response-cache-control=private";
    DWORD dwOffSet = 0;
    CPiBPDown bpDown(m_pInternet.get(), dInfo.bNeedBPDown);
    if(!bpDown.CalcDownedSize(dInfo.strSavePath))
    {
        //删除本地同名, 重新下载
        CFileUtil::DeleteFile(TO_CSTRING(dInfo.strSavePath));
    }


    //更新进度
    if (dInfo.taskState != ts_retry)
    {
        STLString strMsg;
        strMsg.Format(_T("add progress:%d, url:%s"), bpDown.GetDownedSize(), dInfo.strUrl.c_str());
        OutInfo(strMsg.c_str());
        m_pInternet->UpdateData(bpDown.GetDownedSize(), true);
    }

    bool bRet = false;
    do 
    {
        bRet = m_pInternet->DownloadObject(NULL, dInfo.strSavePath.c_str());
    } while ( !bRet && m_pInternet->CanRetryCn() && bpDown.SetDataOffset(bpDown.GetDownedSize() + m_pInternet->GetDownSize()));

    if ( ! bRet )
    {
        SaveErrInfo(dInfo, m_pInternet.get());
        m_pInternet->DownloadUnInit();
        return false;
    }
    if (m_pInternet->IsRetryOK())
    {
        OutInfo(_T("retry down ok"));
    }

	m_pInternet->DownloadUnInit();
    bpDown.ClearBPInfo(dInfo.strSavePath);
    dInfo.dState = ok_completion;
	return true ;
}

DWORD CTrDownFile::DoWork()
{
    CAutoType at;
    at.SetData(GetCurrentThreadId());
    //OutInfo(_T("new CTrDownFile thread id:"), at.ToHex());
    DownList lstTemp;
    //FOR_EACH_ITER(DownList, m_lstToDown, it)

    m_it = new DownList::iterator;
    DownList::iterator& it = *m_it;
    
    //TODO:改为从原始列表获取(做资源保护), 而不是从副本列表获取
    //FOR_EACH_DOWN_TASK(dLst, it)
    m_pDowningInfoMng->AddDownItem(m_pInternet.get());
    //TODO:设置更新进度的回调接口, 返回给本类, 单一进度或多文件统一在这里处理
    UNLONG dwSizeTotal = 0;
    UNLONG dwSizeTotalDowneded = 0;
    bool bErr = false;
    
    UNINT nFailFileCount = 0;
    DownFileInfo dfi;

    if ( ! GetAllFileSize(dwSizeTotal))
    {
        bErr = true;
        ++nFailFileCount;
        dfi.dState = err_unKnown;   //网络问题, 或者获取大小字段出错
    }
    else
    {
        SetShowTotalSize(dwSizeTotal);
    }

    while(! bErr)
    {
        it = GetFirstTask();
        if (it == m_lstToDown.end())
        {
            GetNextTask();
            it = GetFirstTask();
            if (it == m_lstToDown.end())
            {
                break;
            }
        }
        m_pDownInfo = &*it;
        while(1)
        {
            //是否暂停了下载, 等待开启继续
            if (it->dState == err_user_stop)
            {
                m_pEvPause->GetLock();
            }
            tstring strNameSrc = it->strSavePath;
            it->strSavePath += STRING_DOWING_FILE_SUFFIX_DEFAULT;
            tstring strTempName = it->strSavePath;
            bool bDownSuc = DownFile(*it);

            
            dwSizeTotalDowneded += m_pInternet->GetDownSize();

            

            STLString strInfo;
            strInfo.Format(_T("downed Size:%d, local file size:%d, file total size:%d")
                , m_pInternet->GetDownSize(), CFileOpr::GetFileSize(strTempName.c_str()), m_pInternet->GetFileSize());
            OutInfo(strInfo.c_str());

            it->strSavePath = strNameSrc;
            //dfi = *it;  //保存最后一个下载失败的任务

            if(!bDownSuc)
            {
                GetErrInfo(*it);
                DealDownFail(*it);
                ++m_nCurRetryTimes;
                if(1)
                //if(!CanRetry(it->dState) || RetryOut())
                {
                    OutInfo(_T("can't retry :"), it->strUrl.c_str());
                    ++nFailFileCount;
                    m_bQuitDown = true;
                    break;
                }
                else
                {
                    it->taskState = ts_retry;
                    ClearState();
                    //it->strSavePath = strNameSrc;
                    
                    CRAIILock raii(m_lockDownList->Lock());
                    continue;   //重试下载
                }
            }
            CFileUtil::DeleteFile(TO_CSTRING(strNameSrc));
            
            rename(pw2psSmart(strTempName.c_str()), pw2psSmart(strNameSrc.c_str()));
            //it->strSavePath = strNameSrc;

            if (IsReTryOK())
            {
                OutInfo(_T("retry success:"), it->strSavePath.c_str());
            }
            break;
        }
        dfi = *it;  //保存最后一个下载失败的任务

        //如果取消下载, 则忽略当前和剩下的所有下载项, 退出线程
        if (m_bQuitDown)
        {
            break;
        }
        //下载完成或失败都通知外部
        //OutInfo(_T("Down end"));
        //NotifyObj no(type_single_file, &*it);
        NotifyObj no(type_single_file, this);
        Notify(&no);
        CRAIILock raii(m_lockDownList->Lock());
        
        m_lstToDown.erase(it);

        //Sleep(10000);
        m_nCurRetryTimes = 0;
    }
    OutInfo(_T("a batch task total downed:"), dwSizeTotalDowneded);

    //通知显示下载完成
    if (!nFailFileCount)
    {
        DealAllOk();
    }
    //dfi.dState = ok_completion;
    m_pDownInfo = &dfi;
    NotifyObj no(type_all_file_complete, this);
    Notify(&no);

    ClearMem(m_it);//栈上的迭代器析构了
    return 1;
}

bool CTrDownFile::CanRetry( DownState ds)
{
    //404会重试
    return !( m_bQuitDown || /*err_server_404 == ds ||*/  err_too_big == ds
        || IsProxyErr(ds));  //如果是这几种错误， 则不进行重试

}

bool CTrDownFile::AddDownTask( DownFileInfo dfi )
{
    //OutInfo(_T("begin CTrDownFile::AddDownTask"));
    CRAIILock raii(m_lockDownList->Lock());
    m_lstToDown.push_back(dfi);
    //OutInfo(_T("end begin CTrDownFile::AddDownTask"));
    return true;
}

bool CTrDownFile::AddDownTask( DownList dl )
{
    FOR_EACH_DOWN_TASK(dl, it)
    {
        AddDownTask(*it);
    }
    /*CRAIILock raii(m_lockDownList->Lock());
    m_lstToDown.merge(dl);
    OutInfo(_T("end begin CTrDownFile::AddDownTask"));*/
    return true;
}

bool CTrDownFile::GetNextTask()
{
    //TODO: 子类不应该依赖上层管理类
    CDownFileManager* pMng = static_cast<CDownFileManager*>(m_pDownInfoCBAdd);
    DownList::iterator it;
    
    if (!pMng->GetNextTask(it))
    {
        return false;
    }
    AddDownTask(*it);
    return true;
}

void CTrDownFile::SetProxy( tcpchar szProxyName)
{
    //szProxyName = _T("1.93.1.1:222");
    CRAIILock raii(m_pLockProxy->Lock());
    m_strProxyName = szProxyName;
    m_bUsedProxy    = false;
}

void CTrDownFile::GetErrInfo( DownFileInfo& dfi )
{
    if (dfi.dState == ok_completion)
    {
        return ;
    }
    if (dfi.nServerCode == 403)
    {
        dfi.dState = err_server_403;
    }
    else if (HTTP_STATUS_BAD_GATEWAY == dfi.nServerCode 
        || dfi.nErrCode == ERROR_INTERNET_CONNECTION_RESET || dfi.nErrCode == ERROR_INTERNET_CONNECTION_ABORTED
         || ERROR_INTERNET_CANNOT_CONNECT == dfi.nErrCode
         /*|| ERROR_INTERNET_TIMEOUT == dfi.nErrCode*/)
    {
        dfi.dState = err_proxy;
    }
}

bool CTrDownFile::StopDown()
{
    m_pInternet->QuitDown();
    return true;
}

bool CTrDownFile::ExitDown()
{
    m_bQuitDown = true;
    m_pInternet->QuitDown();
    CThreadLight::ExitThread();
    return true;
}

bool CTrDownFile::ResumeDown()
{
    m_pInternet->ResumeDown();
    m_pEvPause->SetEventSign();
    return true;
}

bool CTrDownFile::IsUsedProxy()
{
    CRAIILock raii(m_pLockProxy->Lock());
    return m_bUsedProxy;
}

bool CTrDownFile::SetHasUseProxy()
{
    m_bUsedProxy = true;
    return true;
}

bool CTrDownFile::IsProxyErr( DownState ds )
{
    return ds == err_server_403 || ds == err_proxy;
}

bool CTrDownFile::SaveErrInfo( DownFileInfo& dInfo, CInternetHttp* m_pInternet )
{
    dInfo.nServerCode   = m_pInternet->GetServerState();
    dInfo.nErrCode      = m_pInternet->ErrCode();
    dInfo.dState        = m_pInternet->GetErrType();
    return true;
}

bool CTrDownFile::DealDownFail( DownFileInfo it )
{
    CAutoType at;
    tstring strInfo;
    strInfo = _T("down fail, ");
    strInfo += it.strSavePath;
    strInfo += _T(" url:") + it.strUrl;
    strInfo +=_T(" serverCode:") ;
    strInfo += (tcpchar)at.SetData((int)it.nServerCode);
    strInfo += _T(" downState:");
    strInfo += (tcpchar)at.SetData((int)it.dState);
    strInfo += _T(" ErrCode:");
    strInfo += (tcpchar)at.SetData((int)it.nErrCode);

    OutInfo(strInfo.c_str());
    OutInfo(_T("retry"));
    return true;
}

bool CTrDownFile::RetryOut()
{
    if (m_nCurRetryTimes > NUM_RETRY_TIMES_MAX)
    {
        OutInfo(_T("retry out"));
        return true;
    }
    return false;
}

bool CTrDownFile::IsReTryOK()
{
    bool bOK = m_nCurRetryTimes > 0;
    m_nCurRetryTimes = 0;
    return bOK;
}

bool CTrDownFile::ClearState()
{
    m_pInternet->ClearState();
    return true;
}

bool CTrDownFile::SetHeader( LPCTSTR param1 )
{
    return true;
}

bool CTrDownFile::InitInetObj()
{
    return true;
}

bool CTrDownFile::SetFileSizeLimit( UNLONG nSize )
{
    m_dwSingleSizeLimit = nSize;
    return true;
}

bool CTrDownFile::DealAllOk()
{
    //下载完成, 立即刷新一次显示进度
    m_pDowningInfoMng->Show();
    m_pDowningInfoMng->ClearInfo(m_pInternet->GetShowID());

    return true;
}

bool CTrDownFile::GetAllFileSize( UNLONG& dwSize )
{
    bool bRet = false;
    UNLONG nSizeTotal = 0;
    m_lockDownList->Lock();
    DownList dl = m_lstToDown;
    m_lockDownList->UnLock();
    size_t nSize = dl.size();
    UNLONG dwSizeSingle = 0;
    FOR_EACH_DOWN_TASK(dl, it)
    {
        dwSizeSingle = 0;
        if( ! CInternetHttp::QueryFileSize(it->strUrl.c_str(), dwSizeSingle))
        {
            return bRet;

        }
        nSizeTotal += dwSizeSingle;

    }
    dwSize = nSizeTotal;
    OutInfo(_T("total size:"), dwSize);
    bRet = true;
    return bRet;
}

bool CTrDownFile::SetShowTotalSize( UNLONG nSize )
{
    if (nSize <= 0 )
    {
        return false;
    }
    m_pDowningInfoMng->SetTotalSize(m_pInternet->GetShowID(), nSize);
    return true;
}

DownList::iterator CTrDownFile::GetFirstTask()
{
    DownList::iterator it; 
    m_lockDownList->Lock();
    it = m_lstToDown.begin();
    m_lockDownList->UnLock();
    return it;
}

bool CTrDownFile::UnInit()
{
    bool bRet = ExitDown();
    ClearMem(m_it);
    return bRet;
}
