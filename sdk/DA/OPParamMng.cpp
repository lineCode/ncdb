#include "StdAfx.h"
#include "OPParamMng.h"

// Modified by xuemingzhi [2014/04/23] >>>
//#include "unicorn_test_environment_def.hpp"

// ¨¢ao?¨®?¡ä¡ä¦Ì?2¨²?¡¤2a¨º??¡¤?3
#ifdef UNICORN_TEST_ENVIRONMENT
#define STRING_URL_GET_AUTH         _T("http://pa.test.udongman.cn/index.php/member/authorize")
#else
#define STRING_URL_GET_AUTH         _T("http://pa.udongman.cn/index.php/member/authorize")
#endif //UNICORN_TEST_ENVIRONMENT

// #ifndef DEBUG
//     #define STRING_URL_GET_AUTH         _T("http://pa.udongman.cn/index.php/member/authorize")
// #else
//     #define STRING_URL_GET_AUTH         _T("http://pa.test.udongman.cn/index.php/member/authorize")
// #endif

// Modified by xuemingzhi[2014/04/23] <<<
COPParamMng::COPParamMng(void)
{
}


COPParamMng::~COPParamMng(void)
{
}

tstring COPParamMng::GetUrlLoginAuth()
{
    return STRING_URL_GET_AUTH;
}

MAP_STRING COPParamMng::GetParamLoginAuth(ENUM_OP_LOGIN_TYPE loginType)
{
    MAP_STRING mapParam;
    mapParam[_T("type")] = STRING_LOGIN_TYPE[loginType];
    return mapParam;
}
