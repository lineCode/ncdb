#ifndef _PIPE_DATA_TYPE_SOURCE_H_
#define _PIPE_DATA_TYPE_SOURCE_H_

//*******************************************************
//作用:用于指定命名管道的管理的相关类型定义

//******************************************************

#ifdef TEST_PIPE
	#define NUM_PIPE_WRITE_TIMEOUT		INFINITE
#else
	#define NUM_PIPE_WRITE_TIMEOUT		2000		//写操作超时时间

#endif
#define NUM_PIPE_TIMEOUT_CONNECT		10 * 1000		//连接管道超时时间

typedef void (*m_pReadFun)(char *, int nSize);

//连续读取失败N次数后,退出工作线程
//const int g_nReadErrorCount = 3;
//一次读写最大字节数
//const int g_nReadDataMaxSize = MAX_PATH;


//创建 PIPE 类型
typedef	enum
{
	ENUM_PIPE_TYPE_SERVER = 0,				//服务端
	ENUM_PIPE_TYPE_CLIENT,					//客户端
} ENUM_CREATE_PIPE_TYPE;

//线程工作状态
typedef enum
{
	ENUM_THREAD_STATE_START = 0,			//开始
	ENUM_THREAD_STATE_WAIT,					//暂停
	ENUM_THREAD_STATE_STOP,					//停止
} ENUM_THREAD_STATE_TYPE;

enum ENUM_PIPE_DATA_TYPE
{
	dt_single = 0			,
	dt_struct
};
#endif //_PIPE_DATA_TYPE_SOURCE_H_
