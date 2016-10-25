#include "StdAfx.h"
#include "TextStringCut.h"
#include "doString.h"
#include "define_gnl.h"
#include <AutoType.h>

static const tcpchar STRING_DQM_STR         = _T("\"");


CTextStringCut::CTextStringCut(void)
{
    InitVariable();
}

CTextStringCut::CTextStringCut( tcpchar val )
{
    InitVariable();
    m_strText = val;
}

CTextStringCut::CTextStringCut( const tstring& val )
{
	InitVariable();
	m_strText = val;
}


void CTextStringCut::InitVariable()
{
    m_nPosCurrent           = -1;
    
}

CTextStringCut::~CTextStringCut(void)
{
}

bool CTextStringCut::GetInDQM( tstring& strDist )
{
    m_nPosCurrent = ::GetContentBetweenChar(m_strText, strDist, STRING_DQM_STR, 1, NextPos(m_nPosCurrent));   //获取帖子链接
    return m_nPosCurrent != tstring::npos;
}

bool CTextStringCut::GoForward( tcpchar szSignAnPostLink )
{
    int nPos = GetCharPos(m_strText, szSignAnPostLink, 1, NextPos(m_nPosCurrent)); //定位到作者首页url标识
    bool bValid = IsValidStrPos(nPos);
    if (bValid)
    {
        m_nPosCurrent = nPos;
    }
    
    return bValid;
    
}

bool CTextStringCut::GetInAngleBracketsClose( tstring& strDist )
{
    m_nPosCurrent = GetContentBetweenTwoChar(m_strText, strDist, _T(">"), _T("<"), 1, NextPos(m_nPosCurrent)); //作者首页url
    return m_nPosCurrent != tstring::npos;

}

bool CTextStringCut::GoForwardLineEnd()
{
    return GoForward(_T("\n"));
}

bool CTextStringCut::GetContent( tcpchar szAnchorLeft, tcpchar szAnchorRight, tstring& strDist)
{
    m_nPosCurrent = GetContentBetweenTwoChar(m_strText, strDist, szAnchorLeft, szAnchorRight
        , 1, NextPos(m_nPosCurrent)); //作者首页url
    return IsValidStrPos(m_nPosCurrent);    
}

bool CTextStringCut::GetContent( tcpchar szAnchorLeft, tcpchar szAnchorRight, int& Dist )
{
	tstring strData;
	if(!GetContent(szAnchorLeft, szAnchorRight, strData))
	{
		return false;
	}
	Dist = CAutoType(strData);
	return true;
}

void CTextStringCut::SetText( tcpchar param1 )
{
	InitVariable();
	m_strText = param1;
}
