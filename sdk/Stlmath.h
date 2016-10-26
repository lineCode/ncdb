#pragma once
#include "PiTypeDef.h"
/************************************************************************/
/*  作用: 提供一些数学相关的函数接口, 用stl实现, 如迭代器,
/************************************************************************/

template <class InputIterator>
__int64 Sum(InputIterator first, InputIterator last);

template <class InputIterator>
__int64 Average(InputIterator first, InputIterator last);

/************************************************************************
        fun:	判断一个数是否是2的乘方
        param:  
        ret:    如果是则返回true, 否则返回false
        remark: 
/************************************************************************/
template<class T>
bool Is2Power(T nNum);



template <class InputIterator>
__int64 Sum(InputIterator first, InputIterator last)
{
    __int64 iTotal = 0;
    for (;first != last; ++first)
    {
        iTotal += (*first);
    }
    return iTotal;
}






template <class InputIterator>
__int64 Average(InputIterator first, InputIterator last)
{
    __int64 iTotal = Sum(first, last);

    return iTotal/ (last - first);
}



UNLONG SecondToMillSecond(UNLONG nSecond);
UNLONG MillSecondToSecond(UNLONG nMillSecond);

template<class T>
bool Is2Power(T nNum)
{
	/************************************************************************
		一个数是2的乘方, 则该数二进制最高位为1, 其余为0,  (n-1)& n == 0
	/************************************************************************/
	return (nNum & nNum - 1) == 0;
}
