
#ifndef CARLTON_VERSION_H__
#define CARLTON_VERSION_H__

#include "BaseDef.h"

/* .Declaration Header
****************************************************************************************
* Theme:	版本辅助
* Author:	Ming Zhi.Xue ( Carlton )
* Data:		2014.1.21
* Brief:	
****************************************************************************************
*/

//子版本集合容器定义
typedef std::vector<unsigned int>	SubVersionSet;

class CCarltonVersion
{
public:
	CCarltonVersion();
	~CCarltonVersion();

public:

	//解析版本字符串
	static BOOL			ParseVersion(IN LPCTSTR lpszVer,OUT SubVersionSet& set);
    static StlString    VerToString(const SubVersionSet& sVer);
	//比较字符串式版本
	static enum CmpVerRetT { paramer_error=-2,ver1_gt_ver2=1,ver1_equal_ver2=0,ver1_lt_ver2=-1 };
	static CmpVerRetT	CompareVersion(IN LPCTSTR lpszVer1,IN LPCTSTR lpszVer2);

    static CmpVerRetT CompareVersion( SubVersionSet &vs1, SubVersionSet &vs2 );

};

//////////////////////////////////////////////////////////////////////////
#endif