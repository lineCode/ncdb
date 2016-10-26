
#pragma once

#include <string>
#include <vector>
using namespace std;

/*
***********************************************************************
* Theme:	版本比较
* Author:	Ming Zhi.Xue
* Data:		2014/02/28
***********************************************************************
*/

//版本类型定义
typedef		vector<unsigned char>	VersionT;

//字符串类型定义
#ifdef UNICODE
typedef		wstring					StlString;
#else
typedef		string					StlString;
#endif//UNICODE

class CVersionCompare
{
public:

	//VersionT转换成字符串，verlen=-1时，默认长度
	static StlString	ConvertVersionT2String(__in VersionT& ver,int verlen = -1);

	//字符串转换成VersionT，verlen=-1时，默认长度
	static void			ConvectString2Version(__in StlString strver,__out VersionT& ver,int verlen = -1);

	//版本比较，0：相等 1：v1>v2 -1：v1<v2
	static int			CompareVersion(__in StlString v1,__in StlString v2);

	//计算字符串型版本号有几位子版本
	static int			GetStringVersionLength(__in StlString strver);
};
