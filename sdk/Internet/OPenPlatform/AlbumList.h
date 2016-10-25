#pragma once
#include "listmng.h"
#include "OPQzoneDef.h"
class CAlbumList :
    public CListMng<tagAlbumQzone, LST_ALBUM_QZONE>
{
public:
    CAlbumList(void);
    ~CAlbumList(void);
};

