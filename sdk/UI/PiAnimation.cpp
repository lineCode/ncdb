#include "StdAfx.h"
#include "PiAnimation.h"
#include "ImageEx.h"
#include "define_gnl.h"
#include "StlSupply.h"
#include "PathLight.h"
#include "Lock.h"
//#include "RAIILock.h"
#include "EventLight.h"
#include "ResCommon.h"
#include "functional.h"
using namespace ns_PiPi;

CPiAnimation::CPiAnimation(void)
{
	m_bIsGif = false;
	m_nCurrentPic = 0;
	m_pGif = NULL;
	m_bFrameLast = false;
	m_ptDraw.x = 0;
	m_ptDraw.y = 0;

}

CPiAnimation::~CPiAnimation(void)
{
	UnInit();
}

bool CPiAnimation::Init()
{
	CThreadLight::Init();
	SetWaitThreadTimeout(1000);
	CThreadLight::SetThreadName(_T("Animation"));
	m_pLockGroup = auto_ptr<CLock>(new CLock);
	m_pEventDrawIdle = auto_ptr<CEventLight>(new CEventLight);
	return  m_pEventDrawIdle->Init();
}

bool CPiAnimation::UnInit()
{
	CThreadLight::UnInit();
	ClearMem(m_pGif);
	return true;
}

bool CPiAnimation::ShowNextFrame( HDC hdcUI )
{
	if (!hdcUI)
	{
		return false;
	}
	m_hdcUI = hdcUI;
	
	//多张静态图绘制

	//CRAIILock raii(m_pLockGroup->Lock());
	m_pLockGroup->Lock();

	if (!m_lstFrameGroupCurrent || !m_lstFrameGroupCurrent->lstFrame.size())
	{
		m_pLockGroup->UnLock();
		return false;
	}
	if(m_lstFrameGroupCurrent->bGif)
	{
		//gif绘制
		DrawGif();
	}
	else
	{
		//多张静态
		DrawMultiPic();
	}
	
	//OutInfo(_T("动画一轮绘制完"));
	m_pLockGroup->UnLock();
	
	return true;
}

void CPiAnimation::SetImageDir( LPCTSTR param1 )
{
	m_strDirPic = param1;
}

bool CPiAnimation::SetImage( LPCTSTR param1 )
{
	assert(0);
	m_pGif = new ImageEx(param1);
	if(m_pGif->IsPicOk())
	{
		return false;
	}
	m_strPicGif = param1;
	m_bIsGif = true;
	return true;
}

int CPiAnimation::NextFrame()
{
	//CRAIILock raii(m_pLockGroup->Lock());
	m_pLockGroup->Lock();
	int nFramePrevious = m_nCurrentPic++;
	if (m_lstFrameGroupCurrent->lstFrame.size() == m_nCurrentPic)
	{
		m_nCurrentPic = 0;
		m_bFrameLast = true;
	}
	m_pLockGroup->UnLock();
	return nFramePrevious;
}

DWORD CPiAnimation::DoWork()
{
	if (!m_lstFrameGroupCurrent)
	{
		m_lstFrameGroupCurrent = &*m_lstFrameGroup.begin();
	}

	//界面发送消息来绘制
	while(1)
	{
		/*LST_INT::iterator itRate = m_lstFrameRate.begin();
		int nRateIndex = m_nCurrentPic;
		advance(itRate, nRateIndex);*/
		//CRAIILock raii(m_pLockGroup->Lock());
		m_pLockGroup->Lock();
		UNINT nSleep = GetFrameSleep();
		m_pLockGroup->UnLock();

		::SendMessage(m_hWnd, m_nMsg, 0, 0);
		bool bStopLoop = IsNeedStopLoop();

		if (bStopLoop)
		{
			nSleep = INFINITE;
			m_bFrameLast = false;
		}
		m_pEventDrawIdle->GetLock(nSleep);	//超时或者有外部事件中断
		if (CThreadLight::IsWillStopThread())
		{
			break;
		}
		//WaitTimer(nSleep);	
	}
	
	return 1;
}

void CPiAnimation::SetMsgID( int nMsg )
{
	m_nMsg = nMsg;
}

void CPiAnimation::SetImage( LST_STRING lstPic )
{
	m_lstPic.clear();
	FOR_EACH_ITER(LST_STRING, lstPic, it)
	{
		m_lstPic.push_back(*it);
	}
	m_bIsGif = false;
}

bool CPiAnimation::SetFrameRate( LST_INT lstRate )
{
	m_lstFrameRate = lstRate;
	return true;
}


bool CPiAnimation::AddGroup( const tagFRAME_GROUP& fGroup )
{
	m_lstFrameGroup.push_back(fGroup);
	return true;
}

void CPiAnimation::SetPlayGroup( GroupID id )
{
	
	//CRAIILock raii(m_pLockGroup->Lock());
	m_pLockGroup->Lock();
	OutInfo(_T("即将设置"));
	m_lstFrameGroupCurrent = NULL;
	FOR_EACH_ITER(LST_FRAME_GROUP, m_lstFrameGroup, it)
	{
		if (id != it->nIDGroup)
		{
			continue;
		}
		m_lstFrameGroupCurrent = &*it;
		m_nCurrentPic = 0;
		m_bFrameLast = false;
		if (m_bIsGif = m_lstFrameGroupCurrent->bGif)
		{
			if(!InitGif())
			{
				m_lstFrameGroupCurrent = NULL;
				OutInfo(_T("加载新的挂件gif失败"));
				break;
			}
			
		}
		OutInfo(_T("设置完成"));
		m_pEventDrawIdle->SetEventSign();
		break;
	}
	m_pLockGroup->UnLock();
}

bool CPiAnimation::IsNeedStopLoop()
{
	//CRAIILock raii(m_pLockGroup->Lock());
	m_pLockGroup->Lock();
	
	bool bRet = false;
	if (! m_lstFrameGroupCurrent)
	{
		return bRet;
	}
	if (m_lstFrameGroupCurrent->bGif)
	{
		bRet = false;
	}
	else
	{
		bRet = m_lstFrameGroupCurrent->lstFrame.size() <= 1 
			|| m_bFrameLast && !m_lstFrameGroupCurrent->bLoop;
	}
	m_pLockGroup->UnLock();

	return bRet;
}

bool CPiAnimation::SetDrawPos( const POINT& ptPos )
{
	m_ptDraw = ptPos;
	
	return true;
}

bool CPiAnimation::DrawMultiPic()
{
	CPathLight strPath(m_strDirPic);
	//strPath += m_lstPic[NextFrame()];
	strPath += GetListElem(m_lstFrameGroupCurrent->lstFrame, NextFrame()).strPicPath;
	Image img(strPath);
	if (img.GetLastStatus() != Ok)
	{
		OutInfo(_T("图片加载失败"));
		return false;
	}

	Graphics grp(m_hdcUI);
	if(grp.DrawImage(&img, m_ptDraw.x, m_ptDraw.y, img.GetWidth(), img.GetHeight()) != Ok)
	{
		OutInfo(_T("err dr"));
		m_pLockGroup->UnLock();
		return false;
	}
	//grp.DrawImage(&img, 0, 0, 50, 66);
	return true;
}

bool CPiAnimation::DrawGif()
{
	m_pGif->ShowNextFrame(m_hdcUI);
	
	return true;
}

bool CPiAnimation::InitGif()
{

	ClearMem(m_pGif);
	m_pGif = ::new ImageEx(m_strDirPic + m_lstFrameGroupCurrent->lstFrame.front().strPicPath.c_str());
	//ClearMem(m_pGif);
	if( ! m_pGif->IsPicOk() )
	{
		return false;
	}
	m_pGif->SetDrawPoint(m_ptDraw);
	return true;
}

UNINT CPiAnimation::GetFrameSleep()
{
	UNINT nSleep = 1000;
	if (m_lstFrameGroupCurrent->bGif)
	{
		nSleep = m_pGif->GetFrameDelay(m_pGif->GetCurrentFramePosition());
	}
	else
	{
		nSleep = GetListElem(m_lstFrameGroupCurrent->lstFrame, m_nCurrentPic).nFrameRate;
	}
	return nSleep;
}
