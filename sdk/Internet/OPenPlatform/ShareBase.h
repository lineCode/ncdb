#pragma once

#include "explorer1.h"

// CShareBase 对话框

class CShareBase : public CDialogEx
{
	DECLARE_DYNAMIC(CShareBase)

public:
	CShareBase(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShareBase();

// 对话框数据
	enum { IDD = IDD_SHAREBASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    bool Start();
protected:
    CWnd* m_pWndParent;
    CExplorer1 m_webTest;
    
};
