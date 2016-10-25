#pragma once
#include "Sock.h"
#include <string>
#include <vector> 
#include <list>
#include "SocketSelect.h"
#include "Stlmath.h"
#include "socket_ns.h"
class IDataDeal;
using namespace std;
class CMainFrame;
class CSockClientManager;
class CLIENT_ADDRESS;
class CLock;
class CSockTcp;

namespace ns_sock_tcp
{
    enum err_code{
        err_connect_timeout             = 1,
        err_connect_last_no_completion  = 2,    //上次的连接还没完成, 可能返回10022
    };
}

struct Msg;
const UNINT     NUM_TCP_KEEPALIVE_TIMEOUT       = SecondToMillSecond(10);
//const 

class SERVER_PARAM
{
    UNINT nAcceptSleep;     //接受客户端后的延迟
};

class TCP_SERVER
{
    
};

class INIT_PARAM
{
public:
    bool    bServer;        //是否为服务端
    u_short uPort;          //指定要绑定到哪个端口
    bool    bAutoRecn;      //是否自动重连
    UNLONG  nCnTimeout;     //连接超时, 单位ms
    //server
    bool    bDisFilter;     //启用对客户端进行筛选
    bool    bKeepalive;
    UNINT   nKeepalivetime;
public:
    INIT_PARAM()
    {
        bServer         = false;
        uPort           = 0;
        bDisFilter      = false;
        bKeepalive      = true;
        nKeepalivetime  = NUM_TCP_KEEPALIVE_TIMEOUT;

        bAutoRecn       = true;
        nCnTimeout      = 80;
    }
};

struct THREAD_ADDITION_ARGU
{
	void*		m_Add1;
};

class CSockTcp 
	:public CSock
{
public:
    //接受客户的连接
    enum CONNECT_RESULT {CN_OK, 
                    CN_FILTER, 
                    CN_ERR_CLIENT,  //客户端的错误, 可以继续接受其他的
                    CN_ERROR    //大错误, 
    };
    
    

    enum ERROR_AREA {ERR_INIT_SOCKETLIB, ERR_CREATE_SOCKET, ERR_BIND};
	CSockTcp(SOCKET sock = 0, void* pAdd = 0, const SOCK_PARAM* pSP = NULL);
	//拷贝构造函数
	CSockTcp(const CSockTcp& tcp);
	~CSockTcp(void);
 
public:
    /************************************************************************/
    /*  fun:初始化服务端需要的一些参数, 绑定端口等操作
        remark: 先调用SetInitParam,在调用该接口
    /************************************************************************/
	bool	        Init();
	bool			UnInit();
    void            InitVariable();
    virtual bool    AcceptSleep();

    virtual bool    Bind(u_short port /*= 0*/);
    virtual bool    DealSendFail();    
    
    virtual         bool Close();

	
    

    /************************************************************************/
    /*  fun: 连接服务器
        remark:如果ip = "", 则服务器地址为之前设置的远程地址
    /************************************************************************/
	virtual bool	Connect(const char* ip = "", u_short port = 0);
    virtual void    DealReConnectFail();

    bool            SetInitParam(const INIT_PARAM& iParam);
    bool            SetNonBlockMode(bool bBlock);
    bool            IsConnect();
    /************************************************************************/
    /*  fun:判断是否可以进行重连
        remark: 根据错误返回值+本地连接状态标记+是否启用自动重连来决定是否可以重连
    /************************************************************************/
    bool            CanRecn();      
    bool            ReConnect();
    
    static UNINT            ReConnectSome(list<CSockTcp*>& );

	bool			WaitConnect();// create thread for wait connect 
	bool			Listen(DWORD backlog );
	bool			SetMaxCnCount(DWORD dwCount);

    //接受数据大小,在接受具体数据
    virtual RECV_STATE	Recv(char* &pBuf);
    virtual CSock*  Clone();
	virtual bool	Listen();
	//接收到客户连接之后， 做些初始化
	virtual bool	InitAdditionArgu();
    virtual CONNECT_RESULT	Accept(const char* ip = 0, u_short port = 0);
    virtual bool    DealAcceptError();
    virtual bool    DealAcceptClientError();
    /************************************************************************/
    /*  fun:select sock 函数失败时, 调用该接口
    /************************************************************************/
    virtual bool    DealSelectError(int nErr);

    virtual bool    DealCloseByClient();

	virtual bool	ExitWaitForConnectThread(void* cInfo = NULL);
	virtual bool	ExitRecvMsgThread(void* cInfo = NULL);

    /************************************************************************/
    /*  fun: 当有连接被过滤后,调用该接口
    /************************************************************************/
    virtual bool    DealFilter(const string&, u_short uPort);
    /************************************************************************/
    /*  fun: 增加过滤和筛选客户的接口
    /************************************************************************/
    void            AddFilter(const CLIENT_ADDRESS*     pCaR);
    void            AddDisFilter(const CLIENT_ADDRESS*  pCaR);
    
    
	virtual SEND_STATE	Send(const char* data, u_long lSize);
	virtual SEND_STATE	SendDataSize(DWORD dwSize);
	SEND_STATE		SendData(const char* data, DWORD dwSize);

	bool			SetConfig();//从ini配置文件中设置参数
    
    bool            StartListen();
    
    
	
	DWORD			RecvData(const char* pBuf, u_long lSize);
	DWORD           RecvDataOneTime(char* pBuf, u_long lMaxSize);
	virtual DWORD	RecvDataSize();

	bool			SetRemoteInfo(const char* ip, u_short port);

	string			GetClientIp(SOCKET sock);
    string          GetClientIp();

    string          GetServerIp();
    u_short         GetServerPort();

	u_short			GetClientPort(SOCKET sock);
	void			SetDeal(IDataDeal* pDeal);

	//子类来实现, 对接受到的数据进行处理
	bool    DoDealDataRecv(void* pBuf, DWORD dwRecv);
	
    void            SetDataLock(CLock* pLock = NULL);
    CSockClientManager*     GetClientManager();
    bool            IsStopListen();
    bool            IsStopClientRecv();//客户端线程中判断,是否要停止接收, 退出线程

protected:

public:
    CSockClientManager*     m_pSockClientManager;
	IDataDeal*			    m_pDataDeal;//数据处理对象

protected:
    UNLONG                  m_LastRecvSize;//保存上次接收到的数据大小
private:

    /************************************************************************/
    /* param for client
    /************************************************************************/
    bool                    m_bCnServer;        //是否连接上服务器
    bool                    m_bNeedRebind;      //重连时是否需要重新绑定端口
    /************************************************************************/
    /* param for server
    /************************************************************************/

	//bool					m_bCn;
    bool                    m_bStopListen;
    bool                    m_bStopClientRecv;
    INIT_PARAM              m_iParam;
	HANDLE					m_hListenThread;
    UNINT                   m_nSendBufSize;     //发送缓冲区大小
    UNINT                   m_nRecvBufSize;     //接受缓冲区大小

    string                  m_strClientIp;          //客户端Ip
    u_short                 m_uClientPort;          //客户端端口

	string					m_strServerIp;          //服务端Ip
	u_short					m_uServerPort;          //服务端端口

	string					m_strLastClientIp;
	u_short					m_uLastClientPort;
    SOCKET                  m_sockLastRecv;

	DWORD					m_dwMaxCnCount; //最大在线客户端人数
	CLock*                  m_pDataLock;    //如果是服务端, 则必须设置该指针, 用来控制数据的线程互斥
	THREAD_ADDITION_ARGU*	m_pThreadArgu;
};
