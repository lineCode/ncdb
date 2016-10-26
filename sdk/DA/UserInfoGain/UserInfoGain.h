#pragma once
#include <PiTypeDef.h>
class CUserInfoGain
{
public:
    CUserInfoGain(void);
    ~CUserInfoGain(void);
public:
    tstring GetUserLocalInfo();
    bool Init();
};

