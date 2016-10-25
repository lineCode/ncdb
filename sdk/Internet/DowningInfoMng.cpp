#include "StdAfx.h"
#include "define_gnl.h"
#include "DowningInfoMng.h"
#include "assert.h"
#include "InternetHttp.h"
#include "Lock.h"
#include "RAIILock.h"
#include "functional.h"
#include "EventLight.h"
#include "ResCommon.h"
CDowningInfoMng::CDowningInfoMng(CBDowningInfo pFun /*= NULL*/, void* pAdd /*= NULL*/)
{
    m_pCBFun = pFun;
    m_pAdd = pAdd;
    m_ShowCircleTime    = 0;
    m_idNext    = 0;
    m_bShow     = true;
    OutInfo(_T("CDowningInfoMng cons"));
}


CDowningInfoMng::~CDowningInfoMng(void)
{
    UnInit();
}

/*
void CDowningInfoMng::UpdaterData( ItemID id, UNLONG nDownedSize )
{
    CRAIILock r(m_pLockShowDowningInfo->Lock());
    LST_DOWN_ITEM_INFO::iterator it = m_DownItemlist.find(id);
    if (it == m_DownItemlist.end())
    {
        return ;
    }
    it->second.dwDowned += nDownedSize;
}*/

void CDowningInfoMng::UpdaterData( ItemID id, DowningItemInfo& dii )
{
    CRAIILock r(m_pLockShowDowningInfo->Lock());
    LST_DOWN_ITEM_INFO::iterator it = m_DownItemlist.find(id);
    if (it == m_DownItemlist.end())
    {
        return ;
    }
    it->second = dii;
}

void CDowningInfoMng::UpdaterData( ItemID id, UNLONG nDownedSize, bool bClearSpeed /*= false*/ )
{
    CRAIILock r(m_pLockShowDowningInfo->Lock());
    LST_DOWN_ITEM_INFO::iterator it = m_DownItemlist.find(id);
    if (it == m_DownItemlist.end())
    {
        return ;
    }
    it->second.dwDowned += nDownedSize;
    if (bClearSpeed)
    {
        it->second.dwDownedCircle = nDownedSize;
    }
}

ItemID CDowningInfoMng::AddDownItem( CDownItem* pDI)
{
    if (!pDI)
    {
        assert(0);
    }
    
    //ItemID id = GetItemId(pDI);
    DowningItemInfo* pdii = GetItemDownInfo(pDI);
    //assert(!pdii->bShow);
    CRAIILock raii(m_pLockShowDowningInfo->Lock());
    pdii->dwTotal = pDI->GetFileSize();
    pdii->bShow = false;
    if (pdii->dwTotal)
    {
        pdii->bShow = true;
    }

    pDI->SetDowningInfoID(pdii->ID);
    pDI->SetDowningInfoMng(this);
    return pdii->ID;
}

DWORD CDowningInfoMng::DoWork()
{
    while(1)
    {
        if ( ! m_bShow)
        {
            m_pEvent->GetLock();

        }
        Sleep(m_ShowCircleTime);
        UpdaterAllItem();
    }
    return 1;
}

void CDowningInfoMng::UpdaterAllItem()
{
    DowningInfoUpdate di;
    di.fPercent = 0;
    di.nSpeed = 0;
    m_pLockShowDowningInfo->Lock();
    //OutInfo(_T("--info show UI DOwnInfo begin"));
    for (LST_DOWN_ITEM_INFO::iterator it = m_DownItemlist.begin(); it != m_DownItemlist.end(); ++it)
    {
        DowningItemInfo& dii = it->second;
        //更新每个下载项的下载信息
        if (dii.bShow && m_pCBFun)
        {
            di.nSizeTotal = dii.dwTotal;
            di.nSizeDowned = dii.dwDowned;

            di.nSpeed = (it->second.dwDowned - it->second.dwDownedCircle );
            di.nSpeed = (int)(1000.0 / m_ShowCircleTime * di.nSpeed);
            di.fPercent = (double)it->second.dwDowned / it->second.dwTotal;
            m_pCBFun(&di, m_pAdd);
            it->second.dwDownedCircle = it->second.dwDowned;
        }
    }
    m_pLockShowDowningInfo->UnLock();
    //OutInfo(_T("--info show UI DOwnInfo end"));
}

bool CDowningInfoMng::Init()
{
    CThreadLight::Init();
    CThreadLight::SetThreadName(_T("CDowningInfoMng"));
    m_ShowCircleTime = 1000;
    m_pLockShowDowningInfo = auto_ptr<CLock>(new CLock);

    Create_Auto_Ptr(m_pEvent, CEventLight);
    m_pEvent->Init();
    m_pEvent->SetWaitTimeout(INFINITE);
    return true;
}

bool CDowningInfoMng::UnInit()
{
    return CThreadLight::ExitThread();
}

void CDowningInfoMng::ClearInfo( ItemID id )
{
    //TODO: 
    
    CRAIILock r(m_pLockShowDowningInfo->Lock());
    LST_DOWN_ITEM_INFO::iterator itFind = m_DownItemlist.find(id);
    if (itFind == m_DownItemlist.end())
    {
        return ;
    }
    itFind->second.bShow        = false;
    itFind->second.dwDowned = 0;
    itFind->second.dwDownedCircle = 0;
}

void CDowningInfoMng::EraseItem( ItemID id )
{
    //TODO: 
    CRAIILock r(m_pLockShowDowningInfo->Lock());
    LST_DOWN_ITEM_INFO::iterator itFind = m_DownItemlist.find(id);
    if (itFind == m_DownItemlist.end())
    {
        return ;
    }
    m_DownItemlist.erase(itFind);
}

ns_down_file::ItemID CDowningInfoMng::GetItemId( CDownItem* pDI )
{
    CRAIILock r(m_pLockShowDowningInfo->Lock());
    LST_DOWN_ITEM_INFO::iterator itFind = m_DownItemlist.end();
    ItemID id = 0;
    //for (DownItemInfo::iterator it = m_DownItemlist.begin(); it != m_DownItemlist.end(); ++it)
     FOR_EACH_ITER(LST_DOWN_ITEM_INFO, m_DownItemlist, it)
     {
         if(it->second.pInet == pDI)
         {
             itFind = it;
         }
     }
//     m_DownItemlist.find(pDI);

    if (itFind != m_DownItemlist.end())
    {
        id = itFind->second.ID;
    }
    else
    {
        id = MakeNewID();
        DowningItemInfo dii;
        dii.ID      = id;
        dii.dwTotal = pDI->GetFileSize();
        dii.pInet    = pDI;
        m_DownItemlist.insert(LST_DOWN_ITEM_INFO::value_type(id, dii));
    }
    return id;
}

ItemID CDowningInfoMng::MakeNewID()
{
    CRAIILock r(m_pLockShowDowningInfo->Lock());
    return ++m_idNext;
}

DowningItemInfo* CDowningInfoMng::GetItemDownInfo( CDownItem* pDI )
{
    DowningItemInfo* pdii = NULL;

    CRAIILock r(m_pLockShowDowningInfo->Lock());
    LST_DOWN_ITEM_INFO::iterator itFind = m_DownItemlist.end();
    ItemID id = 0;
    //for (DownItemInfo::iterator it = m_DownItemlist.begin(); it != m_DownItemlist.end(); ++it)
    FOR_EACH_ITER(LST_DOWN_ITEM_INFO, m_DownItemlist, it)
    {
        if(it->second.pInet == pDI)
        {
            itFind = it;
        }
    }
    //     m_DownItemlist.find(pDI);

    if (itFind != m_DownItemlist.end())
    {
        pdii = &itFind->second;
    }
    else
    {
        id = MakeNewID();
        OutInfo(_T("iMng.ID"), id);
        DowningItemInfo dii;
        dii.ID      = id;
        dii.dwTotal = pDI->GetFileSize();
        dii.pInet    = pDI;
        m_DownItemlist.insert(LST_DOWN_ITEM_INFO::value_type(id, dii));
        pdii = &m_DownItemlist[id];
    }
    return pdii;
}

bool CDowningInfoMng::Show()
{
    UpdaterAllItem();
    //m_pCBFun()
    return true;
}

void CDowningInfoMng::SetTotalSize(ItemID id, UNLONG nSize )
{
    DowningItemInfo dii;
    dii.dwTotal = nSize;
    dii.ID = id;
    UpdaterData(id, dii);
}

bool CDowningInfoMng::ShowOn()
{
    m_bShow = true;
    m_pEvent->SetEventSign();
    return true;
}

bool CDowningInfoMng::ShowOff()
{
    m_bShow = false;
    return true;
}
