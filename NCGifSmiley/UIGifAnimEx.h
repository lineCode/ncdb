#ifndef GifAnimUIEx_h__
#define GifAnimUIEx_h__

#pragma once
class CControl;

#define NORMAL_BORDER_COLOR      0xFFCCCCCC              //普通状态边框颜色
#define HOT_BORDER_COLOR         0xFF00B3F3              //焦点状态边框颜色
#define TOP_DISTANCE_GIF         4                       //表情容器距离顶部距离
#define BOTTOM_DISTANCE_GIF      40                      //表情容器距离底部距离
#define HEIGHT_RECT              60                      //矩形网格高度
#define WIDTH_RECT               60                      //矩形网格宽度
#define DUI_MSGTYPE_GIF_CLICK                  (_T("gifclick"))
#define DUI_MSGTYPE_GIF_MENU                   (_T("gifmenu"))
class CGifAnimUIEx : public CGifAnimUI
{
	DECLARE_DUICONTROL(CGifAnimUIEx)
public:
	CGifAnimUIEx(void);
	~CGifAnimUIEx(void);

	void	DoEvent(TEventUI& event) override;
	void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
	void	DoPaint(HDC hDC, const RECT& rcPaint) override;
	void	DoInit() override;
private:
	void    OnLineTimer(UINT_PTR idEvent);
	Gdiplus::Image*	LoadGifFromFile(LPCTSTR pstrGifPath);
	Gdiplus::Image* LoadGifFromMemory(LPVOID pBuf, size_t dwSize);
	Gdiplus::Image	*m_pGifImage;

	bool smallGif;
public:
	int gifHeight;
	int gifWidth;
};

#endif // GifAnimUI_h__
