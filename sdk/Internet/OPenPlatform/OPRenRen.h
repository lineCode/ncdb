#pragma once
#include "opsharebase.h"
#include "ListMng.h"
#include "OPRenRenDef.h"
#include "JsonHeader.h"
#include "OPSharePA.h"
#include "OPShareAlbum.h"
class CRRAlbumList :
    public CListMng<tagRRAlbum, LST_RR_ALBUM>
{
public:
    CRRAlbumList(void){     }
    ~CRRAlbumList(void){    }
};


class COPRenRen :
    public COPShareAlbum<CRRAlbumList>
{
public:
    COPRenRen(void);
    ~COPRenRen(void);
public:
    virtual bool ParseAuthInfo( const tstring& strWeb );
    virtual bool QueryToken();
    virtual bool UploadPic(const tstring& strDesc, const tstring& strPath, const tstring& strAlbumID);

    /************************************************************************
        fun:    解析出token ,并保存
        remark:
        ret:   
    ************************************************************************/
    virtual string GetRequestCodeUrl();

    //相册分享
    virtual bool QuestUserInfo()    ;
    virtual bool QuestAlbumList()   ;
    virtual bool CreateAlbum(const tagCreateAlbumRR& car);

    bool ParseAlbumList( const tstring& strWeb );

    bool ParseAlbum( const tstring& strWeb, CRRAlbumList* pAlbumLst);

protected:
    bool AddParamGeneral();
    bool ParseUserInfo( const tstring& strWeb );

    bool IsQuestOk( const tstring& strWeb, Json::Value* pBody = NULL);

    bool _ParseOneAlbum( const Json::Value& elem, CRRAlbumList* plbumLst);
    bool ParseAddTopic( const tstring& m_strOutput );
    bool ParseAddAblum( const tstring& strOutput );
private:
    tagRRUser           m_rrUser;
};

