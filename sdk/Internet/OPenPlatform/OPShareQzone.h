#pragma once
//#include "OPShareBase.h"
#include "ResCommon.h"
#include "AlbumList.h"
#include "JsonHeader.h"
#include "OPShareAlbum.h"

class COPShareQzone 
    : public COPShareAlbum<CAlbumList>
{
public:
    COPShareQzone(void);
    ~COPShareQzone(void);
public:
    virtual bool ParseAuthInfo( const tstring& strWeb );
    virtual bool QueryToken();
    virtual bool ParseToken(const tstring& strRet, string& strToken);

    //œ‡≤·∑÷œÌ
    virtual bool QuestAppAuthInfo();
    virtual bool QuestUserInfo()    ;
    virtual bool QuestAlbumList()   ;
    //virtual bool GetAlbumList(void*& pAlbum);
    virtual bool UploadPic(const tstring& strDesc, const tstring& strPath, const tstring& strAlbumID);
    virtual bool CreateAlbum(const tagCreateAlbumRR& car);

    bool Init();
    
    bool GetOpenId();
    bool ParseOpenID( const tstring& strWeb, string& m_strOpenid );
    bool ParseAlbum( const tstring& strWeb );
    void Test();
    bool ParseUserInfo( const tstring& strOutput );
    bool _ParseOneAlbum( Json::Value elem, CAlbumList* pAbLst);
    bool AddParamGeneral();
    bool ParseUploadPic( const tstring& m_strOutput );
    bool ParseAddAlbum( tstring strOutput );
private:
};

