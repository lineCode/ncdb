#pragma once
#include "ini.h"

class CRDIni :
    public CIni
{
public:
    CRDIni(const wchar_t* filename);
    ~CRDIni(void);

public:
    virtual wchar_t*  ReadParam(wchar_t* section, wchar_t* key);
};

