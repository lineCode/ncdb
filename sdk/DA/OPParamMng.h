#pragma once
#include "piTypeDef.h"
#include "OpenPlatformDef.h"
#include "PAOPDef.h"
class COPParamMng
{
public:
    COPParamMng(void);
    ~COPParamMng(void);
    static tstring GetUrlLoginAuth();
    static MAP_STRING GetParamLoginAuth(ENUM_OP_LOGIN_TYPE loginType);
public:
};

