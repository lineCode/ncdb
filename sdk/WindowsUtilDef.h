#pragma once
#include "PiTypeDef.h"
#include <atltypes.h>
#include <WinDef.h>
class tagWindowInfo
{
public:
    HWND    hWnd;
    tstring strName;
    CRect   rt;
    tagWindowInfo()
    {
        hWnd = 0;
    }
};
typedef list<tagWindowInfo*>    LST_WINDOW;