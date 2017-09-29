#include "stdafx.h"
#include "MsgPack.h"


MsgPack::MsgPack(void)
{
	memset(this,0, sizeof(*this));
}

MsgPack::MsgPack(const char *pStr, int nLen)
{
	memset(this,0, sizeof(*this));
	if (strlen(pStr)<MsgPackSIZE)
	{
		::CopyMemory(m_SendBuf, pStr, nLen);
		m_nlen = nLen;
	}
}

MsgPack::~MsgPack(void)
{
}

BOOL MsgPack::PushMemory(const void* pstMemory, int nSize, BOOL isStr)
{
	if (m_iPushIndex+100>=MsgPackSIZE)
	{
		return FALSE;
	}
	if (isStr)
	{
		::CopyMemory(&(m_SendBuf[m_iPushIndex]),&nSize, sizeof(short));
		m_iPushIndex += sizeof(short);
	}
	::CopyMemory(&(m_SendBuf[m_iPushIndex]),pstMemory, nSize);
	m_iPushIndex += nSize;
	return TRUE;
}

BOOL MsgPack::PushMemory(const byte& pstMemory )
{
	return PushMemory(&pstMemory, sizeof(byte), FALSE);
}

BOOL MsgPack::PushMemory(const USHORT& pstMemory )
{
	return PushMemory(&pstMemory, sizeof(USHORT), FALSE);
}

BOOL MsgPack::PushMemory(const UINT& pstMemory )
{
	return PushMemory(&pstMemory, sizeof(UINT), FALSE);

}

BOOL MsgPack::PushMemory(const INT& pstMemory )
{
	return PushMemory(&pstMemory, sizeof(INT), FALSE);

}

BOOL MsgPack::PushMemory(const bool& pstMemory )
{
	return PushMemory(&pstMemory, sizeof(bool), FALSE);

}


BOOL MsgPack::GetMemory(void* pstMemory, int nSize, BOOL isStr)
{
	if (m_iPushIndex>=m_nlen)
	{
		return TRUE;
	}
	if (isStr)
	{
		USHORT usLen = 0;
		GetMemory(&usLen, sizeof(USHORT));
		if (usLen>=nSize && nSize>=0)
		{
			return FALSE;
		}
		::CopyMemory(pstMemory,&(m_SendBuf[m_iPushIndex]), usLen);
		m_iPushIndex += usLen;
	}
	else
	{
		::CopyMemory(pstMemory, &(m_SendBuf[m_iPushIndex]), nSize);
		m_iPushIndex += nSize;
	}
	return TRUE;
}

BOOL MsgPack::GetMemory( USHORT* pstMemory )
{
	return GetMemory(pstMemory, sizeof(USHORT), FALSE);
}

BOOL MsgPack::GetMemory( UINT* pstMemory )
{
	return GetMemory(pstMemory, sizeof(UINT), FALSE);
}

BOOL MsgPack::GetMemory( byte* pstMemory )
{
	return GetMemory(pstMemory, sizeof(byte), FALSE);
}

BOOL MsgPack::GetMemory( bool* pstMemory )
{
	return GetMemory(pstMemory, sizeof(bool), FALSE);
}

BOOL MsgPack::GetMemory( int* pstMemory )
{
	return GetMemory(pstMemory, sizeof(int), FALSE);

}

BOOL MsgPack::SetMsgLen()
{
	short MsgLen =  m_iPushIndex;
	::CopyMemory(m_SendBuf, &MsgLen, sizeof(short));
	return TRUE;
}


__int64			MsgPack::GetMemory_64()
{
	__int64 V = 0;
	GetMemory(&V, sizeof(__int64));
	return V;
}
__int32			MsgPack::GetMemory_32()
{
	__int32 V = 0;
	GetMemory(&V, sizeof(__int32));
	return V;
}
__int16			MsgPack::GetMemory_16()
{
	__int16 V = 0;
	GetMemory(&V, sizeof(__int16));
	return V;
}
__int8			MsgPack::GetMemory_8()
{
	__int8 V = 0;
	GetMemory(&V, sizeof(__int8));
	return V;
}

void	MsgPack::PushMemory_64(__int64	mv)
{
	PushMemory(&mv,sizeof(__int64));
}
void	MsgPack::PushMemory_32(__int32	mv)
{
	PushMemory(&mv,sizeof(__int32));
}
void	MsgPack::PushMemory_16(__int16	mv)
{
	PushMemory(&mv,sizeof(__int16));
}
void	MsgPack::PushMemory_8(__int8	mv)
{
	PushMemory(&mv,sizeof(__int8));
}

void MsgPack::Clear()
{
	m_iPushIndex = 0;
}
