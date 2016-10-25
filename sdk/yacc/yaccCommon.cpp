#include "stdafx.h"
#include "yaccCommon.h"
#include <fstream>
#include <iostream>
using namespace std;
void DebugInfo(const char* s)
{
#ifdef DEBUG_INFO
    cout<<s<<endl;
#endif
}

void DebugInfo(const char* s, int nValue)
{
#ifdef DEBUG_INFO
    cout<<s<<nValue<<endl;
#endif
}

void DebugInfo(const wchar_t* s)
{
#ifdef DEBUG_INFO
    wcout<<s<<endl;
#endif
}

void DebugInfo(const wchar_t* s, int nValue)
{
#ifdef DEBUG_INFO
    wcout<<s<<nValue<<endl;
#endif
}