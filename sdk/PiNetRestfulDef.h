#pragma once
#include "piTypeDef.h"

const tcpchar g_szType[] = 
{
    _T("POST")
    ,_T("GET")
    ,_T("DELETE")
    ,_T("PUT")
};
enum ENUM_RESTFUL_TYPE
{
    Restful_Post = 0,
    Restful_Get,
    Restful_Delete,
    Restful_Put,
};