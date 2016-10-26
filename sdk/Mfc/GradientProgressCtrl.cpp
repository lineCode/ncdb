// GradientProgressCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "GradientProgressCtrl.h"
#include "ColorShade.h"
#include "ResCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGradientProgressCtrl

CGradientProgressCtrl::CGradientProgressCtrl()
{
    m_nLower        = 0;
    m_nUpper        = 100;
    m_nCurrentPosition = 0;
    m_nStep = 10;

    //Initial Color of show

    m_clrText = COLORREF(RGB(0, 0, 0));
    
    m_pdcBg             = nullptr;
    m_bgOldObj          = nullptr;
    m_bInit             = false;
    m_nBmIDDone         = 0;
    m_nBmIDUnDone       = 0;
    //Not show Word
    m_bShowPercent = TRUE;
}

CGradientProgressCtrl::~CGradientProgressCtrl()
{
}


BEGIN_MESSAGE_MAP(CGradientProgressCtrl, CProgressCtrl)
    //{{AFX_MSG_MAP(CGradientProgressCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradientProgressCtrl message handlers

BOOL CGradientProgressCtrl::Init()
{
    if (m_nBmIDDone)
    {
        CBitmap bm;
        bm.LoadBitmap(m_nBmIDDone);
        BITMAP tagbm = {0};
        bm.GetBitmap(&tagbm);
        m_rectClient.bottom = tagbm.bmHeight;
        m_rectClient.right = tagbm.bmWidth;
    }

    
    CDC* pdcUI = GetDC();
    m_pdcBg = new CDC;
    if( !m_pdcBg->CreateCompatibleDC(pdcUI) )
    {
        return FALSE;
    }

    if( ! m_bm.CreateCompatibleBitmap(pdcUI, m_rectClient.right, m_rectClient.bottom) )
    {
        return FALSE;
    }
    ReleaseDC(pdcUI);

    
    m_bgOldObj = m_pdcBg->SelectObject(m_bm);
    //cs.SetDC(pdcClient->m_hDC);

    return TRUE;
}

BOOL CGradientProgressCtrl::InitBg()
{
    /*this->ModifyStyle(WS_BORDER | WS_THICKFRAME, 0);
    this->ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 0);*/

    m_csDone.SetPic(m_nBmIDDone);
    m_csDone.SetBgDC(m_pdcBg);
    m_csDone.SetRect(m_rectClient);
    m_csDone.SetHwnd(this);
    m_csDone.InitBg();

    
    m_csUnDone.SetPic(m_nBmIDUnDone);
    m_csUnDone.SetBgDC(m_pdcBg);
    m_csUnDone.SetRect(m_rectClient);
    m_csUnDone.SetHwnd(this);
    m_csUnDone.InitBg();
    
    return TRUE;
}

void CGradientProgressCtrl::SetRange(int nLower,int nUpper)
{
    //This Function is to Set Range of the progress 
    m_nLower            = nLower;
    m_nUpper            = nUpper;
    m_nCurrentPosition  = nLower;

    
    CProgressCtrl::SetRange(nLower,nUpper);
}

int CGradientProgressCtrl::SetStep(int nStep)
{
    m_nStep=nStep;
    return (CProgressCtrl::SetStep(nStep));
}

BOOL CGradientProgressCtrl::OnEraseBkgnd(CDC* pDC) 
{
    // TODO: Add your message handler code here and/or call default
    //return TRUE;
    return CProgressCtrl::OnEraseBkgnd(pDC);
    //return TRUE;//CProgressCtrl::OnEraseBkgnd(pDC);
}

void CGradientProgressCtrl::DrawProgress(CPaintDC *pDC, const RECT &rectClient, const int &nMaxWidth)
{
    //根据nMaxWidth 从背景dc里贴出相应宽度的图片

    //先绘制未完成, 在画已完成, 覆盖上去
    CRect rt(rectClient);
    m_csUnDone.Show(rt);
    //ShowUI();

    rt.right = nMaxWidth;
    m_csDone.Show(rt);
    //ShowUI();


    if(m_bShowPercent)
    {
        CString percent;
        percent.Format(_T("%d%%"),(int)(100*(float)m_nCurrentPosition/m_nUpper));
        m_pdcBg->SetTextColor(m_clrText);
        m_pdcBg->SetBkMode(TRANSPARENT);
        m_pdcBg->DrawText(percent,&m_rectClient,DT_VCENTER|DT_CENTER|DT_SINGLELINE);
        ShowUI();

    }

    return ;
}


void CGradientProgressCtrl::OnPaint() 
{
    //CProgressCtrl::OnPaint();
    Invalidate();

    CPaintDC dc(this); // device context for painting
    //dc.
    // TODO: Add your message handler code here
    if( m_nCurrentPosition < m_nLower || m_nCurrentPosition >= m_nUpper )
    {
        m_nCurrentPosition = 0;
        /*CBrush brush;
        brush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
        //dc.FillRect(&m_rectClient,&brush);
        VERIFY(brush.DeleteObject());
        return;*/
    }

    float maxWidth((float)m_nCurrentPosition/(float)m_nUpper*(float)m_rectClient.right);//当前显示多长位图
    //绘制
    this->DrawProgress(NULL, m_rectClient,(int)maxWidth);

    // Do not call CProgressCtrl::OnPaint() for painting messages
}

int CGradientProgressCtrl::SetPos(int nPos)
{
    //Set the Position of the Progress
    m_nCurrentPosition=nPos;
    return (CProgressCtrl::SetPos(nPos));
}


BOOL CGradientProgressCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: 在此添加专用代码和/或调用基类
    cs.style &= ~(WS_BORDER | WS_THICKFRAME);
    return CProgressCtrl::PreCreateWindow(cs);
}


BOOL CGradientProgressCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
    // TODO: 在此添加专用代码和/或调用基类

    return CProgressCtrl::Create(dwStyle, rect, pParentWnd, nID);
}

bool CGradientProgressCtrl::UnInit()
{
    m_bgOldObj = m_pdcBg->SelectObject(m_bgOldObj);
    ClearMem(m_pdcBg);
    return true;
}

void CGradientProgressCtrl::ShowUI()
{
    CDC* pdc = GetDC();
    if(!pdc->BitBlt(0, 0, m_rectClient.right, m_rectClient.bottom, m_pdcBg, 0, 0, SRCCOPY))
    {
        return ;
    }
    this->ReleaseDC(pdc);
}
