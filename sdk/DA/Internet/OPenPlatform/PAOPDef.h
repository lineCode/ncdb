#pragma once

enum ENUM_OP_LOGIN_TYPE
{
    lt_sina = 0,
    lt_qq,
};

const tcpchar STRING_LOGIN_TYPE[] = 
{
    _T("sina"), 
    _T("qq"), 
};

enum ENUM_OP_SHARE_TYPE
{
    st_sina = 0,
    st_renren, 
};