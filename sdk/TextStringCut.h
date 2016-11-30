#pragma once
#include "PiTypeDef.h"
class CTextStringCut
{
public:
	CTextStringCut(tcpchar val);
    CTextStringCut(const tstring& val);
    CTextStringCut(void);
    ~CTextStringCut(void);
public:

	bool GetInDQM(tstring& strDist);           //双引号
	bool GetInDQM(ARR_STRING& arrString);           //双引号
    bool GetInSQM(tstring strDist);           //单引号
	bool GetContent(tcpchar szAnchorLeft, tcpchar szAnchorRight, tstring& strDist);           //单引号
    bool GetContent(tcpchar szAnchorLeft, tcpchar szAnchorRight, int& Dist);           //单引号

    tstring Text() const { return m_strText; }
    void Text(tcpchar val) { m_strText = val; }
//protected:
    void InitVariable();
    bool GoForward( tcpchar szSignAnPostLink );
    /************************************************************************
        fun:	获取封闭的角括号中的值, ><
        param:  
        ret:    
        remark: 如<>abc<>  , 获取 abc
    ************************************************************************/
	bool GetInAngleBracketsClose( tstring& strDist );
    /************************************************************************
    fun:	到换行符, \n
    param:  
    ret:    
    remark: 
/************************************************************************/
    bool GoForwardLineEnd();
	void SetText( tcpchar param1 );
private:
    tstring             m_strText;
    
    int                 m_nPosCurrent;
};