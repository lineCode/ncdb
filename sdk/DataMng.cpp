#include "StdAfx.h"
#include "DataMng.h"
#include "ResCommon.h"



CDataMng::CDataMng(void)
{
    InitVariable();
}


CDataMng::~CDataMng(void)
{
    ClearData();
}

bool CDataMng::Append( void* pMem, UNLONG nDataSize )
{
    UNLONG nNewSize  = m_nDataSizeTotal + nDataSize;
    NewMemAndInit(pBufBig, nNewSize);
    memcpy_s(pBufBig, nNewSize, m_pData, m_nDataSizeTotal); //把原来的数据dataSrc拷贝到新的大区域
    UNLONG nFree = nNewSize - m_nDataSizeTotal;   
    memcpy_s( reinterpret_cast<char*>(pBufBig) + m_nDataSizeTotal, nFree, pMem, nDataSize); //把新的数据追加到大区域里dataSrc后面

    ClearMemArr(m_pData);
    //跟踪数据信息
    m_pData = pBufBig;
    m_nDataSizeTotal = nNewSize;
    return true;
}

bool CDataMng::ClearData()
{
    ClearMemArr(m_pData);
    m_nDataSizeTotal = 0;
    return true;
}

void* CDataMng::GetData()
{
    return m_pData;
}

UNLONG CDataMng::GetDataSize()
{
    return m_nDataSizeTotal;
}

void CDataMng::InitVariable()
{
    m_pData             = NULL;
    m_nDataSizeTotal    = 0;
}

CDataMng::operator void*()
{
	return m_pData;
}

CDataMng::operator tpchar()
{
	return (tpchar)m_pData;
}

void* CDataMng::Alloc( UNINT dwSize )
{
	ClearData();
	m_pData = new byte[dwSize];
	return m_pData;
}
