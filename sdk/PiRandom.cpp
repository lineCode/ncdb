#include "StdAfx.h"
#include "PiRandom.h"
#include "PiTypeDef.h"
#include "AutoType.h"
CPiRandom::CPiRandom(void)
{

}


CPiRandom::~CPiRandom(void)
{
}

bool CPiRandom::Init()
{
    //初始化随机数种子
    ::srand((UNINT)::time(NULL));
    return true;
}

int CPiRandom::GetRandomNum( int nMax )
{
    //const int nRandomMax = 32767;
    CPiRandom rd;
    rd.Init();
    nMax = min(nMax, RAND_MAX );

    int nRandom = ::rand();
    TRACE(_T("randomNum: "));
    CAutoType at(nRandom);
    TRACE((tcpchar)at);
    TRACE(_T("\n"));

    //0 to RAND_MAX (32767). 
    //TODO: x 为 0 - 100 的随机数, +n 后为 n - 100+n 的随机数?

    nRandom = nRandom % (nMax + 1);
    return nRandom;
}
