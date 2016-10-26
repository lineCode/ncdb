#include "StdAfx.h"

#include "SockClientManager.h"
#include "SockTcp.h"
#include "StlSupply.h"
#include <algorithm>
#include <Lock.h>
#include "PiTypeDef.h"
typedef CSockClientManager::CLIENT_DATA CLIENT_DATA;

bool PredCloseSocket(CLIENT_DATA& s);
void PredTerminateThread(CLIENT_DATA& hThread);
bool PredFilterIp(const CLIENT_ADDRESS& caL, const CLIENT_ADDRESS& caR);

bool PredFindIp(CLIENT_DATA& cd, const string& strIp);

const UNLONG WAIT_CLIENT = 2000;//毫秒，等待所有客户端线程关闭的时间

CSockClientManager::CSockClientManager(void)
{
	
    InitVariable();
}

CSockClientManager::~CSockClientManager(void)
{
	
}

void CSockClientManager::InitVariable()
{
    m_pVtClientsSock        = NULL;
    m_pLock                 = NULL;
    m_pLockFilter           = NULL;
    m_pLockDisFilter        = NULL;

    m_iParam.bDisFilter    = false;
}

bool CSockClientManager::Init()
{
    m_pLock             = new CLock;
    m_pLockFilter       = new CLock;
    m_pLockDisFilter    = new CLock;
	//InitializeCriticalSection(&m_cs);
	m_pVtClientsSock = new ClientInfoList;
	return true;
}

bool CSockClientManager::UnInit()
{
    CloseAll();
	if (m_pVtClientsSock)
	{
		delete m_pVtClientsSock;
		m_pVtClientsSock = NULL;
	}

    if (m_pLock)
    {
        delete m_pLock;
        m_pLock = NULL;
    }

    if (m_pLockFilter)
    {
        delete m_pLockFilter;
        m_pLockFilter       = NULL;
    }
    if (m_pLockDisFilter)
    {
        delete m_pLockDisFilter;
        m_pLockDisFilter    = NULL;
    }
	return true;
}

bool CSockClientManager::SetInitParam(const INIT_PARAM& iParam)
{
    m_iParam   = iParam;
    return true;
}

bool CSockClientManager::Add(CLIENT_DATA& ci)
{
	m_pLock->Lock();
	m_pVtClientsSock->push_back(ci);
	m_pLock->UnLock();
	return true;
}

UNLONG  CSockClientManager::Size()
{
    m_pLock->Lock();
    UNLONG nSize = m_pVtClientsSock->size();
    m_pLock->UnLock();
    return nSize;
}

bool CSockClientManager::Delete(const CSockTcp* s)
{
    //std::shared_ptr<int> df;
    //auto_ptr
	m_pLock->Lock();
    for(ClientInfoList::iterator it = m_pVtClientsSock->begin(); it != m_pVtClientsSock->end(); )
    {
        if (s == it->pClientSockTcp)
        {
            it = m_pVtClientsSock->erase(it);//只删除一个
            m_pLock->UnLock();
            return true;
        }
        else
        {
            ++it;
        }
    }

	m_pLock->UnLock();;
	return true;
}

bool CSockClientManager::CloseAll()
{
    m_pLock->Lock();
    //关闭所有客户端连接,之后，客户端线程就会退出
    for_each(m_pVtClientsSock->begin(), m_pVtClientsSock->end(), PredCloseSocket);
    m_pLock->UnLock();

    HANDLE* pHandle = ToArray();
    DWORD dwRet = WaitForMultipleObjects(m_pVtClientsSock->size(), pHandle, true, WAIT_CLIENT);
    delete [] pHandle;
    //从列表中删除
    m_pVtClientsSock->clear();


    //判断是否超时
    if (WAIT_TIMEOUT == dwRet)
    {
        Sleep(5000);
        //timeout
        for_each(m_pVtClientsSock->begin(), m_pVtClientsSock->end(), PredTerminateThread);
    }
    else
    {
        //all thread is exit safely!
    }
    
    return true;
}

HANDLE* CSockClientManager::ToArray()
{
    HANDLE* pHandle = new HANDLE[m_pVtClientsSock->size()];
    UNINT iPos = 0;
    for(ClientInfoList::iterator it = m_pVtClientsSock->begin(); it != m_pVtClientsSock->end();++it)
    {
        pHandle[iPos++] = it->hThread;
    }
    return pHandle;
}

bool CSockClientManager::FindIp(const string& strIp)
{
    m_pLock->Lock();
    ClientInfoList::iterator it;
    it = find_if(m_pVtClientsSock->begin(), 
        m_pVtClientsSock->end(), 
        PredFindIp, 
        strIp
        );
    bool bRet = m_pVtClientsSock->end() != it;
    m_pLock->UnLock();
    return bRet;
}

bool CSockClientManager::Filter(const CLIENT_ADDRESS& ca)
{
    //查找过滤列表
    if(m_lstFilterList.end() != find_if(m_lstFilterList.begin(), m_lstFilterList.end(), PredFilterIp, ca))
    {
        //在过滤列表里,过滤掉
        return false;
    }

    if (m_iParam.bDisFilter)
    {
        //不在筛选列表里的, 则过滤掉
        if(m_lstDisFilterList.end() == find_if(m_lstDisFilterList.begin(), m_lstDisFilterList.end(), PredFilterIp, ca))
        {
            return false;
        }
    }
    return true;
}



void CSockClientManager::AddFilter(const CLIENT_ADDRESS& caR)
{
    m_pLockFilter->Lock();
    m_lstFilterList.push_back(caR);
    m_pLockFilter->UnLock();
}

bool CSockClientManager::WithDrawFilter(const CLIENT_ADDRESS& caR)
{
    m_pLockFilter->Lock();
    DeleteVector(m_lstFilterList, caR);
    m_pLockFilter->UnLock();
    return true;
}

void CSockClientManager::AddDisFilter(const CLIENT_ADDRESS& caR)
{
    m_pLockDisFilter->Lock();
    m_lstDisFilterList.push_back(caR);
    m_pLockDisFilter->UnLock();
}

bool CSockClientManager::WithDrawDisFilter(const CLIENT_ADDRESS& caR)
{
    m_pLockDisFilter->Lock();
    DeleteVector(m_lstDisFilterList, caR);
    m_pLockDisFilter->UnLock();
    return true;
}


/*************************** 谓语 *******************************/

bool PredFilterIp(const CLIENT_ADDRESS& caL, const CLIENT_ADDRESS& caR)
{
    /************************************************************************/
    /* 过滤规则: 
                ip 一样    
                    port = 0
                    port = port
                -> 过滤掉
    /************************************************************************/
    if (caL.strIp == caR.strIp 
        && (caL.uPort == 0 || caL.uPort == caR.uPort)
        )
    {
        return true;
    }
    return false;
}
bool PredCloseSocket(CLIENT_DATA& client)
{
    if (client.pClientSockTcp)
    {
        client.pClientSockTcp->Close();
        /*closesocket(client.sock);
        client.sock= NULL;*/
    }
    
    return true;
}

void PredTerminateThread(CLIENT_DATA& client)
{
    if(TerminateThread(client.hThread, 0))
    {
        client.hThread = NULL;
    }
    
}

bool PredFindIp(CLIENT_DATA& cd, const string& strIp)
{
    return cd.pClientSockTcp->GetClientIp() == strIp;
}