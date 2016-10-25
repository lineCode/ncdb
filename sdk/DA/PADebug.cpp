#include "StdAfx.h"
#include "PADebug.h"

#define INFO_PA1_1

void PAOut10( tcpchar sz1 )
{
#ifdef INFO_PA1_0
    OutInfo(sz1);
#endif
}

void PAOut10( tcpchar sz1, tcpchar sz2 )
{
#ifdef INFO_PA1_0
    OutInfo(sz1, sz2);
#endif
}



void PAOut11( tcpchar sz1 )
{
#ifdef INFO_PA1_1
    OutInfo(sz1);
#endif
}

void PAOut11( tcpchar sz1, tcpchar sz2 )
{
#ifdef INFO_PA1_1
    OutInfo(sz1, sz2);
#endif
}

