#pragma once

class CColorShade
{
public:
    CColorShade(void);
    ~CColorShade(void);
    bool Init();
    bool UnInit();
    void SetRect( CRect m_rectClient );
    void SetHwnd( CWnd* pWnd ) { m_pWnd = pWnd; };
    void SetBgDC( CDC* pdc ) { m_pDCDist = pdc; }

    

    bool InitBg();

    void InitBgShade();

    bool Show(const CRect& rt);

    void SetStartColor(COLORREF color)	{m_clrStart=color;}
    void SetEndColor(COLORREF color)	{m_clrEnd=color;}

    void SetPic(int nPicID) { m_nBmID = nPicID;}
    

    bool InitBgPic();
    void ShowUI();
private:
	COLORREF	m_clrStart, m_clrEnd;
    CWnd*       m_pWnd;

    CDC*        m_pdcBg;
    CDC*        m_pDCDist;  //绘制目标位图, 可能为内存dc
    //CDC*        m_pDCSrc;   //设备dc, 用户创建多色位图

    CBitmap     m_bm;

    CRect       m_rectUI;
    bool        m_bInit;
    HGDIOBJ     m_bgOldObj;

    int         m_nBmID;
};

