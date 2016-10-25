#pragma once
#include "piTypeDef.h"
class CPANetParamMng
{
public:
    CPANetParamMng(void);
    ~CPANetParamMng(void);
public:
    static tstring GetUrlNews();
    static MAP_STRING GetParamNews();
    static tstring GetGUID();
};

