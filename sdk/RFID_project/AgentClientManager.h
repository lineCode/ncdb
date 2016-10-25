#pragma once
class CSockClientManager;

class CAgentClientManager
{
public:
    CAgentClientManager(void);
    ~CAgentClientManager(void);

public:
    bool        FindIpHasConnect(const string& strIp);
    void        SetSource(CSockClientManager* pManager);

private:
    CSockClientManager*     m_pSockClientManager;    
};
