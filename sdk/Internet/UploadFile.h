#pragma once
#include "tTypeDef.h"
#include "PiTypeDef.h"
#include <memory>

class CInternetHttp;

using namespace std;

class CUploadFile
{
public:
    CUploadFile(void);
    ~CUploadFile(void);
public:
    bool UploadFile(const tchar* szUrl, const tchar* szLocalFile );
    bool Init();
    ULONGLONG GetBodyData(const tchar* szPath, void* pBuf);
    tstring GetHeadData();
    ULONGLONG GetFileData( const tchar* szPath, void*& pBuf);
private:
    //DownList		m_lstDown;		//下载列表
    auto_ptr<CInternetHttp>			m_pInternet;
    //auto_ptr<CDowningInfoMng>       m_pDowningInfoMng;
    UNLONG			m_dwSingleSizeLimit;	//单一文件下载大小限制
    bool            m_bQuitDown;        //是否停止下载

};

