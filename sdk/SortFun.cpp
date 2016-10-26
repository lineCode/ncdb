#include "StdAfx.h"
#include "SortFun.h"


CSortFun::CSortFun(void)
{
}


CSortFun::~CSortFun(void)
{
}

bool CSortFun::CompareString( const tstring& str1, const tstring& str2, bool bAsc, ENUM_COMPARE_RESULT& compRet )
{
    bool bRet = false;
    //ENUM_COMPARE_RESULT compCompare = comp_smaller;
    if(str1 < str2)
    {
        compRet = comp_smaller;
        bRet = true;
    }
    else if(str1 == str2)
    {
        compRet = comp_equire;
    }
    else
    {
        compRet = comp_bigger;
        bRet = true;
    }
    if (bRet)
    {
        compRet = (ENUM_COMPARE_RESULT)TurnCompare(compRet, bAsc);  //bAsc=true, 为升序
    }
    return bRet;    //有结果(非相等)
}
