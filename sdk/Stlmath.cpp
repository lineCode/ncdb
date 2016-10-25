#pragma once
#include "stdafx.h"
#include "Stlmath.h"
/************************************************************************/
/*  作用: 提供一些数学相关的函数接口, 用stl实现, 如迭代器,
/************************************************************************/

UNLONG SecondToMillSecond(UNLONG nSecond)
{
    return nSecond * 1000;
}

UNLONG MillSecondToSecond(UNLONG nMillSecond)
{
    return nMillSecond / 1000;
}