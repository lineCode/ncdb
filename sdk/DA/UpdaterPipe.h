#pragma once
#include "pipemanage.h"
#include "UpdaterPipeDef.h"

typedef bool (*CB_READ_DATA)(void*, void*);
class CUpdaterPipe :
    public CPipeManage
{
public:
    using CPipeManage::Write;

    CUpdaterPipe(void);
    ~CUpdaterPipe(void);
public:
    virtual bool DealReadData();
    bool SetHwnd(HWND hWnd);
    void SetCB(void* pObj, CB_READ_DATA pFunc);
    bool WriteUpdateState(tagUPDATE_PERCENT tag);
    bool Write(const tagSTATE_NOTIFY& us);
    bool WriteError(ENUM_UPDATER_ERROR_TYPE errType);

    bool DealUpdateState();
    bool DealStateNotify();
    virtual bool DealReadFail();
    bool DealRequestExit();
    static ENUM_PIPE_DATA_TYPE GetDataType( void* pAdd );
private:
    HWND                    m_hwnd;
    void*                   m_pObj;
    CB_READ_DATA            m_fun;
};

