// ShareBase.cpp : 实现文件
//

#include "stdafx.h"
#include "PA_test.h"
#include "ShareBase.h"
#include "afxdialogex.h"


// CShareBase 对话框

IMPLEMENT_DYNAMIC(CShareBase, CDialogEx)

CShareBase::CShareBase(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShareBase::IDD, pParent)
{

}

CShareBase::~CShareBase()
{
}

void CShareBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShareBase, CDialogEx)
END_MESSAGE_MAP()

bool CShareBase::Start()
{
    return true;
}


// CShareBase 消息处理程序
