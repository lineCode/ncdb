#pragma once
#include <string>
#include "PiTypeDef.h"
#include <map>
#include "ThreadLight.h"
#include <memory>
#include "InetDef.h"
#include "DowningInfoDef.h"
using std::string;
using std::map;
using std::auto_ptr;

class CLock;
class CEventLight;

using namespace ns_down_file;



class CDowningInfoMng
    :public CThreadLight
{
    typedef map<ItemID, DowningItemInfo> LST_DOWN_ITEM_INFO;
public:
    CDowningInfoMng(CBDowningInfo pFun = NULL, void* pAdd = NULL);
    ~CDowningInfoMng(void);
public:
    bool Init();
    bool UnInit();
    virtual DWORD DoWork();
    /************************************************************************
        fun:	添加要显示信息的下载任务
        param:  
        ret:    
        remark: 
	/************************************************************************/
    ItemID AddDownItem(CDownItem*);
    void UpdaterData(ItemID id, UNLONG nDownedSize, bool bClearSpeed = false);
    void UpdaterData(ItemID id, DowningItemInfo& dii);
    void UpdaterAllItem();
    void EraseItem(ItemID id);
    void ClearInfo(ItemID id);
    ItemID GetItemId( CDownItem* pDI );
    DowningItemInfo* GetItemDownInfo( CDownItem* pDI );
    ItemID MakeNewID();
    bool Show();
    void SetTotalSize( ItemID id, UNLONG nSize );
    void SetShowCircle( UNINT nCircle) { m_ShowCircleTime = nCircle; }
    bool ShowOn();
    bool ShowOff();
private:
    CBDowningInfo           m_pCBFun;    
    void*                   m_pAdd;
    LST_DOWN_ITEM_INFO    m_DownItemlist;
    UNINT               m_ShowCircleTime;   //计算速度的周期时间,ms
    auto_ptr<CLock>           m_pLockShowDowningInfo;
    ItemID                  m_idNext;        //下一个任务id
    auto_ptr<CEventLight>             m_pEvent;
    bool                    m_bShow;
};

