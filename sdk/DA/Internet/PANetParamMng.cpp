#include "StdAfx.h"
#include "PANetParamMng.h"
#include "functional.h"
#include "PAXmlOther.h"
#include "unicorn_test_environment_def.hpp"


//#define TEST_86

CPANetParamMng::CPANetParamMng(void)
{
}


CPANetParamMng::~CPANetParamMng(void)
{
}

tstring CPANetParamMng::GetUrlNews()
{
#ifdef UNICORN_TEST_ENVIRONMENT
	return _T("http://pa.test.udongman.cn/index.php/message/news");
#endif // UNICORN_TEST_ENVIRONMENT

#ifndef TEST_86
    return _T("http://pa.udongman.cn/index.php/message/news"); 
#endif

    return _T("http://paofu.sinaapp.com/index.php/message/newstest");
    //return _T("http://192.168.2.86/index.php/message/newstest");

    //return _T("http://pa.udongman.cn/index.php/software/list");
}

MAP_STRING CPANetParamMng::GetParamNews()
{
    MAP_STRING mapParam;
#ifndef TEST_86
    tstring strGUID = GetGUID();
    mapParam[_T("guid")] = strGUID;
#endif

    //test
    //mapParam[_T("l")] = _T("8E7BB14CF7DC14810982AAEAC865EA5CCA6D1E4ECD282851810C7FD45D78AFF488D0378D0EA3DCF3");
    return mapParam;
}

tstring CPANetParamMng::GetGUID()
{
    //从配置文件中获取guid字符串
    tstring strGUID;
    CPAXmlOther xmlOther;
    if (xmlOther.Init() && xmlOther.OpenDB())
    {
        xmlOther.ReadData(ddso_guid, strGUID);      //GUID
    }
    //strGUID = MakeGUID();
    return strGUID;
    //return _T("strGUID");
}
