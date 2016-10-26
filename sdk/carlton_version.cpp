
#include "StdAfx.h"
#include "carlton_version.h"

BOOL CCarltonVersion::ParseVersion( IN LPCTSTR lpszVer, OUT SubVersionSet& set)
{
	if(NULL == lpszVer)
		return FALSE;

	StlString strVer = lpszVer;
	int nPos = strVer.find(_T("."));
	while(-1 != nPos)
	{
		StlString strSub = strVer.substr(0,nPos);
		unsigned int nSub = _ttoi(strSub.c_str());
		set.push_back(nSub);
		int nStart = nPos + 1;
		strVer = strVer.substr(nStart,strVer.size());
		nPos = strVer.find(_T("."));

		if(-1 == nPos && !strVer.empty()){
			unsigned int nSub = _ttoi(strVer.c_str());
			set.push_back(nSub);
			break;
		}
	}

	return TRUE;
}

//v1 > v2 => 1 ; v1 = v2 => 0 ; v1 < v2 => -1 ;
CCarltonVersion::CmpVerRetT CCarltonVersion::CompareVersion(IN LPCTSTR lpszVer1,IN LPCTSTR lpszVer2)
{
	if(NULL == lpszVer1 || NULL == lpszVer2)
		return paramer_error;

	SubVersionSet vs1,vs2;
	ParseVersion(lpszVer1,vs1);
	ParseVersion(lpszVer2,vs2);

    return CompareVersion(vs1, vs2);

}

CCarltonVersion::CmpVerRetT CCarltonVersion::CompareVersion( SubVersionSet &vs1, SubVersionSet &vs2 )
{
    //版本号多的为较新版本
    if(vs1.size() > vs2.size()){
        int n = vs1.size() - vs2.size();
        for(int i = 0; i < n; ++i)
            vs2.push_back(0);
        //return ver1_gt_ver2;
    }
    else if(vs1.size() < vs2.size()){
        int n = vs2.size() - vs1.size();
        for(int i = 0; i < n; ++i)
            vs1.push_back(0);
        //return ver1_lt_ver2;
    }

    for(int i = 0; i < vs1.size(); ++i)
    {
        unsigned int val1 = vs1.at(i);
        unsigned int val2 = vs2.at(i);

        if( val1 > val2 )
            return ver1_gt_ver2;
        else if( val1 < val2 )
            return ver1_lt_ver2;
    }

    return ver1_equal_ver2;
}

StlString CCarltonVersion::VerToString( const SubVersionSet& sVer)
{
    StlString strVer;
    size_t nSize = sVer.size();
    TCHAR cBuf[100] = {0};
    for (int i = 0; i < nSize; ++i)
    {
        _itot(sVer[i], cBuf , 10);
        strVer += cBuf;
        strVer += _T(".");
    }
    strVer.back() = '\0';
    return strVer.c_str();
}
