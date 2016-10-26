#pragma once
#include <regex>
#include "PiRegexDef.h"
#include "PiTypeDef.h"
#include "define_gnl.h"
#include <list>
using namespace std;

typedef list<tstring>   LST_STRING;

NS_Pi_Regex_Begin

class CPiRegex
{
public:
    CPiRegex(void);
    ~CPiRegex(void);
public:
    bool GetIpPort(tcpchar szSrc, LST_STRING& lstMatch);

    bool CPiRegex::IsMatch(tcpchar szSrc, tcpchar szPattern);
    bool CPiRegex::FindPatternFirst(tcpchar szSrc, tcpchar szPattern, tstring& strMatch);
    bool CPiRegex::FindPattern(tcpchar szSrc, tcpchar szPattern, LST_STRING& lstMatch);
};

NS_Pi_Regex_End