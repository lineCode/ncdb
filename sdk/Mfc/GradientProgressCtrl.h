#if !defined(AFX_GRADIENTPROGRESSCTRL_H__91ECB8BC_A7DA_42EB_B2C5_37419D31B753__INCLUDED_)
#define AFX_GRADIENTPROGRESSCTRL_H__91ECB8BC_A7DA_42EB_B2C5_37419D31B753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GradientProgressCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGradientProgressCtrl window
#include "ColorShade.h"

class CGradientProgressCtrl : public CProgressCtrl
{
// Construction
public:
	CGradientProgressCtrl();

	//Set_function
	void SetTextColor(COLORREF color)	{ m_clrText = color; }
    void SetBkColorStart(COLORREF color)		{ m_csUnDone.SetStartColor(color);}
	void SetBkColorEnd(COLORREF color)		{ m_csUnDone.SetEndColor(color); }
    

	void SetStartColor(COLORREF color)	{ m_csDone.SetStartColor(color); }
	void SetEndColor(COLORREF color)	{ m_csDone.SetEndColor(color); }
	//Percent TXT showed in the Progress 
	void ShowPercent(BOOL bShowPercent=TRUE)	{m_bShowPercent=bShowPercent;}
    void SetPicDone(int nPicID) { m_nBmIDDone = nPicID;}
    void SetPicUnDone(int nPicID) { m_nBmIDUnDone = nPicID;}
    //bool SetUnDonePic(int nPicID) { m_nBmIDDone = nPicID;}

	//Get_Function
	/*COLORREF GetTextColor(void)		{return m_clrText;}
	COLORREF GetBkColor(void)		{return m_clrBkGround;}
	COLORREF GetStartColor(void)	{return m_clrStart;}
	COLORREF GetEndColor(void)		{return m_clrEnd;}*/

    BOOL Init();
    BOOL InitBg();

// Attributes
public:
	int			m_nLower,m_nUpper,m_nStep,m_nCurrentPosition;
	//COLORREF	m_clrStart,m_clrEnd,m_clrBkGround;
    COLORREF	m_clrText;
	BOOL		m_bShowPercent;

    CDC*        m_pdcBg;
    CBitmap     m_bm;
    CColorShade m_csDone;
    CColorShade m_csUnDone;

    CRect       m_rectClient;
    bool        m_bInit;
    HGDIOBJ     m_bgOldObj;
    int         m_nBmIDDone;
    int         m_nBmIDUnDone;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradientProgressCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	int SetPos(int nPos);
	int	 SetStep(int nStep);
	void SetRange(int nLower,int nUpper);
	virtual ~CGradientProgressCtrl();

	// Generated message map functions
protected:
	void DrawProgress(CPaintDC *pDC,const RECT& rectClient,const int & nMaxWidth);
	//{{AFX_MSG(CGradientProgressCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

    afx_msg void ShowUI();

    //}}AFX_MSG

	DECLARE_MESSAGE_MAP()
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
    bool UnInit();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADIENTPROGRESSCTRL_H__91ECB8BC_A7DA_42EB_B2C5_37419D31B753__INCLUDED_)
