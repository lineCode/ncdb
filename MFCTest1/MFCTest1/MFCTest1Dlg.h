
// MFCTest1Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "ui/PiDropTarget.h"
#include "MyDropTarget.h"
#include "ui/PiDataSource.h"


// CMFCTest1Dlg 对话框
class CMFCTest1Dlg : public CDialogEx
{
// 构造
public:
	CMFCTest1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCTEST1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	void TestOleDrag();
	CButton m_btn1;

private:
	COleDataSource m_oleDataSource;
	bool			m_bDraging;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindragFilelist(NMHDR* pNMHDR, LRESULT* pResult);

	bool BeginDrag();
	void DragIng();
	void DragIng2();
	void DragDui();

	bool				m_bBtnDown;
	//CPiDropTarget		m_DropTarget;
	CListCtrl			c_FileList;
	CPiDataSource		m_idSource;
	CMyDropTarget		m_droptarget;
};
