#pragma once
#include "PiTypeDef.h"
#include <string>
#include <list>
#include "winsock2.h"

#include <vector>
using namespace std;

class CSockTcp;
class CLock;

//保存tcp客户端的一些信息
class CLIENT_ADDRESS
{
public: 
    

    CLIENT_ADDRESS(string& strIpR, u_short uPortR = 0)
    {
        strIp = strIpR;
        uPort = uPortR;
    }
    bool operator ==(const CLIENT_ADDRESS& caR) const
    {
        return (uPort == caR.uPort && strIp == caR.strIp);
    }
public:
    string strIp;       //客户端ip
    u_short uPort;        //客户端端口
};


class CSockClientManager
{
public:    

    struct INIT_PARAM
    {
        bool    bDisFilter; //启用对客户端进行筛选
    };

    struct CLIENT_DATA
    {
        CSockTcp*   pClientSockTcp;
        //SOCKET sock; 
        HANDLE hThread;
    };
    typedef vector<CLIENT_DATA> ClientInfoList;

public:
	CSockClientManager(void);
	~CSockClientManager(void);

public:
	bool    Init();
	bool    UnInit();
    bool    SetInitParam(const INIT_PARAM& iParam);
    void    InitVariable();
    bool    CloseAll();
    UNLONG  Size();
    /************************************************************************/
    /*  fun: 线程句柄从容器队列里转成数组形式
    /************************************************************************/
    HANDLE* ToArray();
	bool    Add(CLIENT_DATA&);
	bool    Delete(const CSockTcp* s);

    bool    FindIp(const string& strIp);

    /************************************************************************/
    /*  fun:    对接受连接之后的客户ip 进行过滤和筛选
        remark: 先过滤, 在看是否需要筛选
        return: false-> 过滤掉
    /************************************************************************/
    bool            Filter(const CLIENT_ADDRESS&);


    /************************************************************************/
    /*  fun: 增加过滤元素
    /************************************************************************/
    void            AddFilter(const CLIENT_ADDRESS& caR);

    /************************************************************************/
    /*  fun: 取消之前设置过的过滤项
    /************************************************************************/
    bool            WithDrawFilter(const CLIENT_ADDRESS& caR);

    /************************************************************************/
    /*  fun: 增加筛选元素
    /************************************************************************/
    
    void            AddDisFilter(const CLIENT_ADDRESS& caR);

    /************************************************************************/
    /*  fun: 取消之前的筛选
    /************************************************************************/
    bool            WithDrawDisFilter(const CLIENT_ADDRESS& caR);
private:

	ClientInfoList*		    m_pVtClientsSock;
    list<CLIENT_ADDRESS>    m_lstFilterList;    //过滤列表
    list<CLIENT_ADDRESS>    m_lstDisFilterList; //筛选列表
    INIT_PARAM              m_iParam;
	//CRITICAL_SECTION	m_cs;
    CLock*              m_pLock;
    CLock*              m_pLockFilter;
    CLock*              m_pLockDisFilter;
};
