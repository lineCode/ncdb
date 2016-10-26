#include "StdAfx.h"
#include "PiDataSeries.h"
#include "AutoType.h"
#include "FileOpr.h"
#include "StrCoding.h"


CDataSeries::CDataSeries(void)
{
}


CDataSeries::~CDataSeries(void)
{
}

CDataSeries& CDataSeries::operator<<( tstring& strData )
{
    int nSize = strData.size(); //×Ö·û¸öÊý
    *this << nSize;
    m_pFile->Write(&w2sSmart(strData).at(0), strData.length());
    m_pFile->Flush();
    return *this;
}

CDataSeries& CDataSeries::operator<<( int& strData )
{
    m_pFile->Write(&strData, sizeof(int));
    m_pFile->Flush();
    return *this;
}

CDataSeries& CDataSeries::operator<<( UNINT& strData )
{
    return *this;
}

CDataSeries& CDataSeries::operator>>( tstring& strData )
{
    //CAutoType at;
    void* pBuf = NULL;
    int nSize = 0;
    *this >> nSize; //×Ö·û¸öÊý

    /*pBuf = m_pFile->Read(nSize);  
    at.SetData(tcpchar(pBuf));*/

    m_pFile->Read(nSize, pBuf);  //×Ö·û´®
    string strSrc((PCSTR)pBuf, nSize);
    //at.SetData(strSrc.c_str());
    strData = s2wSmart(strSrc);
    ClearMemArr(pBuf);

    return *this;
}

CDataSeries& CDataSeries::operator>>( int& strData )
{
    //CAutoType at;
    void* pBuf = NULL;
    m_pFile->Read(sizeof(int), pBuf);
    //at.SetData( tcpchar(pBuf));
    strData = *(int*)pBuf;
    ClearMemArr(pBuf);
    return *this;
}

CDataSeries& CDataSeries::operator>>( UNINT& strData )
{

    return *this;
}


bool CDataSeries::Open( tcpchar szDBPath )
{
    return m_pFile->Open(pw2psSmart(szDBPath), "wb");
}

bool CDataSeries::OpenRead( tcpchar szDBPath )
{
    return m_pFile->Open(pw2psSmart(szDBPath), "rb");
}

bool CDataSeries::Init()
{
    Create_Auto_Ptr(m_pFile, CFileOpr);
    m_pFile->Init();
    return true;

}

bool CDataSeries::UnInit()
{
    return true;

}
