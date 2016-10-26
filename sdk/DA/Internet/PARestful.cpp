#include "StdAfx.h"
#include "PARestful.h"
//#include "PANetParamMng.h"
#include "functional.h"
#include "StrTrans.h"
#include "Markup.h"
#include "StrCoding.h"
#include "define_gnl.h"
//#include "NewsMng.h"
#include "AutoType.h"
//#include "PAMsgPushDef.h"

//#define MARKUP_STL  //让Cmarkup使用stl::string类型

CPARestful::CPARestful(void)
{
    Create_Auto_Ptr(m_pXML, CMarkup);
    m_SendType = Restful_Get;
}

CPARestful::~CPARestful(void)
{
}

bool CPARestful::GetDistWebData( tstring& strData )
{
// #ifdef DEBUG
// 
// 	m_strWebData = 
//  		_T("<?xml version=\"1.0\" encoding=\"utf-8\"?> \
		<xml><data><datum><type>6LWE6K6v</type>\
		<title>6IGU5ZCI5LyY5Yib5o6o5Ye6VUJPWO+8jOa1t+mHj+WKqOa8q+e0oOadkOS4gOmUruWuieijhQ==</title>\
		<content>6L+R5pel77yM6IGU5ZCI5LyY5Yib5o6o5Ye65LiA5qy+UEPnq6/lt6Xlhbfova/ku7bigJxVQk9Y4oCd77yM5L2c5Li65Yqo5ryr5Yib5L2c5pSv5o+05bmz5Y+w4oCU4oCU5LyY5Yqo5ryr55qE5a6i5oi356uv56iL5bqP77yM5ZCR5bm/5aSn55So5oi35q2j5byP5byA5pS+57Sg5p2Q5LiL6L295a6J6KOF5pyN5Yqh44CCIA==</content>\
		<url>aHR0cDovL3d3dy51ZG9uZ21hbi5jbi9pbmRleC5waHA/bT1uZXdzJmM9aW5mbyZpZD02OQ==</url>\
		<date>1396578900</date></datum></data></xml>"
// #endif

    string strDataTemp = CStrCoding::UnicodeToAnsi(m_strWebData.c_str());
    strData = CStrCoding::Utf8ToUnicode(strDataTemp.c_str());

    
    return true;
}


bool CPARestful::DoParam()
{
    /* 对所有参数值 进行 base64加密*/
    FOR_EACH_ITER(MAP_STRING, m_MapParam, it)
    {
        string strTemp = CStrCoding::UnicodeToAnsi(it->second.c_str());
        it->second = CStrCoding::AnsiToUnicode(CStrCoding::EncodeBase64( strTemp.c_str() , strTemp.length()).c_str());
    }
    return true;
}






