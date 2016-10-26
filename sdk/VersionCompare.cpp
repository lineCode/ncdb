
#include "stdafx.h"
#include "VersionCompare.h"
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////

StlString CVersionCompare::ConvertVersionT2String(__in VersionT& ver,int verlen /*= -1*/)
{
	StlString strVer;
	int len = (int)ver.size();
	int dis = -1;
	if(verlen > len)
		dis = verlen - len;

	for(int i = 0;i < len; i++)
	{
		TCHAR tBuff[128] = {0};
		_stprintf(tBuff,_T("%d"),ver.at(i));
		strVer += tBuff;
	}

	if(dis)
	{
		for(int i = 0; i < dis; i++){
			strVer += _T('0');
		}
	}

	return strVer;
}

void CVersionCompare::ConvectString2Version(__in StlString strver,__out VersionT& ver,int verlen /*= -1*/)
{
	StlString strVer = strver;
	int nPos = strVer.find(_T("."));
	if( -1 == nPos )
	{
		unsigned char nSub = _ttoi(strVer.c_str());
		ver.push_back(nSub);
	}
	else
	{
		while(-1 != nPos)
		{
			StlString strSub = strVer.substr(0,nPos);
			unsigned char nSub = _ttoi(strSub.c_str());
			ver.push_back(nSub);
			int nStart = nPos + 1;
			strVer = strVer.substr(nStart,strVer.size());
			nPos = strVer.find(_T("."));

			if(-1 == nPos && !strVer.empty()){
				unsigned char nSub = _ttoi(strVer.c_str());
				ver.push_back(nSub);
				break;
			}
		}
	}

	if(verlen > (int)ver.size())
	{
		int dis = verlen - (int)ver.size();
		for(int i = 0; i < dis; i++){
			ver.push_back(0);
		}
	}
}

int CVersionCompare::CompareVersion(__in StlString v1,__in StlString v2)
{
	int v1Len = GetStringVersionLength(v1);
	int v2Len = GetStringVersionLength(v2);
	int dis = v1Len - v2Len;
	int len = v1Len;
	if(dis < 0)
		len = v2Len;


	VersionT vs1,vs2;
	ConvectString2Version(v1,vs1,len);
	ConvectString2Version(v2,vs2,len);

	for(int i = 0; i < len; ++i)
	{
		unsigned char val1 = vs1.at(i);
		unsigned char val2 = vs2.at(i);

		if( val1 > val2 )
			return 1;
		else if( val1 < val2 )
			return -1;
	}

	return 0;
}

int CVersionCompare::GetStringVersionLength(__in StlString strver)
{
	int len = 0;
	for(int i = 0; i < (int)strver.size(); i++)
	{
		TCHAR c = strver.at(i);
		if( _T('.') != c )
			++len;
	}
	return len;
}