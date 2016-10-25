#pragma once
#include "PiTypeDef.h"

class CMemBuf
{
public:
	CMemBuf(void);
	~CMemBuf(void);

public:
	/************************************************************************/
	/*	fun: 追加新的内存数据到类维护的缓冲区中, 如果内存不够,则会自动申请更大内存
		ret: 申请更大的内存空间后,原先的内存会被释放掉
	/************************************************************************/
	void AppendData(void* pData, UNLONG dwSize);
	void ReleaseMem();
	char*	GetBuffer();
	UNLONG	GetBufTotalSize();
protected:
	/************************************************************************/
	/*	fun: 申请更大的内存,并把原先的数据移动到新的内存中
		ret: 会自己释放掉原来的内存
	/************************************************************************/
	void	MoreToBigger();

private:
	UNLONG	m_bufTotalSize;//维护的缓冲区大小
	UNLONG	m_dataSize;//实际有存放数据的大小

};
