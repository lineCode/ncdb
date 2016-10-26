#include "StdAfx.h"
#include "PiBPDown.h"
#include <afxinet.h>
#include "ResCommon.h"
#include "functional.h"
#include "FileOpr.h"
#include "InternetHttp.h"
#include <assert.h>
#include "AutoType.h"
#include "PathLight.h"
#define CharSize2MemSize(nCharCount) ((nCharCount) * sizeof(TCHAR))
#define MemSize2CharSize(nCount) ((nCount) / sizeof(TCHAR))

/*
    bool    ,   续传标识
    int     ,   url大小
    char[]  ,   url字符串, 
    int     ,   最新修改时间大小
    char[]  ,   最新修改时间字符串
*/

CPiBPDown::CPiBPDown( CInternetHttp* pInet, bool bNeedBP /*= false*/ )
{
    m_pInet = pInet;
    m_bBPTrans = bNeedBP;
    m_pInet->BPDown(this);
    m_pFile = nullptr;
    m_nOffset = 0;
}


CPiBPDown::~CPiBPDown(void)
{
    m_pInet->BPDown(NULL);
}


bool CPiBPDown::WriteBPInfo()
{
    if ( ! m_bBPTrans)
    {
        return true;
    }
    //获取url和上次修改时间
    tagFILE_BP_INFO bpInfo;
    bpInfo.strLastModify = m_pInet->QueryFileModifyTime();
    bpInfo.strUrl = m_pInet->Url();
    //写入到文件
    

    //判断是否支持断点下载, 如果没有, 写入不支持标识
    //支持标识, url和最后修改时间
    int nSize = 0;
    int nSizeTime = 0;
    bool bSuppose = true;
    DWORD nWrited = 0;

    nSize = bpInfo.strUrl.length();
    nSizeTime = bpInfo.strLastModify.length();

    bSuppose = IsSupposeBPDown(bpInfo.strUrl);
    if (bpInfo.strLastModify.empty())
    {
        bSuppose = false;
    }
    tstring strMsg(_T("is file bpSuppose:"));
    CAutoType at;
    strMsg += (tcpchar)at.SetData(bSuppose);
    strMsg += _T(" ") + bpInfo.strUrl;
    OutInfo(strMsg.c_str());

    m_pFile->Write(&bSuppose, sizeof(bool));
    //WriteFile(hFile, &bSuppose, sizeof(bool), &nWrited, NULL);
    if(!bSuppose)
    {
        return true;
    }

    if(
        m_pFile->Write(&nSize, sizeof(nSize))
        //WriteFile(hFile, &nSize, sizeof(nSize), &nWrited, NULL)  //url大小
        && m_pFile->Write(bpInfo.strUrl.c_str(), bpInfo.strUrl.length() * sizeof(TCHAR))
        //&& WriteFile(hFile, bpInfo.strUrl.c_str(), bpInfo.strUrl.length() * sizeof(TCHAR), &nWrited, NULL)  //url大小
        && m_pFile->Write(&nSizeTime, sizeof(nSizeTime))
        //&& WriteFile(hFile, &nSizeTime, sizeof(nSizeTime), &nWrited, NULL)  //url大小
        && m_pFile->Write(bpInfo.strLastModify.c_str(), bpInfo.strLastModify.length()* sizeof(TCHAR))
        //&& WriteFile(hFile, bpInfo.strLastModify.c_str(), bpInfo.strLastModify.length()* sizeof(TCHAR), &nWrited, NULL)  //url大小
        )
    {
        m_pFile->Flush();
        return true;

    }
    return false;
}


int CPiBPDown::GetBPInfoLen( const tstring& szPath )
{
    //TODO:异常处理, 文件数据太少

    int nLenTotal = 0;
    int nSize = 0;
    bool bSuppose = false;
    CFile file;

    if(!file.Open(szPath.c_str(), CFile::modeRead | CFile::shareDenyWrite))
    {
        return nLenTotal;
    }
    NewMemAndInit(cBuf, 500);
    nSize = 1;
    file.Read(&bSuppose, nSize);     //是否可续传标识
    nLenTotal += nSize;
    if (!bSuppose)
    {
        return nLenTotal;
    }


    nSize = sizeof(int);
    nLenTotal += nSize;
    file.Read(&nSize, nSize);

    nSize = CharSize2MemSize(nSize);
    nLenTotal += nSize;
    file.Read(cBuf, nSize);

    nSize = sizeof(int);
    nLenTotal += nSize;
    file.Read(&nSize, nSize);

    nSize = CharSize2MemSize(nSize);
    nLenTotal += nSize;
    file.Read(cBuf, nSize);

    return nLenTotal;
}

/*

bool CPiBPDown::GetDownedSize( StringCRef strUrl, const tstring& strPath, UNLONG& dwDownedSize )
{
    if (!m_bBPTrans)
    {
        return true;
    }

    m_nOffset = dwDownedSize = 0;
    UINT nLen = 0;
    UINT nSize = 0;
    const int nCharSize = sizeof(TCHAR);
    CString strBuf;
    CString strUrlLocal;
    CString strModifyTime;
    int nBufLenMax = 1000;
    bool bSuppose = false;
    bool bDelete = true;

    FOR_COUNT(1, i)
    {
        if ( ! CFileUtil::IsFileExist(strPath.c_str()))
        {
            break;
        }
        / *根据偏移量获取当前本地已下载大小* /
        CFile file(strPath.c_str(), CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary);

        //TODO:如果支持断点下载
        //...
        file.Read(&bSuppose, sizeof(bSuppose));
        if ( ! bSuppose)
        {
            break;
        }

        file.Read(&nLen, sizeof(nLen));
        nSize = nLen * nCharSize;
        if (nSize > nBufLenMax)
        {
            break;
        }
        file.Read(strBuf.GetBuffer(nSize + 1), nSize);
        strBuf.ReleaseBuffer(nLen);

        strUrlLocal = strBuf;
        if (strUrlLocal != strUrl.c_str())
        {
            //不是同一个文件
            break;
        }
        file.Read(&nLen, sizeof(nLen));
        nSize = nLen * nCharSize;
        if (nSize > nBufLenMax)
        {
            break;
        }
        file.Read(strBuf.GetBuffer(nSize + 1), nSize);
        strBuf.ReleaseBuffer(nLen);
        strModifyTime = strBuf;
        if(strModifyTime != CPiBPDown::GetUrlFileModifyTime(strUrl))
        {
            break;
        }
        int nBPLen = (1 + 4 * 2 + strUrlLocal.GetLength() * nCharSize 
            + strModifyTime.GetLength() * nCharSize);
        m_nOffset = dwDownedSize = file.GetLength() - nBPLen/ *文件总大小 - 断点数据大小* /;
        OutInfo(_T("bpDowned:"), dwDownedSize);
        bDelete = false;
        
    }
    if (bDelete)
    {
        CFileUtil::DeleteFile(TO_CSTRING(strPath));
    }
    return true;
}
*/

bool CPiBPDown::CalcDownedSize( const tstring& strPath )
{
    //return GetDownedSize(, strPath, m_nOffset);
    if ( ! m_bBPTrans)
    {
        return false;
    }

    tstring strUrl = m_pInet->Url();
    UNLONG dwDownedSize = m_nOffset;

    m_nOffset = dwDownedSize = 0;
    UINT nLen = 0;
    UINT nSize = 0;
    const int nCharSize = sizeof(TCHAR);
    CString strBuf;
    CString strUrlLocal;
    CString strModifyTime;
    UINT nBufLenMax = 1000;
    bool bSuppose = false;
    bool bDelete = true;

    FOR_COUNT(1, i)
    {
        if ( ! CPathLight::IsFileExist(strPath.c_str() ))
        {
            break;
        }
        /*根据偏移量获取当前本地已下载大小*/
        CFile file(strPath.c_str(), CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary);

        //TODO:如果支持断点下载
        //...
        file.Read(&bSuppose, sizeof(bSuppose));
        if ( ! bSuppose)
        {
            break;
        }

        file.Read(&nLen, sizeof(nLen));
        nSize = nLen * nCharSize;
        if (nSize > nBufLenMax)
        {
            break;
        }
        file.Read(strBuf.GetBuffer(nSize + 1), nSize);
        strBuf.ReleaseBuffer(nLen);

        strUrlLocal = strBuf;
        if (strUrlLocal != strUrl.c_str())
        {
            //不是同一个文件
            break;
        }
        file.Read(&nLen, sizeof(nLen));
        nSize = nLen * nCharSize;
        if (nSize > nBufLenMax)
        {
            break;
        }
        file.Read(strBuf.GetBuffer(nSize + 1), nSize);
        strBuf.ReleaseBuffer(nLen);
        strModifyTime = strBuf;
        if(strModifyTime != m_pInet->QueryFileModifyTime().c_str())
        {
            break;
        }
        int nBPLen = (1 + 4 * 2 + strUrlLocal.GetLength() * nCharSize 
            + strModifyTime.GetLength() * nCharSize);
        m_nOffset = dwDownedSize = (UNLONG)(file.GetLength() - nBPLen)/*文件总大小 - 断点数据大小*/;
        OutInfo(_T("bpDowned:"), dwDownedSize);
        bDelete = false;

    }
    OutInfo(_T("inet, get bpPos:"), dwDownedSize);
    if (bDelete)
    {
        CFileOpr::DeleteFile(strPath.c_str());
    }
    return true;
}

bool CPiBPDown::IsSupposeBPDown( const tstring& strUrl )
{
    bool bSuppose = true;
    
    CInternetHttp http;
    CPiBPDown bp(&http, true);
    bp.SetDataOffset(1);
    http.DownloadInit(strUrl.c_str());
    if( ! http.SendCn() )
    {
        //TODO:网络问题, 而不是不支持断点
        //return false;
    }
    //bSuppose = http.AddHeaderOffset(0);
    http.DownloadUnInit();
    return bSuppose;

    /*CInternetSession	ois(NULL, 1, PRE_CONFIG_INTERNET_ACCESS, NULL, NULL, INTERNET_FLAG_RELOAD);
    CHttpConnection*	pHttpConnect = NULL;
    CHttpFile*			pHttpFile = NULL;
    CString				strServer;
    CString				strObject;
    INTERNET_PORT		wPort;
    DWORD				dwType;
    BOOL				isInterrupt = FALSE;
    CString             strTime;
    DWORD       dwTimeOut = 5000;
    ois.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,dwTimeOut);
    ois.SetOption(INTERNET_OPTION_SEND_TIMEOUT  ,dwTimeOut);
    ois.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT,dwTimeOut);
    ois.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,dwTimeOut);
    ois.SetOption(INTERNET_OPTION_CONNECT_RETRIES,3);

    try
    {
        if(!AfxParseURL(strUrl.c_str(),dwType,strServer,strObject,wPort))
            return bSuppose;

        pHttpConnect = ois.GetHttpConnection(strServer ,wPort);
        if(NULL == pHttpConnect){
            return bSuppose;
        }

        pHttpFile = pHttpConnect->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject, NULL, 1, 0, 0, INTERNET_FLAG_RELOAD);
        if(NULL == pHttpFile){
            return bSuppose;
        }

        if(!pHttpFile->SendRequest()){
            return bSuppose;
        }

        DWORD dwStateCode;
        pHttpFile->QueryInfoStatusCode(dwStateCode);
        if(dwStateCode != HTTP_STATUS_OK)
        {
            return bSuppose;
        }

        pHttpFile->Seek(0, CFile::begin); //如果不支持续传, 抛出异常 CInternetException&
        bSuppose = true;
    }
    catch(...)
    {
        OutInfo(_T("不支持断点续传:"), strUrl.c_str());
    }
    if(pHttpFile != NULL)
    {
        pHttpFile->Close();
        delete pHttpFile;
        pHttpFile = NULL;
    }
    if(pHttpConnect != NULL)
    {
        pHttpConnect->Close();
        delete pHttpConnect;
        pHttpConnect = NULL;
    }
    ois.Close();
    return bSuppose;*/
}

bool CPiBPDown::SetOffsetLocal()
{
    if ( ! m_bBPTrans)
    {
        return true;
    }
    if (m_nOffset)
    {
        return MoveLocalFilePos();
    }
    else
    {
        return WriteBPInfo();
    }
    return false;
}

bool CPiBPDown::SetDataOffset( UNLONG nOffset )
{
    m_nOffset = nOffset;
    return true;
}

bool CPiBPDown::ClearBPInfo( const tstring& strSavePath )
{
    if ( ! m_bBPTrans )
    {
        return true;
    }
    int nLen = CPiBPDown::GetBPInfoLen(strSavePath);
    return CFileOpr::RemoveData(strSavePath.c_str(), nLen, pos_begin);
}

void CPiBPDown::SetIsBpDown( bool bNeedBP )
{
    m_bBPTrans = bNeedBP;
}

void CPiBPDown::SetFile( CFileOpr* file )
{
    m_pFile = file;
}

UNLONG CPiBPDown::GetDownedSize()
{
    return m_nOffset;
}

bool CPiBPDown::MoveLocalFilePos()
{
    if ( ! m_bBPTrans)
    {
        return true;
    }
    if (m_nOffset)
    {
        m_pFile->SetPos(0, SEEK_END);
    }
    return true;
}

bool CPiBPDown::MoveInetFilePos()
{
    if ( ! m_bBPTrans)
    {
        return true;
    }
    if (m_nOffset)
    {
        return m_pInet->AddHeaderOffset(m_nOffset);
    }
    return true;
}

bool CPiBPDown::IsNeedBP()
{
    return m_bBPTrans;
}
