#pragma once
#include "PiTypeDef.h"
#include "ResCommon.h"
#include "define_gnl.h"
#include <assert.h>

enum ENUM_OPERATOR
{
    operator_rename = 0,
    operator_delete,
};

struct tagFILE_OPER
{
    ENUM_OPERATOR oper;
    tstring strSrc;
    tstring strDist;

    bool GetRobackOper()
    {
        switch(oper)
        {
        case operator_rename:
            swap(strSrc, strDist);
            break;
        case operator_delete:
            assert(0 && _T("ÔÝ²»Ö§³Ö"));
            swap(strSrc, strDist);
            break;

        default:
            break;
        }
        return true;
    }


};

typedef list<tagFILE_OPER*> LST_OPER;
#define FOR_EACH_FILE_OPER(Cn, Iter)\
    FOR_EACH_ITER(LST_OPER, Cn, Iter)