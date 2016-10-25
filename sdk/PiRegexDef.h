#pragma once
#include <regex>
using namespace std;

#define NS_Pi_Regex_Begin namespace ns_pi_regex{
#define NS_Pi_Regex_End }

#define NS_Pi_Regex_Using using namespace ns_pi_regex

namespace ns_pi_regex
{
    
#ifndef _UNICODE
    typedef regex   tregex;
    typedef smatch  tmatch;
#else
    typedef wregex  tregex;
    typedef wsmatch tmatch;
#endif
}
