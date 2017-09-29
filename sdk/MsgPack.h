#pragma once
#include <string>
using namespace std;


typedef short _short;
typedef unsigned short _ushort;
typedef int _int;
typedef unsigned int _uint;
typedef long _long;
typedef unsigned long _ulong;
typedef char _byte;
typedef unsigned char _ubyte;

#define MsgPackSIZE 1024*20

class MsgPack
{
public:
	MsgPack(void);
	MsgPack(const char *pStr, int len);
	~MsgPack(void);

public:
	//			isStr 0非字符串  1字符串
	BOOL		PushMemory(const void* pstMemory, int nSize, BOOL isStr=0);
	BOOL		PushMemory(const bool& pstMemory);
	BOOL		PushMemory(const byte& pstMemory);
	BOOL		PushMemory(const USHORT& pstMemory);
	BOOL		PushMemory(const UINT& pstMemory);
	BOOL		PushMemory(const INT& pstMemory);

	BOOL		GetMemory(void* pstMemory, int nSize, BOOL isStr=0);
	BOOL		GetMemory(USHORT* pstMemory);
	BOOL		GetMemory(UINT* pstMemory);
	BOOL		GetMemory(byte* pstMemory);
	BOOL		GetMemory( int* pstMemory );
	BOOL		GetMemory( bool* pstMemory );
	BOOL		SetMsgLen();

	__int64			GetMemory_64();
	__int32			GetMemory_32();
	__int16			GetMemory_16();
	__int8			GetMemory_8();


	void	PushMemory_64(__int64	mv);
	void	PushMemory_32(__int32	mv);
	void	PushMemory_16(__int16	mv);
	void	PushMemory_8(__int8	mv);
	void Clear();
public:
	char		m_SendBuf[MsgPackSIZE];
	_int		m_iPushIndex;	//当前偏移量
	int			m_nlen;			//出栈时，初始化字符串的长度
};

