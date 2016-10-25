#include "StdAfx.h"
#include "OPShareMsg.h"


COPShareMsg::COPShareMsg(void)
{
}


COPShareMsg::~COPShareMsg(void)
{
}


bool COPShareMsg::SendPic(const tstring& strText, const tstring& strPath)
{
    if (QueryToken() 
        && SendWeiboPic(strText, strPath))
    {
        return true;
    }
    return false;
}

bool COPShareMsg::SendPic2( const tagSEND_WEIBO& tagWB )
{
    if (SendPic(tagWB.strText, tagWB.strFile))
    {
        return true;
    }
    return false;
}
