#include "StdAfx.h"
#include "UIGifAnimEx.h"

///////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DUICONTROL(CGifAnimUIEx)

CGifAnimUIEx::CGifAnimUIEx(void)
{
	m_pGifImage = NULL;
	gifHeight = 0;
	gifWidth = 0;

	m_pPropertyItem = NULL;
	m_nFrameCount = 0;
	m_nFramePosition = 0;
	m_bIsAutoPlay = true;
	m_bIsAutoSize = false;
	m_bIsPlaying = false;
	pStm = NULL;
}


CGifAnimUIEx::~CGifAnimUIEx(void)
{
	DeleteGif();
	m_pManager->KillTimer(this, EVENT_TIEM_ID);
}

void CGifAnimUIEx::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_MOUSEENTER)
	{
		if (IsEnabled() && gifHeight > 0 && gifWidth > 0) {
			//LOG(INFO) << "*************:mouse in";
			RECT rc = GetPos();
			rc.bottom += 2;
			rc.left -= 2;
			rc.right += 2;
			rc.top -= 2;
			SIZE sizeClient = GetManager()->GetClientSize();                      //获取窗口大小
			HDC hdc = this->GetManager()->GetPaintDC();                           //获取窗口HDC
			
			if (gifHeight < HEIGHT_RECT || gifWidth < WIDTH_RECT){
				rc.left -= (WIDTH_RECT - gifWidth) / 2-1;
				rc.top -= (HEIGHT_RECT - gifHeight) / 2-1;
				rc.right += (WIDTH_RECT - gifWidth) / 2+1;
				rc.bottom += (HEIGHT_RECT - gifHeight) / 2+1;
			}
			if (rc.bottom > sizeClient.cy - BOTTOM_DISTANCE_GIF){
				rc.bottom = sizeClient.cy - BOTTOM_DISTANCE_GIF;

				RECT rcBorder;
				rcBorder = rc;
				rcBorder.right = rcBorder.left;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, HOT_BORDER_COLOR, m_nBorderStyle);

				rcBorder = rc;
				rcBorder.bottom = rcBorder.top;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, HOT_BORDER_COLOR, m_nBorderStyle);

				rcBorder = rc;
				rcBorder.right -= 1;
				rcBorder.left = rcBorder.right;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, HOT_BORDER_COLOR, m_nBorderStyle);
			}
			else if (rc.top < TOP_DISTANCE_GIF){
				rc.top = TOP_DISTANCE_GIF;
				RECT rcBorder;

				rcBorder = rc;
				rcBorder.right = rcBorder.left;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, HOT_BORDER_COLOR, m_nBorderStyle);

				rcBorder = rc;
				rcBorder.right -= 1;
				rcBorder.left = rcBorder.right;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, HOT_BORDER_COLOR, m_nBorderStyle);

				rcBorder = rc;
				rcBorder.bottom -= 1;
				rcBorder.top = rcBorder.bottom;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, HOT_BORDER_COLOR, m_nBorderStyle);
			}
			else{
			//	LOG(INFO) << "rc.left:" << rc.left << "  rc.top:" << rc.top << "  rc.right:" << rc.right << "  rc.bottom:" << rc.bottom;
				CRenderEngine::DrawRect(hdc, rc, 1, HOT_BORDER_COLOR, m_nBorderStyle);
				if (gifHeight >= HEIGHT_RECT || gifWidth >= WIDTH_RECT)
					m_pManager->SetTimer(this, ENTER_TIMER, 100);
			}
			PlayGif();
		}
	}
	if (event.Type == UIEVENT_MOUSELEAVE){
		if (IsEnabled() && gifHeight > 0 && gifWidth > 0) {
			//LOG(INFO) << "+++++++++++++:mouse leave";
			RECT rc = GetPos();
			rc.bottom += 2;
			rc.left -= 2;
			rc.right += 2;
			rc.top -= 2;
			SIZE sizeClient = GetManager()->GetClientSize();                      //获取窗口大小
			HDC hdc = this->GetManager()->GetPaintDC();

			if (gifHeight < HEIGHT_RECT || gifWidth < WIDTH_RECT){
				rc.left -= (WIDTH_RECT - gifWidth) / 2 -1;
				rc.top -= (HEIGHT_RECT - gifHeight) / 2 - 1;
				rc.right += (WIDTH_RECT - gifWidth) / 2 + 1;
				rc.bottom += (HEIGHT_RECT - gifHeight) / 2 + 1;
			}
			if (rc.bottom > sizeClient.cy - BOTTOM_DISTANCE_GIF){
				rc.bottom = sizeClient.cy - BOTTOM_DISTANCE_GIF;
				RECT rcBorder;

				rcBorder = rc;
				rcBorder.right = rcBorder.left;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, NORMAL_BORDER_COLOR, m_nBorderStyle);

				rcBorder = rc;
				rcBorder.bottom = rcBorder.top;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, NORMAL_BORDER_COLOR, m_nBorderStyle);

				rcBorder = rc;
				rcBorder.right -= 1;
				rcBorder.left = rcBorder.right;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, NORMAL_BORDER_COLOR, m_nBorderStyle);
			} 
			else if (rc.top < TOP_DISTANCE_GIF){
				rc.top = TOP_DISTANCE_GIF;
				RECT rcBorder;

				rcBorder = rc;
				rcBorder.right = rcBorder.left;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, NORMAL_BORDER_COLOR, m_nBorderStyle);

				rcBorder = rc;
				rcBorder.right -= 1;
				rcBorder.left = rcBorder.right;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, NORMAL_BORDER_COLOR, m_nBorderStyle);

				rcBorder = rc;
				rcBorder.bottom -= 1;
				rcBorder.top = rcBorder.bottom;
				CRenderEngine::DrawLine(hdc, rcBorder, 1, NORMAL_BORDER_COLOR, m_nBorderStyle);
			}
			else {
				CRenderEngine::DrawRect(hdc, rc, 1, NORMAL_BORDER_COLOR, m_nBorderStyle);
			}

			PlayGif();  //返回到第一帧状态
			StopGif();
		}
	}
	if (event.Type == UIEVENT_BUTTONUP){
		if (IsEnabled() && gifHeight > 0 && gifWidth > 0) {
			CDuiString path = GetBkImage();
			TNotifyUI msg;
			msg.sType = DUI_MSGTYPE_GIF_CLICK;
			msg.pSender = this;
			m_pManager->SendNotify(msg);
		}
	}
	if (event.Type == UIEVENT_RBUTTONUP){
		if (this->GetTag() == 1){
			TNotifyUI msg;
			msg.sType = DUI_MSGTYPE_GIF_MENU;
			msg.pSender = this;
			m_pManager->SendNotify(msg);
		}
	}
	if (event.Type == UIEVENT_TIMER)
	{
		if ((UINT_PTR)event.wParam == ENTER_TIMER)
			OnMouseInTimer((UINT_PTR)event.wParam);
		if ((UINT_PTR)event.wParam == EVENT_TIEM_ID)
			OnTimer((UINT_PTR)event.wParam);
	}
	//CControlUI::DoEvent(event);
}

void CGifAnimUIEx::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("bkimage")) == 0 ) SetBkImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("autoplay")) == 0 ) {
		SetAutoPlay(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("autosize")) == 0 ) {
		SetAutoSize(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else
		CControlUI::SetAttribute(pstrName, pstrValue);
}

void CGifAnimUIEx::DoPaint(HDC hDC, const RECT& rcPaint)
{
	//CGifAnimUI::DoPaint(hDC, rcPaint);	
	//if (!bFirstPaint){
	//	m_pManager->SetTimer(this, SHOWLINE_TIME_ID, 100);
	//	bFirstPaint = true;
	//}
	if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) return;
	if (NULL == m_pGifImage)
	{
		InitGifImage();
	}
	DrawFrame(hDC);
}

void CGifAnimUIEx::DoInit()
{
	//if (!m_pGifImage)
	//	m_pGifImage = LoadGifFromFile(GetBkImage());
	//if (m_pGifImage){
	//	gifHeight = m_pGifImage->GetHeight();
	//	gifWidth = m_pGifImage->GetWidth();
	//	if (gifWidth < WIDTH_RECT&&gifHeight<HEIGHT_RECT){
	//		RECT rcPadding;
	//		rcPadding.left = (WIDTH_RECT - gifWidth) / 2;
	//		rcPadding.top = (HEIGHT_RECT - gifHeight) / 2;
	//		SetPadding(rcPadding);
	//	}
	//	else
	//		SetAutoSize(false);
	//}
	//CGifAnimUI::DoInit();
	InitGifImage();
}

Gdiplus::Image* CGifAnimUIEx::LoadGifFromFile(LPCTSTR pstrGifPath)
{
	LPBYTE pData = NULL;
	DWORD dwSize = 0;

	do
	{
		CDuiString sFile = CPaintManagerUI::GetResourcePath();
		if (CPaintManagerUI::GetResourceZip().IsEmpty()) {
			sFile += pstrGifPath;
			HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
				FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) break;
			dwSize = ::GetFileSize(hFile, NULL);
			if (dwSize == 0) break;

			DWORD dwRead = 0;
			pData = new BYTE[dwSize];
			::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
			::CloseHandle(hFile);

			if (dwRead != dwSize) {
				delete[] pData;
				pData = NULL;
				break;
			}
		}

	} while (0);

	while (!pData)
	{
		//读不到图片, 则直接去读取bitmap.m_lpstr指向的路径
		HANDLE hFile = ::CreateFile(pstrGifPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) break;
		dwSize = ::GetFileSize(hFile, NULL);
		if (dwSize == 0) break;

		DWORD dwRead = 0;
		pData = new BYTE[dwSize];
		::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
		::CloseHandle(hFile);

		if (dwRead != dwSize) {
			delete[] pData;
			pData = NULL;
		}
		break;
	}
	if (!pData)
	{
		return NULL;
	}

	Gdiplus::Image* pImage = LoadGifFromMemory(pData, dwSize);
	delete[] pData;
	return pImage;
}


Gdiplus::Image* CGifAnimUIEx::LoadGifFromMemory(LPVOID pBuf, size_t dwSize)
{
	hMem = ::GlobalAlloc(GMEM_FIXED, dwSize);
	BYTE* pMem = (BYTE*)::GlobalLock(hMem);

	memcpy(pMem, pBuf, dwSize);

	//IStream* pStm = NULL;
	::CreateStreamOnHGlobal(hMem, TRUE, &pStm);
	Gdiplus::Image *pImg = Gdiplus::Image::FromStream(pStm);
	if (!pImg || pImg->GetLastStatus() != Gdiplus::Ok)
	{
		pStm->Release();
		::GlobalUnlock(hMem);
		return 0;
	}
	return pImg;
}
void CGifAnimUIEx::OnMouseInTimer(UINT_PTR idEvent)
{
	if (idEvent != ENTER_TIMER)
		return;
	m_pManager->KillTimer(this, ENTER_TIMER);

	CDuiPoint pt;
	::GetCursorPos(&pt);
	ScreenToClient(GetManager()->GetPaintWindow(), &pt);
	pt.x -= 4;
	pt.y -= 4;
	RECT rc = GetPos();

	TEventUI event = { 0 };
	if (pt.x > rc.left&&pt.x<rc.right&&pt.y>rc.top&&pt.y < rc.bottom){
		event.Type = UIEVENT_MOUSEENTER;
	}
	else
		event.Type = UIEVENT_MOUSELEAVE;
	this->Event(event);
}

void CGifAnimUIEx::InitGifImage()
{
	m_pGifImage = LoadGifFromFile(GetBkImage());
	if (NULL == m_pGifImage) return;
	UINT nCount = 0;
	nCount = m_pGifImage->GetFrameDimensionsCount();
	GUID* pDimensionIDs = new GUID[nCount];
	m_pGifImage->GetFrameDimensionsList(pDimensionIDs, nCount);
	m_nFrameCount = m_pGifImage->GetFrameCount(&pDimensionIDs[0]);
	int nSize = m_pGifImage->GetPropertyItemSize(PropertyTagFrameDelay);
	m_pPropertyItem = (Gdiplus::PropertyItem*) malloc(nSize);
	m_pGifImage->GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem);
	delete  pDimensionIDs;
	pDimensionIDs = NULL;

	if (m_pGifImage){
		gifHeight = m_pGifImage->GetHeight();
		gifWidth = m_pGifImage->GetWidth();
		if (gifWidth < WIDTH_RECT&&gifHeight<HEIGHT_RECT){
			RECT rcPadding;
			rcPadding.left = (WIDTH_RECT - gifWidth) / 2;
			rcPadding.top = (HEIGHT_RECT - gifHeight) / 2;
			SetPadding(rcPadding);
		}
		else
			SetAutoSize(false);
	}

	if (m_bIsAutoSize)
	{
		SetFixedWidth(m_pGifImage->GetWidth());
		SetFixedHeight(m_pGifImage->GetHeight());
	}
	if (m_bIsAutoPlay)
	{
		PlayGif();
	}

}

void CGifAnimUIEx::DrawFrame(HDC hDC)
{
	if (NULL == hDC || NULL == m_pGifImage) return;
	GUID pageGuid = Gdiplus::FrameDimensionTime;
	Gdiplus::Graphics graphics(hDC);
	graphics.DrawImage(m_pGifImage, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
	m_pGifImage->SelectActiveFrame(&pageGuid, m_nFramePosition);
}

void CGifAnimUIEx::SetBkImage(LPCTSTR pStrImage)
{
	if (m_sBkImage == pStrImage || NULL == pStrImage) return;

	m_sBkImage = pStrImage;

	StopGif();
	DeleteGif();

	Invalidate();

}

LPCTSTR CGifAnimUIEx::GetBkImage()
{
	return m_sBkImage.GetData();
}

void CGifAnimUIEx::SetAutoPlay(bool bIsAuto)
{
	m_bIsAutoPlay = bIsAuto;
}

bool CGifAnimUIEx::IsAutoPlay() const
{
	return m_bIsAutoPlay;
}

void CGifAnimUIEx::SetAutoSize(bool bIsAuto)
{
	m_bIsAutoSize = bIsAuto;
}

bool CGifAnimUIEx::IsAutoSize() const
{
	return m_bIsAutoSize;
}

void CGifAnimUIEx::PlayGif()
{
	if (m_bIsPlaying || m_pGifImage == NULL)
	{
		return;
	}

	long lPause = ((long*)m_pPropertyItem->value)[m_nFramePosition] * 10;
	if (lPause == 0) lPause = 100;
	m_pManager->SetTimer(this, EVENT_TIEM_ID, lPause);

	m_bIsPlaying = true;
}

void CGifAnimUIEx::PauseGif()
{
	if (!m_bIsPlaying || m_pGifImage == NULL)
	{
		return;
	}

	m_pManager->KillTimer(this, EVENT_TIEM_ID);
	this->Invalidate();
	m_bIsPlaying = false;
}

void CGifAnimUIEx::StopGif()
{
	if (!m_bIsPlaying)
	{
		return;
	}

	m_pManager->KillTimer(this, EVENT_TIEM_ID);
	m_nFramePosition = 0;
	this->Invalidate();
	m_bIsPlaying = false;
}

void CGifAnimUIEx::DeleteGif()
{
	if (m_pGifImage != NULL)
	{
		pStm->Release();
		::GlobalUnlock(hMem);
		delete m_pGifImage;
		m_pGifImage = NULL;
	}

	if (m_pPropertyItem != NULL)
	{
		free(m_pPropertyItem);
		m_pPropertyItem = NULL;
	}
	m_nFrameCount = 0;
	m_nFramePosition = 0;
}

void CGifAnimUIEx::OnTimer(UINT_PTR idEvent)
{
	if (idEvent != EVENT_TIEM_ID)
		return;
	m_pManager->KillTimer(this, EVENT_TIEM_ID);
	this->Invalidate();

	m_nFramePosition = (++m_nFramePosition) % m_nFrameCount;

	long lPause = ((long*)m_pPropertyItem->value)[m_nFramePosition] * 10;
	if (lPause == 0) lPause = 100;
	m_pManager->SetTimer(this, EVENT_TIEM_ID, lPause);
}

void CGifAnimUIEx::SetVisible(bool bVisible /* = true */)
{
	CControlUI::SetVisible(bVisible);
	if (bVisible)
		PlayGif();
	else
		StopGif();
}