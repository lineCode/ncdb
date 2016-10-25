#pragma once
#include "OPSharePA.h"
#include "OpenPlatformDef.h"
#include "ResCommon.h"

template<class AlbumList>
class COPShareAlbum:
    public COPShareBase
{
public:
    COPShareAlbum(void);
    ~COPShareAlbum(void);
public:

    //œ‡≤·∑÷œÌ

    bool QuestAlbumInfo(int nNoUse = 0);
    template<class CBObj, class CBFunc, class CBParam> 
    bool QuestAlbumInfoAsync(CBObj cbObj, CBFunc cbFun, CBParam cbPr)
    {
        CallASync(this, &COPShareAlbum::QuestAlbumInfo, 0, cbObj, cbFun, cbPr);
        return true;
    }


    //œ‡≤·
    virtual bool QuestUserInfo()    = 0;
    virtual bool QuestAlbumList()   = 0;
    virtual bool CreateAlbum( const tagCreateAlbumRR& up ) = 0;

    virtual AlbumList* GetAlbumList()
    {
        return m_pAlbumLst.get();
    }
    virtual bool UploadPic(const tstring& strDesc, const tstring& strPath, const tstring& strAlbumID) = 0;

    bool UploadPicTag(const tagUPLOAD_PIC& up);

    bool CreateAlbumTag( const tagCreateAlbumRR& up )
    {
        return this->CreateAlbum(up);
    }


    template<class CBObj, class CBFunc, class CBParam>
    bool UploadPicAsync( const tagUPLOAD_PIC& up, CBObj cbObj, CBFunc cbFun, CBParam cbPr)
    {
        CallASync(this, &COPShareAlbum::UploadPicTag, up, cbObj, cbFun, cbPr);
        return true;
    }

    template<class Param, class CBObj, class CBFunc, class CBParam>
    bool CreateAlbumAsync( const Param& up, CBObj cbObj, CBFunc cbFun, CBParam cbPr)
    {
        CallASync(this, &COPShareAlbum::CreateAlbumTag, up, cbObj, cbFun, cbPr);
        return true;
    }
    
protected:
    auto_ptr<AlbumList>               m_pAlbumLst;
};


template<class AlbumList>
COPShareAlbum<AlbumList>::COPShareAlbum(void)
{
    Create_Auto_Ptr(m_pAlbumLst, AlbumList);
}

template<class AlbumList>
COPShareAlbum<AlbumList>::~COPShareAlbum(void)
{
}

template<class AlbumList>
bool COPShareAlbum<AlbumList>::QuestAlbumInfo( int nNoUse /*= 0*/ )
{
    if( QuestAppAuthInfo()
        && QuestUserInfo()
        && QuestAlbumList())
    {
        return true;
    }
    return false;
}

template<class AlbumList>
bool COPShareAlbum<AlbumList>::UploadPicTag( const tagUPLOAD_PIC& up )
{
    return UploadPic(up.strText, up.strFile, up.strAlbumID);
}




