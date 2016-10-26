#include "StdAfx.h"
#include "AgentClientManager.h"
#include "SockClientManager.h"

CAgentClientManager::CAgentClientManager(void)
{
}

CAgentClientManager::~CAgentClientManager(void)
{
}

void CAgentClientManager::SetSource(CSockClientManager* pManager)
{
    m_pSockClientManager = pManager;
}

bool CAgentClientManager::FindIpHasConnect(const string& strIp)
{
    return m_pSockClientManager->FindIp(strIp);
}