#include "stdafx.h"

/*************
*
*作者 Abel(李万良)
*功能 一些字符串的操作： 查找，替换···
*************/
#include <Windows.h>
#include "doString.h"
#include <comutil.h>
#include "PiTypeDef.h"
#include "define_gnl.h"
#include <algorithm>
using std::string;
#pragma comment(lib, "comsuppw.lib") //_bstr_t 需要的库文件
int GetContentBetweenChar(const tstring& strSource,
                          tstring & strDest, 
                          tcpchar c,
						  const UINT cTimes /*= 1*/,
                          UINT pos /*= 0*/ )
{
	//int pos(0);
	size_t charLen = _tcslen(c);
	size_t end(0);
	//string strPHtype;
	string     strTemp;
	const int nNPos = cTimes ;//第几个间隔符后的数据
    byte bySymbleHoldLen = cTimes > 0 ? charLen : 0;
    if (cTimes > 0)
    {
        pos = strSource.find(c ,pos);
        if(pos == string::npos)
        {
            return pos ;
        }
        for (int i = 2;i <= nNPos;i++)
        {
            pos = strSource.find(c ,NextPos(pos));
            if(pos == string::npos)
            {
                return pos ;
            }

        }
    }
	
	end = strSource.find(c ,NextPos(pos + charLen));

	//wstring wTemp;
	//没找到右边的 区间字符，说明剩下右边的就是数据
	if(end ==  string::npos)
	{
		if(pos == strSource.length() - 1)//要查找的字符是最后的字符,说明右边没有数据了
		{
			strDest = _T("");
		}
		else
		{
			strDest = strSource.substr(pos + bySymbleHoldLen,string::npos);
		}
	}
	else
	{
		strDest = strSource.substr(pos + bySymbleHoldLen,end - pos - bySymbleHoldLen);
	}
	
    if (0 == cTimes)
    {
        return end;
    }
	return end;
}

//获取两个 不同字符 中间的字符串
int GetContentBetweenTwoChar(const tstring& strSource, 
                             tstring & strDest, 
                             tcpchar c1,
                             tcpchar c2 /*= 1*/,
                             const UINT cTimes1,
                             UINT pos /*= 0*/)
{
	int c1Len = _tcslen(c1);//裁剪字符串时必须算上查找字符的长度
	int c2Len = _tcslen(c2);
	
	int end(-1);
	//string strPHtype;
	string     strTemp;
	const int nNPos = cTimes1 ;//第几个间隔符后的数据

	pos = strSource.find(c1 ,pos);
	if (pos ==  string::npos )
	{
		return end;
	}

	for (UINT i = 2;i <= cTimes1;i++)
	{
		pos = strSource.find(c1 ,pos + 1);

	}
	
	end = strSource.find(c2, NextPos(pos + c1Len));
	

	//wstring wTemp;
	//没找到右边的 c2字符，则查找换行字符
	if(end ==  string::npos )
	{
        end = strSource.find(_T("\r\n"),pos+1);
		if (end == string::npos)
		{
		
			if(pos == strSource.length() - 1)//要查找的字符是最后的字符,说明右边没有数据了
			{
				strDest = _T("");
			}
			else//不是最后一个字符,则把剩下的都拿出来
			{
				strDest = strSource.substr(pos + c1Len,string::npos);
				
			}
		}
	}
	else //有找到右边的c2字符
	{
		strDest = strSource.substr(pos + c1Len,end - pos - c1Len);
	}
	return end; //返回符合条件的c2的位置
}


bool StrReplace(tstring& strSource,tcpchar c,tcpchar c2  /*= ""*/, int nCount/* = -1*/)
{
    //-1,无限, 0, 不找, >0 , 指定次数
	bool bReplase = false;
    if (nCount == 0)
    {
        return bReplase;
    }
	int pos = -1;
	bool bCount = nCount >0;
	while(1)
	{
        if (nCount > 0)
        {
            --nCount;
        }
        else if(bCount)
        {
            break;
        }

		pos = strSource.find(c, pos + _tcslen(c2));
		if (pos == string::npos)
		{
			break;
		}
		strSource.replace(pos, _tcslen(c), c2, _tcslen(c2));
        ++bReplase;
	}
	return bReplase;
}

/*
bool StrReplace( wstring& strSource,PCSTR c,PCSTR c2 / *= ""* / )
{
    string strTemp(ws2s(strSource.c_str()));
    bool bRet = StrReplace(strTemp, c, c2);
    strSource = s2ws(strTemp.c_str());
    return bRet;
}
*/

bool StrReplaceFirst( tstring& strSource, tcpchar cFind, tcpchar cDist /*= _T("")*/ )
{
    int pos=0;

    pos = strSource.find(cFind,0);
    if (pos == string::npos)
    {
        return false;
    }
    strSource.replace(pos,_tcslen(cFind),cDist,_tcslen(cDist));
    return true;
}

int GetCharPos(const tstring strSource,tcpchar c,const UINT cTimes /*= 1*/, int nOffset /*= 0*/)
{
	int end(0);
	int pos = nOffset;
	//int strLen = strlen(c); //字符长度

	const int nNPos = cTimes ;//第几个间隔符后的数据

	pos = strSource.find(c ,pos);
	for (int i = 2;i <= nNPos && pos != string::npos;i++)
	{
		pos = strSource.find(c ,pos + 1);

	}

	//end = strSource.find(c ,pos + 1);
	return pos;
}

DWORD GetCharHaveCount( const tstring &strSource, tcpchar pCh )
{
	DWORD nCount = 0;
	DWORD pos = 0;
	pos = strSource.find(pCh,pos);
	while(pos != string::npos)
	{
		pos = strSource.find(pCh,pos+1);
		++nCount;

	}
	return nCount;
}


string ws2s(const wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}

string ws2s(const wchar_t* ws)
{
	_bstr_t t = ws;
	string result = (char*)t;
	return result;
}

wstring s2ws(const string& s)
{
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	wstring result = pwchar;
	return result;
}

wstring s2ws(const char* s)
{
	_bstr_t t = s;
	wstring result  = (wchar_t*)t;
	return result;
}

bool StrReplaceRange( tstring& strSource,tcpchar nFlagLeft, tcpchar nFlagRight,tcpchar cDist /*= _T("")*/, int nCount /*= -1*/ )
{
	if (strSource.empty() || nCount == 0)
	{
		return false;
	}

	//查找左边位置
	int nPosLeft = 0;
	int nPosRight = 0;
	//查找右边位置
	int nCountLeft = nCount;
	bool bCount = !(nCountLeft == -1);
	while(1)
	{
		nPosLeft = strSource.find(nFlagLeft, nPosLeft);
		if (!IsValidStrPos(nPosLeft))
		{
			break;
		}
		nPosRight = strSource.find(nFlagRight, NextPos(nPosLeft));
		if (!IsValidStrPos(nPosRight))
		{
			break;
		}

		strSource.replace(nPosLeft, nPosRight - nPosLeft + 1/*右边标识也作为替换原*/, cDist);
		nPosLeft = NextPos(nPosRight);
		if (bCount && --nCountLeft <= 0)
		{
			break;
		}
	}
	return true;
}

#define IsDigit(num) ((num) >= '0' && (num) <= '9' )
bool Pred_Not_Digit(tchar cChar)
{
	return !(IsDigit(cChar) 
		|| cChar == '.'
		);
}
bool EraseNoDigit( tstring& szStrR )
{
	szStrR.assign(szStrR.begin(), remove_if(szStrR.begin(), szStrR.end(), Pred_Not_Digit));
	return true;
}

bool IsStrExist( const tstring str1, tstring& str2 )
{
	return str1.find(str2) != tstring::npos;
}

bool IsStrExist( const tstring str1, tcpchar str2 )
{
	return str1.find(str2) != tstring::npos;
}
