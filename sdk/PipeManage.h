#pragma once
#ifndef _PIPE_MANAGE_H_
#define _PIPE_MANAGE_H_

#include "PipeDataTypeSource.h"
#include "Lock.h"
#include "PiTypeDef.h"
#include "threadlight.h"
/*******************************************************
作用:	本类用于命名管道的管理, 创建管道\连接管道,并且对管道数据的读写
描述:   内部创建线程来读写管道

使用说明:
   实现接口
   DealReadData, 获取到数据协议后调用该接口, 内部根据协议类型获取数据
   DealReadStruct, 获取到结构体协议后调用该接口, 根据协议类型获取数据, 可结合内置的Transfer接口封装读写一体化接口
   DoReadFail, 读取失败时处理

整套流程:
    发送数据类型(单个数据或结构体) -> 发送协议, 发送数据

    接收数据类型 -> 读取协议-> 根据协议读取协定数据
******************************************************/

class CPipeManage:public CThreadLight
{
public:
	typedef UNINT	DataSize;
	enum ENUM_TRANSFER_TYPE {type_read = 0, type_write};
    CPipeManage();
    virtual ~CPipeManage();
public:



protected:
/************************************************************************
    fun:    管道接受到数据后调用该接口, 
    param: 
    remark: 数据保存在m_pBuf, 大小为m_nReadSize
************************************************************************/
	virtual bool DealReadData();
	/*------------------------------------------------------------
			fun:	读取数据失败时会调用， 可调用IsCloseBySelf判断是否为另一端关闭了管道
			ret:	
			param: 
			remark:	
	------------------------------------------------------------*/
	virtual bool DoReadFail();
	virtual bool DealReadStruct();
public:
	virtual DWORD DoWork();

/************************************************************************
    fun:    管道读取数据失败时调用该接口, 
    param: 
    remark: 数据保存在m_pBuf, 大小为m_nReadSize
************************************************************************/
	void ReadFail();
public:
    bool Init();
    bool UnInit();

	//创建/打开指定类型的PIPE
	bool CreatePipe(	LPCTSTR lpPipeName = _T("\\\\.\\pipe\\mypipe"),			//strPipeName 指定的Pipe名字
						ENUM_CREATE_PIPE_TYPE enumCreateType = ENUM_PIPE_TYPE_CLIENT,	//指定创建是类型,服务端/客户端
						void(*ptr)(char *, int ) = NULL									//处理管道的数据的函数指针
						);
	//关闭管道
    bool ClosePipe();
	/*------------------------------------------------------------
			fun:	判断管道是否为自己这端主动关闭掉的
			ret:	
			param: 
			remark:	
	------------------------------------------------------------*/
	bool IsCloseBySelf();
	bool EndPipe(void);
	bool DealReadFail();

	//向管道写数据
	//char * pBuf 数据指针  nBufSize 数据大小
    bool Write(void * pBuf, int nBufSize, bool bWriteSize = true);
	bool Write(int nValue, bool bWriteSize = true);
	bool Write(UNINT nValue, bool bWriteSize = true);
	bool Write(LLONG nValue, bool bWriteSize = true);
	bool Write(bool nValue, bool bWriteSize = true);
	bool Write(tcpchar nValue);
	bool Write(StringCRef nValue);

	bool Transfer(int& nValue, ENUM_TRANSFER_TYPE enType);
	bool Transfer(UNINT& nValue, ENUM_TRANSFER_TYPE enType);
	bool Transfer(LLONG& nValue, ENUM_TRANSFER_TYPE enType);
	bool Transfer(bool& nValue, ENUM_TRANSFER_TYPE enType);
	bool Transfer(tstring& nValue, ENUM_TRANSFER_TYPE enType);

	bool WriteSingleBegin();
	bool WriteSingleEnd();
	bool WriteSingleData( int nValue );

    bool IsValidHandle();

	//向管道写数据
	//int nData 整形数据
	bool WriteStr(int nData);

	//判定是否已经创建/连接PIPE
    bool IsCreatePipe();
    bool WaitConnect();
	//获取当前对象PIPE类型
    ENUM_CREATE_PIPE_TYPE GetCurPipeType();

	bool WriteStructBegin();
	bool WriteStructBegin(ENUM_TRANSFER_TYPE enMode);
	bool WriteStructEnd();
	bool WriteStructEnd(ENUM_TRANSFER_TYPE enMode);

protected:
	//创建服务端
	bool CreatePipeServer(LPCTSTR lpPipeName);
	//创建客户端
	bool CreatePipeClient(LPCTSTR lpPipeName);

    bool ConnectPipe( LPCTSTR lpPipeName );

/************************************************************************
    fun:    读取管道数据, 如果没数据则会堵塞
    param: 
    remark: 读取成功返回true, 数据保存在m_pBuf, 大小为m_nReadSize
************************************************************************/
	bool ReadPipe(void);
	bool ReadPipeCatch(void);
	bool ReadPipe(bool& nValue);
	bool ReadPipe(int& nValue);
	bool ReadPipe(UNINT& nValue);
	bool ReadPipe(LLONG& nValue);
	bool ReadPipe(tstring& nValue);

    bool IsReadErr();

    bool CreateWorkThread(void);
	//	static DWORD WINAPI RunProc(LPVOID lpParam);
	//void Porc(void);
protected:
	void (*m_pReadFun)(char *, int nSize);
    //另一端结束了管道
    bool DealPipeEnd();
    bool ReWaitConnect();
    bool IsClose();

	bool ReadDataType( ENUM_PIPE_DATA_TYPE& dType );
/************************************************************************
    fun:    客户端连接成功后调用该接口
    param: 
    remark: 
************************************************************************/
	virtual bool DoClientConnectOk();
	bool TransferSP( POINT * param1, ENUM_TRANSFER_TYPE enType);
	HANDLE m_hPipe;
	//HANDLE m_hThread;
	ENUM_CREATE_PIPE_TYPE m_enumPipeType;
	ENUM_THREAD_STATE_TYPE m_enumThreadState;
    void*                   m_pBuf;			//接收数据缓存
    DWORD                   m_nReadSize;	//接收到的数据大小, 字节数

    HANDLE                  m_hEvent;
    HANDLE                  m_hEventRead;
    HANDLE                  m_hEventWrite;
    CLock                   m_lockWrite;
    tstring                 m_strPipeName;
    bool                    m_bEnd;
	bool					m_bStruct;		//标记当前发送的是否是结构体
	bool					m_bConnectOk;
	bool					m_bCloseActive;	//是否是主动关闭的
};

#endif//_PIPE_MANAGE_H_
