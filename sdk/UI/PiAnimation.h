#pragma once
#include "PiTypeDef.h"
#include "ThreadLight.h"
#include "PathLight.h"
/************************************************************************
	fun: 异形窗口的动画类, 也可支持单张静态图片
	remark:	线程用于定时发送消息给窗口来实现绘制, 窗口通过调用该类的ShowNextFrame实现绘制
************************************************************************/
class ImageEx;


struct tagANIMATION_FRAME
{
	tagANIMATION_FRAME(tcpchar szPath, int nRate)
	{
		strPicPath = szPath;
		nFrameRate = nRate;
	}
	tstring		strPicPath;
	int			nFrameRate;
};

typedef list<tagANIMATION_FRAME> LST_FRAME;
typedef int GroupID;
struct tagFRAME_GROUP
{
public:
	tagFRAME_GROUP()
	{
		Clear();
	}
	void Clear()
	{
		nIDGroup = 0;
		bLoop = true;
		bGif = false;
		lstFrame.clear();
	}
	GroupID		nIDGroup;
	LST_FRAME	lstFrame;
	bool		bLoop;		//动画全部播放完后是否循环播放
	bool		bGif;		//是否为gif图片
};
typedef list<tagFRAME_GROUP>	LST_FRAME_GROUP;


class CPiAnimation
	:public CThreadLight
{
public:
	CPiAnimation(void);
	virtual ~CPiAnimation(void);
public:
	bool Init();
	bool UnInit();
	virtual DWORD DoWork() ;

	UNINT GetFrameSleep();

public:
	bool ShowNextFrame( HDC hdcUI );

	bool DrawGif();
	bool DrawMultiPic();

	void SetImageDir( LPCTSTR param1 );
	bool SetImage( LPCTSTR param1 );
	void SetMsgID(int nMsg);
	
	//bool SetFrame(LST_FRAME lstFrame);
	int NextFrame();
	HWND GetWnd() const { return m_hWnd; }
	void SetWnd(HWND val) { m_hWnd = val; }
	bool AddGroup( const tagFRAME_GROUP& fGroup );
	void SetPlayGroup( GroupID id);
	bool SetDrawPos(const POINT& ptPos);
private:
	void SetImage( LST_STRING lstPic );
	bool SetFrameRate(LST_INT lstRate);
	bool IsNeedStopLoop();
	bool InitGif();
public:
	ImageEx*			m_pGif;
	tstring				m_strPicGif;
	vector<tstring>		m_lstPic;
	LST_INT				m_lstFrameRate;
	tagFRAME_GROUP*			m_lstFrameGroupCurrent;	//当前组的动画, ref

	LST_FRAME_GROUP		m_lstFrameGroup;
	auto_ptr<CLock>				m_pLockGroup;
	auto_ptr<CEventLight>		m_pEventDrawIdle;
	int					m_nCurrentPic;

	bool				m_bIsGif;
	CPathLight				m_strDirPic;	

	HWND				m_hWnd;
	HDC					m_hdcUI;		//绘制的dc
	int					m_nMsg;			//绘制动画的消息
	bool				m_bFrameLast;	//本组动画已到最后一帧

	POINT				m_ptDraw;		//绘制的位置
};
