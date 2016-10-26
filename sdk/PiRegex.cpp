#include "StdAfx.h"
#include "PiRegex.h"
//using namespace ns_pi_regex;
NS_Pi_Regex_Using;

CPiRegex::CPiRegex(void)
{
}


CPiRegex::~CPiRegex(void)
{
}

bool CPiRegex::IsMatch(tcpchar szSrc, tcpchar szPattern)
{
    tregex rgObj(szPattern);
    tmatch mResult;
    tstring strSrc(szSrc);
    if(regex_match(strSrc, mResult, rgObj))
    {
        return true;
        //cout<<_T("ok")<<endl;
    }
    return false;
}

bool CPiRegex::FindPatternFirst( tcpchar szSrc, tcpchar szPattern, tstring& strMatch)
{
    tregex   rgObj(szPattern);
    tmatch  mResult;
    tstring strSrc(szSrc);
    bool bRet = regex_search(strSrc, mResult, rgObj
        , regex_constants::match_not_null
        //| regex_constants::match_any
        );
    if (bRet)
    {
        strMatch = mResult.str();
    }
    return bRet;
}

bool CPiRegex::FindPattern( tcpchar szSrc, tcpchar szPattern, LST_STRING& lstMatch )
{
    tregex   rgObj(szPattern);
    tmatch  mResult;
    tstring strSrc(szSrc);
    bool bMatched = false;

    for (; regex_search(strSrc, mResult, rgObj
        , regex_constants::match_not_null
        //| regex_constants::match_any
        //| regex_constants::match_continuous
        )
        ; )
    {
        bMatched = true;
        lstMatch.push_back(mResult.str());
        strSrc = mResult.suffix();
    }

    return bMatched;
}

bool ns_pi_regex::CPiRegex::GetIpPort( tcpchar szSrc, LST_STRING& lstMatch )
{
    //szSrc = _T("27.151.127.,");
    //szSrc = _T("27.46.144,");
    
    tcpchar szPattern = _T("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}:\\d{1,6}");

    return FindPattern(szSrc, szPattern, lstMatch);
}
