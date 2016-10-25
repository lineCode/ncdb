#pragma once
#include "PiTypeDef.h"
//#include "BaseDef.h"

class CFileOpr;
class CInternetHttp;
class CPiBPDown
{
public:
    struct tagFILE_BP_INFO 
    {
        tstring strUrl;
        tstring strLastModify;
    };

    CPiBPDown(CInternetHttp* pInet, bool bNeedBP = false);
    ~CPiBPDown(void);
    bool WriteBPInfo();
    static int GetBPInfoLen(const tstring& szPath);
    /*tstring Url() const { return m_strUrl; }
    void Url(tstring val) { m_strUrl = val; }*/
    /*  func:   获取已离线数据大小,  如果不支持离线会删除文件, 否则dwDownedSize返回已离线大小
        ret:    false:没有离线缓存的数据
    */
    //static bool GetDownedSize( StringCRef strUrl, const tstring& strPath, UNLONG& dwDownedSize );
    bool CalcDownedSize(StringCRef strPath);
    static bool IsSupposeBPDown( const tstring& strUrl );
    bool SetOffsetLocal( );
    bool MoveInetFilePos();

    bool MoveLocalFilePos();

    bool SetDataOffset( UNLONG nOffset );
    bool ClearBPInfo( const tstring& strSavePath );
    void SetIsBpDown( bool bNeedBP );
    void SetFile( CFileOpr* file );
    UNLONG GetDownedSize();
    bool IsNeedBP();
private:
    CFileOpr*                       m_pFile;
    CInternetHttp*                  m_pInet;
    //tstring                         m_strUrl;
    UNLONG                      m_nOffset;
    bool                        m_bBPTrans;
};

