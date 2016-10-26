#pragma once
#include <WinSock2.h>
#include <string>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
class CMainFrame;

struct Msg;
const byte DATA_SIZE_DIGIT_NUMBER			= 30;//字符串形式的数据大小最大支持的位数

struct SOCK_INFO
{ 
    int sockType;
};

//设置socket 选项结构体
class SOCK_PARAM
{
public:
    bool bEnableSendTimeout;
    bool bEnableRecvTimeout;
    bool bSetSendBuf;

    int nSendTimeout;       
    int nRecvTimeout;
    int nSendBuf;
    SOCK_PARAM()
    {
        bEnableSendTimeout  = false;
        bEnableRecvTimeout  = false;
        bSetSendBuf         = false;
        nSendTimeout        = 0;
        nRecvTimeout        = 0;
        nSendBuf            = 10 * 1024;
    }
};

//构造的传入的参数
class SOCK_CST_PARAM
{
public:
    SOCKET sock;
    void* pAdd;
    SOCK_PARAM* pSP ;

    SOCK_CST_PARAM()
    {
        sock            = NULL;
        void* pAdd      = 0;
        SOCK_PARAM* pSP = NULL;
    }
};

class CSock
{
public:
	CSock(SOCKET sock = 0, void* pAdd = 0, const SOCK_PARAM* pSP = NULL);
	//拷贝构造函数
	CSock(const CSock& sock);
	virtual ~CSock(void);

public:
    //接受数据的错误类型
    enum RECV_STATE {RECV_ER_NET, RECV_ER_DATA, RECV_ER_NOCARE, RECV_OK, RECV_ER_UNKNOWN};
    enum SEND_STATE {
        SEND_OK,            //正常
        SEND_ER_CLIENT,     //该包数据的错误, 不会影响到后面的所有send操作
        SEND_ER_CONNECT,    // 连接问题的连续性错误, 会影响到后面基于该连接的所有send操作

    };

	bool		    Init();
	static bool		InitSockLib(byte majorVer, byte minorVer);
	bool		    Bind(u_short uPort = 0);
    virtual void    DealSetSockOptFail();

	SOCKET		    GetSock();
	const char*     GetLocalIp();
	u_short		    GetLocalPort();
    SOCK_INFO       SetSockInfo(const SOCK_INFO& si);
	
	virtual         bool Close();

	//bool	Send(Msg*);
    virtual SEND_STATE	Send(const char* data, u_long lSize) = 0;
	//virtual bool	SendDataSize(DWORD dwSize) = 0;

	//接受数据,具体让子类来实现, 传入指针,返回后,指针指向内部分配的数据
	virtual RECV_STATE	Recv(char* &pBuf) = 0;
	//virtual DWORD	RecvDataSize() = 0;
    virtual bool    DoDealDataRecv(void* pBuf, DWORD dwRecv) = 0;

	//子类来实现, 对接受到的数据进行处理
	virtual CSock*  Clone() = 0;


    void            SetErrDesc(string strErr);
    const string&   GetErrDesc();

	void            SetErrCode(int nErrCode);
    int             GetErrCode();
    
protected:
    void		    SetSock(SOCKET sock);
public:
	/*一个附加指针,通常用来保存重要的数据,如 mfc中主窗口指针在多线程中获取不到
		指针数据在外外部释放
	*/
	void*			m_pAdd; 
private:
    SOCK_PARAM      m_sockParam;        //socket 设置socket选项

	SOCKET			m_sock;             //关联的socket
	string			m_strLocalIp;       //本地Ip, 暂不支持双网卡

	u_short			m_port;             //本地绑定的端口
    string          m_strErrDesc;
	int             m_nErrCode;         //保存错误代码
    SOCK_INFO       m_sockInfo;
};
