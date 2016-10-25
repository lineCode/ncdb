#pragma once
#include "PiTypeDef.h"
enum ENUM_COMPARE_RESULT
{
    comp_bigger     = 0,
    comp_smaller    = 1,    //小的放前面
    comp_equire,
};
#define TurnCompare(compRet, bAsc) (bAsc ? compRet: (compRet == comp_bigger ? comp_smaller: comp_bigger))// ~(bool)compRet << 6)
#define CompareToBool(compRet) (compRet == comp_smaller)


class CSortFun
{
public:
    CSortFun(void);
    ~CSortFun(void);
public:
    static bool CompareString(const tstring& str1, const tstring& str2, bool bAsc, ENUM_COMPARE_RESULT& compRet);
    template<class T> 
    static bool Compare(const T& str1, const T& str2, bool bAsc, ENUM_COMPARE_RESULT& compRet);
};

template<class T>
bool CSortFun::Compare( const T& str1, const T& str2, bool bAsc, ENUM_COMPARE_RESULT& compRet )
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

