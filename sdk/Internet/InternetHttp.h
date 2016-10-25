#pragma once
#include <WinInet.h>
#include <string>
#include "PiTypeDef.h"
#include "InetDef.h"
#include "DowningInfoMng.h"
using std::string;
#define ClearHInternet(a) ::InternetCloseHandle(a); (a) = NULL
#define SaveErrCodeAndRet(a) if(!(a)){ m_dwErrCode = GetLastError(); return false;}
#define SaveErrCodeAndRetEx(nFlag, err) if(!(nFlag)){ m_dwErrCode = GetLastError(); m_dState = err; return false;}

#define SaveNetErrCodeAndRet(a) if(!(a)){ m_dwErrCode = GetLastError(); m_dState = err_net; return false;}

typedef bool (FuncReadData)(void* pObj, void* pData, UNINT nSize);
class CPiBPDown;
class CInternetHttp
{
public:
	CInternetHttp(void);
	virtual ~CInternetHttp(void);
protected:
	virtual bool DealReadData(void* pData, UNINT nSize);

public:
	/************************************************************************
		function：	下载指定网址的数据到本地文件
		remark:		目前只支持http协议
	/************************************************************************/
	bool DownloadFile(tcpchar strUrl);
	/************************************************************************
		function：	转换类型， 从INTERNET_SCHEME 到 INTERNET_SERVICE
		remark:		
	/************************************************************************/
	static int GetServiceType( INTERNET_SCHEME param1 );

	/************************************************************************
		function：	获取url的object部分
		remark:		
	/************************************************************************/
    static tstring FindHttpObject(tcpchar szUrl);

	/************************************************************************
		function：	下载文件， 针对下载同一个网站下的多个文件
		remark:		
		param:		相对主机名的文件下载地址， 如 整个下载地址为http://zhidao.baidu.com/question/189984289.html, 
					则传入"/question/189984289.html"
	/************************************************************************/
    bool DownloadObject(tcpchar szUrl, tcpchar szSavePath);
	bool DownloadObject( tcpchar szUrl);
	
    bool SetOffset(UNLONG nOffset);
	static bool QueryFileSize(tcpchar szUrl, UNLONG& dwSize);
    UNLONG GetFileSize() const;
    void PauseDown();

    /************************************************************************
        fun:	停止下载, 让下载函数退出
        param:  
        ret:    
        remark: 
	/************************************************************************/
    void QuitDown();
    void ResumeDown();
	bool QueryFileSize(UNLONG& dwSize);

    bool SendCn();

    tstring QueryFileModifyTime();
	bool EnsureQueryOK(HINTERNET hq);

    /************************************************************************
		function：	下载之前的初始化
		remark:		只在需要调用DownloadObject才需要调用该接口
		param:		网站地址, 不需要详细到下载页面
	/************************************************************************/
    bool DownloadInit( tcpchar strUrl, tcpchar szProxyServe = NULL);
    HINTERNET GetConnectHandle();

    DownState		GetErrType();
	UNLONG	GetServerState(){return m_dwServerState; }
	/************************************************************************
		function：	下载多个文件完成后, 调用该接口， 释放资源
		remark:		
	/************************************************************************/
	bool DownloadUnInit();
	
    void SetDowningInfoMng(CDowningInfoMng* pR);
    void SetDowningInfoID(ItemID id);
    UNLONG ErrCode() const { return m_dwErrCode; }
    void ErrCode(UNLONG val) { m_dwErrCode = val; }
    bool ClearState();
    bool SetHeader( LPCTSTR param1 );
    ItemID GetShowID() { return m_idDowningInfo;}
    bool AddHeaderOffset( UNLONG m_nOffset );
    static tstring QuestFileModifyTime( tcpchar szUrl);
    tstring Url() const { return m_strUrl; }
    void Url(tstring val) { m_strUrl = val; }
    void BPDown( CPiBPDown* param1 );
    static void Clear(CInternetHttp* pInet);
    UNLONG GetDownSize() { return m_dwDowned; }
    void UpdateData(UNLONG nAddSize, bool bClearSpeed = false);
    bool CanRetryCn();
    bool IsRetryOK();
	void SetHttpsWay();
	static tstring GetCookie( const tstring& strUrl );
	void SetCookie( tcpchar szCookie );
	void SetNoRedirect();
	bool IsNeedRedirect();
	bool CheckSetHttpWay();
	static bool DeleteUrlCache(const tstring& strUrl);
	void SetDownWeb() ;
private:
	void InitVariable();
    bool RetryOut();
	static tstring GetHostName( tcpchar strUrl);
protected:
	tstring         m_strUrl;		//URL路径
    
    bool            m_bQuitDown;        //是否停止下载
    
	HINTERNET m_hInet;		//已打开的网络句柄
	HINTERNET m_hCn;			//已连接的网络句柄
    HINTERNET m_hQeq;       //可进行操作的句柄

	int m_nServiceType;		//要连接的服务器的类型(http, ftp等)
	URL_COMPONENTS m_uc;		//url解析出来的数据
    UNLONG  m_dwDowned;         //已下载字节数
    UNLONG  m_dwSizeTotalSrc;      //文件总大小

	UNLONG	m_dwFlag;			//请求服务器时附加标识
	UNLONG	m_dwErrCode;	//保存最新的错误码
    
    UNINT	m_dwServerState;	// 服务器的状态码
	DownState	m_dState;
    
    CDowningInfoMng*        m_pDowningInfoMng;
    ItemID              m_idDowningInfo;
    int                 m_nRetryedCount;
    int                 m_nCanRetryCountMax;
    tstring             m_strHeader;
    tcpchar             m_szHeader;
    CPiBPDown*            m_pBPDown;
};

