#include "StdAfx.h"
#include "UploadFile.h"
#include "InternetHttp.h"
#include "Math/mathUnit.h"
#include <sstream>
#include <afxinet.h>
#include "DataMng.h"

using namespace std;
Pi_NameSpace_Using

#define NewMemAndInit(pBuf, nFileSize) pBuf = new char[nFileSize]; memset(pBuf, 0, nFileSize);
BOOL  UploadFile(LPCTSTR strURL, //负责接收上传操作的页面的URL
    LPCTSTR strLocalFileName);


CUploadFile::CUploadFile(void)
{
}


CUploadFile::~CUploadFile(void)
{
}

bool CUploadFile::Init()
{
    m_pInternet = auto_ptr<CInternetHttp>(new CInternetHttp);

    return true;
}

bool CUploadFile::UploadFile( const tchar* szUrl, const tchar* szLocalFile )
{
    //const tchar* szU;
    //::UploadFile(szUrl, szLocalFile);

    if(!m_pInternet->DownloadInit((PCTSTR)szUrl))
    {
        return false;
    }

    //用POST动作 发送一个请求, 并把文件数据放在option里, 一起发送过去
    tstring strObject = CInternetHttp::FindHttpObject(szUrl);
    //添加相应的报头

    HINTERNET hCn = m_pInternet->GetConnectHandle();

    tpchar szVert = _T("POST");
    DWORD dwFalg = INTERNET_FLAG_KEEP_CONNECTION;
    HINTERNET hQeq		= ::HttpOpenRequest(hCn, szVert, strObject.c_str()/*objectName*/, NULL, NULL, NULL, dwFalg, NULL);

    tstring strHeader(GetHeadData());
    if(!HttpAddRequestHeaders(hQeq, strHeader.c_str(), strHeader.length(), NULL))
    {
        return false;
    }
    
    void* pBody = NULL;
    ULONGLONG nBodySize = GetBodyData(szLocalFile, pBody);
    BOOL bRet			= ::HttpSendRequest(hQeq, strHeader.c_str(), strHeader.size() * sizeof(tchar)
        , pBody
        , nBodySize);
    if (!bRet)
    {
        int a = GetLastError();
        return false;
    }
   
    ::InternetCloseHandle(hQeq);
    m_pInternet->DownloadUnInit();

    return true;
}

ULONGLONG CUploadFile::GetBodyData( const tchar* szPath, void* pBuf )
{
    tstring strBodyAppend;
    

    void* pBufTemp = NULL;
        
    ULONGLONG nFileSize = GetFileData(szPath, pBufTemp);
    //GetFileName(szPath);
    tcpchar szBodyPart1 = _T("-----------------------------3951081831091\r\n\
Content-Disposition: form-data; name=\"fileupload1\"; filename=\"upload.cpp\"\r\n\
Content-Type: text/plain\r\n\
\r\n");
    tcpchar szBodyPart2 = _T("\r\n\
-----------------------------3951081831091\r\n\
Content-Disposition: form-data; name=\"fileupload2\"; filename=""\r\n\
Content-Type: application/octet-stream\r\n\
\r\n\
-----------------------------3951081831091--\r\n");
    //Result =  AfxParseURL(strURL, dwType, strS)

    CDataMng dataMng;
    dataMng.Append(szBodyPart1, strlen(szBodyPart1));
    dataMng.Append(pBufTemp, nFileSize);
    dataMng.Append(szBodyPart2, strlen(szBodyPart2));
    
    pBuf= dataMng.GetData();

    return dataMng.GetDataSize();
}

tstring CUploadFile::GetHeadData()
{
    tstring strHead;
    strHead = _T("User-Agent: Mozilla/5.0 (Windows NT 6.2; WOW64; rv:25.0) Gecko/20100101 Firefox/25.0\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n\
Accept-Language: zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3\r\n\
Accept-Encoding: gzip, deflate\r\n\
Connection: keep-alive\r\n\
Content-Type: multipart/form-data; boundary=---------------------------3951081831091\r\n");
    return strHead;
}

ULONGLONG CUploadFile::GetFileData( const tchar* szPath, void*& pBuf )
{
    CFile file;
    if(!file.Open(szPath, CFile::modeRead))
    {
        return false;
    }
    ULONGLONG nFileSize = file.GetLength();
    if (nFileSize > NUM_FILE_SIZE_1M * 10)
    {
        return nFileSize;
    }

    NewMemAndInit(pBuf, nFileSize + 1);

    UINT nRet = file.Read(pBuf, nFileSize);
    if (nRet != nFileSize)
    {
        delete pBuf;
        pBuf = NULL;
    }
    return nFileSize;
}


BOOL UploadFile(LPCTSTR strURL, //负责接收上传操作的页面的URL
    LPCTSTR strLocalFileName)  //待上传的本地文件路径
{
    ASSERT(strURL != NULL && strLocalFileName != NULL);

    BOOL bResult = FALSE;
    DWORD dwType = 0;
    CString strServer;
    CString strObject;
    INTERNET_PORT wPort = 0;
    DWORD dwFileLength = 0;
    char * pFileBuff = NULL;

    CHttpConnection * pHC = NULL;
    CHttpFile * pHF = NULL;
    CInternetSession cis;

    bResult =  AfxParseURL(strURL, dwType, strServer, strObject, wPort);
    if(!bResult)
        return FALSE;
    CFile file;
    
        if(!file.Open(strLocalFileName, CFile::shareDenyNone | CFile::modeRead))
            return FALSE;
        dwFileLength = file.GetLength();
        if(dwFileLength <= 0)
            return FALSE;
        pFileBuff = new char[dwFileLength];
        memset(pFileBuff, 0, sizeof(char) * dwFileLength);
        file.Read(pFileBuff, dwFileLength);

        const int nTimeOut = 5000;
        cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //联接超时设置
        cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);  //重试1次
        pHC = cis.GetHttpConnection(strServer, wPort);  //取得一个Http联接

        pHF = pHC->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject);
        if(!pHF->SendRequest(NULL, 0, pFileBuff, dwFileLength))
        {
            delete[]pFileBuff;
            pFileBuff = NULL;
            pHF->Close();
            pHC->Close();
            cis.Close();
            return FALSE;
        }
        DWORD dwStateCode = 0;
        pHF->QueryInfoStatusCode(dwStateCode);

        if(dwStateCode == HTTP_STATUS_OK)
            bResult = TRUE;
        return TRUE;
}