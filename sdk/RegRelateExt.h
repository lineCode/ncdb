/*------------------------------------------------------
   	Fun:	文件关联类
   	Authon: liwanliang
   	Date:	2015/09/30
   ------------------------------------------------------*/
   
 #if !defined(AFX_REGRELATEEXT_H__6DE7E979_9011_4639_967A_D8817DF4DE44__INCLUDED_) 
 #define AFX_REGRELATEEXT_H__6DE7E979_9011_4639_967A_D8817DF4DE44__INCLUDED_ 
 #include "ATLBASE.h" 
   
 #if _MSC_VER > 1000 
 #pragma once 
 #endif // _MSC_VER > 1000 
#include "PiString.h"
class CRegRelateExt : public CRegKey
{ 
public: 
	typedef ns_PiPi::CPiString CPiString;
	CRegRelateExt(); 
	virtual ~CRegRelateExt(); 
public:
	/*------------------------------------------------------------
			fun:	检查制定后缀名是否关联到指定程序
			ret:	true: 已关联
			param: 
			remark:	没有检查关联图标
	------------------------------------------------------------*/
	static bool Check(tcpchar szFileExt, tcpchar szExePath, tcpchar szParam); 
	static bool CheckSelf(tcpchar szFileExt, tcpchar szParam); 
	/*------------------------------------------------------------
			fun:	设置文件关联
			ret:	
			param: 1. fileExe, 
			remark:	
	------------------------------------------------------------*/
	bool Set(tcpchar szFileExt, tcpchar szRelateExe, tcpchar szShellParam = NULL, tcpchar szIcon = NULL); 
	bool SetSelf(tcpchar szFileExt, tcpchar szShellParam = NULL, tcpchar szIcon = NULL); 
	void SetParam(tcpchar szRegClassName, tcpchar szArchiveDesc);
	static bool Delete(tcpchar FileExt); 
protected:
	static tstring MakeShellCmd( tcpchar szRelateExe, tcpchar szParam );
private: 
	//BOOL m_IfRelated; 

	CPiString m_RegClass; 
	//CPiString m_RelateExe; 
	CPiString m_strArchiveDesc;
	 
}; 
   
 #endif // !defined(AFX_REGRELATEEXT_H__6DE7E979_9011_4639_967A_D8817DF4DE44__INCLUDED_) 

