#include "StdAfx.h"
#include "DuiBase.h"
#include "PiWindowPack.h"

CDuiBase::CDuiBase( HWND hParent /*= NULL*/ ) :m_ResType(UILIB_FILE)
,m_bCenterToParent(true)
,m_hParent(hParent)
{
	m_WndClassName =  _T("DuiWnd");
}

CDuiBase::~CDuiBase(void)
{
}


LPCTSTR CDuiBase::GetWindowClassName( void ) const
{
	return m_WndClassName;
}

DuiLib::CDuiString CDuiBase::GetSkinFolder()
{
	return m_SkinFolder;
}

DuiLib::CDuiString CDuiBase::GetSkinFile()
{
	return m_SkinFile;
}

DuiLib::UILIB_RESOURCETYPE CDuiBase::GetResourceType() const
{
	return m_ResType;
}

LPCTSTR CDuiBase::GetResourceID() const
{
	return m_strResID;
}





bool CDuiBase::IsWindow()
{
	return ::IsWindow(m_hWnd) == TRUE;

}

CDuiBase::operator HWND()
{
	return m_hWnd;
}

const PIMSGMAP* CDuiBase::GetPiMessageMap() const
{
	return GetPiThisMessageMap();
}

const PIMSGMAP* PASCAL CDuiBase::GetPiThisMessageMap()
{
	static const PIMSGMAP_ENTRY _messageEntries[] =
	{
		{ 0, 0, 0, 0 }     // nothing here
	};
	static const PIMSGMAP messageMap =
	{
		NULL,
		&_messageEntries[0]
	};
	return &messageMap;	
}

LRESULT CDuiBase::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bDuiHandle = true;
	//
	const PIMSGMAP* pMM = GetPiMessageMap();
	const PIMSGMAP_ENTRY* pMMEntery = NULL;
	pMMEntery = pMM->lpEntries;
	while(pMMEntery )
	{
		if(uMsg == pMMEntery->nMessage)
		{
			break;
		}
		if( pMMEntery->pfn 
			&& (pMMEntery = pMMEntery + 1 )
			)
		{
			continue;
		}
		//当前类消息映射列表没有, 找父类
		if (pMM->pfnGetBaseMap)
		{
			pMM = (*pMM->pfnGetBaseMap)();
			pMMEntery = pMM->lpEntries;
		}
		else
		{
			pMM = NULL;
			pMMEntery = NULL;
			break;
		}
	}

	if (pMMEntery && pMMEntery->pfn)
	{
		//const PIMSGMAP_ENTRY* pMMEntry = pMM->lpEntries;
		bDuiHandle = false;
		tagWNDMSG tag = {0};
		tag.message = uMsg;
		tag.wParam = wParam;
		tag.lParam = lParam;
		(this->*pMMEntery->pfn)(tag);
	}
	else
	{
		if(HandleMessageChild(uMsg, wParam, lParam))
		{
			bDuiHandle = false;
		}
	}
	
	if (bDuiHandle)
	{
		return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	}
	return 1;
}

LRESULT CDuiBase::HandleMessageChild( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)	
	{
	case WM_SHOWWINDOW:
		OnShow((BOOL)wParam, lParam);
		break;
	}
	return 0;
}

CControlUI* CDuiBase::FindControl( LPCTSTR szCt )
{
	return m_PaintManager.FindControl(szCt);
}

void CDuiBase::OnShow(BOOL bShow, int nState)
{
	if (bShow && m_bCenterToParent)
	{
		CPiWindowPack::CenterWindow(m_hWnd, GetParent(m_hWnd));
	}
}

bool CDuiBase::SetDuiWindowText( LPCTSTR szCt, LPCTSTR szText )
{
	CControlUI* pCt = m_PaintManager.FindControl(szCt);
	if (pCt)
	{
		pCt->SetText(szText);
	}
	return true;
}

CDuiString CDuiBase::GetDuiWindowText( LPCTSTR szCt )
{
	CControlUI* pCt = m_PaintManager.FindControl(szCt);
	if (pCt)
	{
		return pCt->GetText();
	}
	return _T("");
}

bool CDuiBase::SetDuiWindowVisible( LPCTSTR szCt, bool bShow )
{
	CControlUI* pCt = m_PaintManager.FindControl(szCt);
	if (pCt)
	{
		pCt->SetVisible(bShow);
	}
	return true;

}


bool CDuiBase::SetDuiWindowBkImage( LPCTSTR szCt, LPCTSTR szPic )
{
	CControlUI* pCt = m_PaintManager.FindControl(szCt);
	if (pCt)
	{
		pCt->SetBkImage(szPic);
	}
	return true;
}

LRESULT CDuiBase::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_WndShadow.SetSize(6);
	m_WndShadow.SetPosition(0,0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetDarkness(121);
	m_WndShadow.Create(m_hWnd);
	return __super::OnCreate(uMsg, wParam, lParam,bHandled);
}

bool CDuiBase::CreateModalShow( UINT* pRet /*= NULL*/ )
{
	if(!Create(m_hParent, _T(""), UI_WNDSTYLE_DIALOG , 0, 0, 0, 0, 0, NULL))
	{
		return false;
	}
	CenterWindow();
	UINT nRet = this->ShowModal();
	if (pRet)
	{
		*pRet = nRet;
	}
	return true;
}


bool CDuiBase::CreateChild(UINT* pRet /*= NULL*/)
{
	if (!Create(m_hParent, _T(""), UI_WNDSTYLE_CHILD, 0, 0, 0, 0, 0, NULL))
	{
		return false;
	}
	this->ShowWindow(true);
	//UINT nRet = this->ShowModal();
	/*if (pRet)
	{
		*pRet = nRet;
	}*/
	return true;
}


bool CDuiBase::ShowFilter( LPCTSTR szContainter, const LST_STRING& lstFilter )
{
	CContainerUI* pCtn = static_cast<CContainerUI*>(m_PaintManager.FindControl(szContainter));
	int nCount = pCtn->GetCount();
	CControlUI* pct = NULL;
	for (int i = 0; i < nCount; ++i)
	{
		pct = pCtn->GetItemAt(i);
		if(lstFilter.end() != find(lstFilter.begin(), lstFilter.end(), pct->GetName().GetData()))
		{
			pct->SetVisible(true);
			continue;
		}
		pct->SetVisible(false);
	}
	return true;
}

int CDuiBase::CenterPosX( ARR_CONTROL& lstCt, int nSpace, int nAreaWidth )
{
	ARR_RECT lstRt;
	for (int i = 0; i < lstCt.size(); ++i)
	{
		CControlUI* pCt = lstCt[i];
		if(!pCt)
		{
			continue;
		}
		lstRt.push_back(CPiRect(pCt->GetFixedXY().cx, pCt->GetFixedXY().cy, pCt->GetFixedWidth(), pCt->GetFixedHeight()));
	}

	int nWidthNeed = CPiWindowPack::CenterPosX(lstRt, nSpace, nAreaWidth);

	SIZE sz = {0};
	for (int i = 0; i < lstRt.size(); ++i)
	{
		sz.cx = lstRt[i].GetPos().x;
		sz.cy = lstRt[i].GetPos().y;
		
		lstCt[i]->SetPos(lstRt[i]);
	}

	return nWidthNeed;
}

int CDuiBase::CenterPosX( LPCTSTR szControlName, int nSpace, int nAreaWidth )
{
	ARR_CONTROL arrCt;
	arrCt.push_back(FindControl(szControlName));
	return CenterPosX(arrCt, nSpace, nAreaWidth);
}

int CDuiBase::CenterPosX( ARR_CONTROL_NAME& lstCt, int nSpace, int nAreaWidth )
{
	ARR_CONTROL arrCt;
	for (int i = 0; i < lstCt.size(); ++i)
	{
		arrCt.push_back(FindControl(lstCt[i]));
	}
	return CenterPosX(arrCt, nSpace, nAreaWidth);
}

SIZE CDuiBase::GetWindowSize()
{
	return CPiWindowPack::GetWindowSize(m_hWnd);
}

bool CDuiBase::SetDuiCtPos( LPCTSTR szCt, const DuiLib::CSize& pt )
{
	CControlUI* pCt = m_PaintManager.FindControl(szCt);
	if (pCt)
	{
		pCt->SetFixedXY(pt);
	}
	return true;
}

void CDuiBase::SetWndowSize( SIZE szSize )
{
	CPiWindowPack::ChangeSize(m_hWnd, szSize.cx, szSize.cy);

}

void CDuiBase::MoveWindow( int x, int y)
{
	CPiWindowPack::MoveWindow(m_hWnd, x, y);
}

