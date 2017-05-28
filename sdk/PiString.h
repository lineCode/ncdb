#pragma once
#include "PiTypeDef.h"
#include "define_gnl.h"

using namespace std;

Pi_NameSpace_Begin
class CPiString
	: public basic_string< TCHAR, char_traits<TCHAR>,allocator<TCHAR> >
{
	typedef basic_string< TCHAR, char_traits<TCHAR>,allocator<TCHAR> > PiStringClassType;

public:
	CPiString();
	CPiString(tcpchar szData);
	CPiString(const tstring& szData);
	~CPiString(void);
public:
	void    Format(const TCHAR* format,...);
	bool	Replace(tcpchar c, tcpchar c2 = _T(""), int nCount = -1);
	bool	ReplaceZero(tcpchar c, int nCount = -1);

	static ARR_STRING SpilitStrToArray(const tstring& strSrc, tcpchar szSpilit, tcpchar szPack = NULL);
	static LST_STRING SpilitStr(const tstring& strSrc, tcpchar szSpilit);

	/*根据分隔符, 把列表中的每串字符串分成左右两部分, 返回map
	abc=4      =>        map["abc"]=4
	cf=5       =>		map["cf"]=5
	*/
	static MAP_STRING SpilitStrMap( const LST_STRING& lstSrc, tcpchar szSpilit );
	static ARR_PAIR_STR SpilitStrToPairArray( const LST_STRING& lstSrc, tcpchar szSpilit );
	static PAIR_STRING SpilitStrTwoPart(const tstring& strSrc, tcpchar szSpilit);

	static bool IsAllNum(tcpchar szStr);
	static void TermStrLeft(tstring& strSrc, tcpchar szFind = _T(" "));
	static void TermStrRight(tstring& strSrc, tcpchar szFind = _T(" "));
	static void TermStr(tstring& strSrc, tcpchar szFind = _T(" "));
	tstring Left(UNINT nCount);
	tstring Left(tcpchar szChar);
	tstring GetStrRight(UNINT nCount);
	bool Exist( LPCTSTR szFind );
	CPiString& ToLower();
	CPiString& ToUpper();
	bool EqualNoCase(tcpchar szStr);
	int ToInt();
	operator tcpchar();
	CPiString operator +(tcpchar szData);
	void AppendZero(int nCount);
	int findFirstOf(tcpchar szChar);
private:
	//tstring		m_strData;
};
Pi_NameSpace_End