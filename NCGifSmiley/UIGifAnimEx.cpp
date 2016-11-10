#include "StdAfx.h"
#include "UIGifAnimEx.h"

///////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DUICONTROL(CGifAnimUIEx)

CGifAnimUIEx::CGifAnimUIEx(void)
{
	m_pGifImage = NULL;
	gifHeight = 0;
	gifWidth = 0;
}


CGifAnimUIEx::~CGifAnimUIEx(void)
{
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
				
			StopGif();
			DoInit();
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
	CGifAnimUI::DoEvent(event);
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
	CGifAnimUI::DoPaint(hDC, rcPaint);	
	//if (!bFirstPaint){
	//	m_pManager->SetTimer(this, SHOWLINE_TIME_ID, 100);
	//	bFirstPaint = true;
	//}
}

void CGifAnimUIEx::DoInit()
{
	m_pGifImage = LoadGifFromFile(GetBkImage());
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
	CGifAnimUI::DoInit();
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
	delete pData;
	return pImage;
}


Gdiplus::Image* CGifAnimUIEx::LoadGifFromMemory(LPVOID pBuf, size_t dwSize)
{
	HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, dwSize);
	BYTE* pMem = (BYTE*)::GlobalLock(hMem);

	memcpy(pMem, pBuf, dwSize);

	IStream* pStm = NULL;
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
//void CGifAnimUIEx::OnLineTimer(UINT_PTR idEvent)
//{
//	if (idEvent != SHOWLINE_TIME_ID)
//		return;
//	m_pManager->KillTimer(this, SHOWLINE_TIME_ID);
//	//this->Invalidate();
//	RECT rc = GetPos();
//	rc.bottom += 3;
//	rc.left -= 1;
//	rc.right += 3;
//	rc.top -= 1;
//	SIZE sizeClient = GetManager()->GetClientSize();                      //获取窗口大小
//	if (rc.bottom > sizeClient.cy - 40){
//		rc.bottom = sizeClient.cy - 40;
//	}
//	HDC hdc = this->GetManager()->GetPaintDC();
//	CRenderEngine::DrawRect(hdc, rc, 1, 0xFFCCCCCC, m_nBorderStyle);
//}