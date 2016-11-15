#ifndef GifAnimUIEx_h__
#define GifAnimUIEx_h__

#pragma once
class CControl;

#define ENTER_TIMER              UIMSG_USER+1
#define EVENT_TIEM_ID	100
#define NORMAL_BORDER_COLOR      0xFFCCCCCC              //普通状态边框颜色
#define HOT_BORDER_COLOR         0xFF00B3F3              //焦点状态边框颜色
#define TOP_DISTANCE_GIF         4                       //表情容器距离顶部距离
#define BOTTOM_DISTANCE_GIF      40                      //表情容器距离底部距离
#define HEIGHT_RECT              60                      //矩形网格高度
#define WIDTH_RECT               60                      //矩形网格宽度
#define DUI_MSGTYPE_GIF_CLICK                  (_T("gifclick"))
#define DUI_MSGTYPE_GIF_MENU                   (_T("gifmenu"))
class CGifAnimUIEx : public CControlUI
{
	DECLARE_DUICONTROL(CGifAnimUIEx)
public:
	CGifAnimUIEx(void);
	~CGifAnimUIEx(void);

	void	DoEvent(TEventUI& event) override;
	void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
	void	DoPaint(HDC hDC, const RECT& rcPaint) override;
	void	DoInit() override;
	void	SetVisible(bool bVisible = true) override;

	void	SetBkImage(LPCTSTR pStrImage);
	LPCTSTR GetBkImage();

	void	SetAutoPlay(bool bIsAuto = true);
	bool	IsAutoPlay() const;
	void	SetAutoSize(bool bIsAuto = true);
	bool	IsAutoSize() const;
	void	PlayGif();
	void	PauseGif();
	void	StopGif();
	
private:
	void	InitGifImage();
	void	DrawFrame(HDC hDC);		// 绘制GIF每帧
	void    OnMouseInTimer(UINT_PTR idEvent);
	Gdiplus::Image*	LoadGifFromFile(LPCTSTR pstrGifPath);
	Gdiplus::Image* LoadGifFromMemory(LPVOID pBuf, size_t dwSize);
	void	DeleteGif();
	void    OnTimer(UINT_PTR idEvent);
	//Gdiplus::Image	*m_pGifImage;

	bool smallGif;
public:
	int gifHeight;
	int gifWidth;

	Gdiplus::Image	*m_pGifImage;
	UINT			m_nFrameCount;				// gif图片总帧数
	UINT			m_nFramePosition;			// 当前放到第几帧
	Gdiplus::PropertyItem*	m_pPropertyItem;	// 帧与帧之间间隔时间

	CDuiString		m_sBkImage;
	bool			m_bIsAutoPlay;				// 是否自动播放gif
	bool			m_bIsAutoSize;				// 是否自动根据图片设置大小
	bool			m_bIsPlaying;
	HGLOBAL hMem;
	IStream* pStm;
};

#endif // GifAnimUI_h__
