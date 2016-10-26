#include "StdAfx.h"
#include "DuiMsgbox.h"
#include "PiWindowPack.h"
#include "PiUIText.h"
#include "DataStruct/PiSize.h"

void InitPic();


static CDuiMsgbox::MAP_PIC		g_mapIcon;

#define STRING_CONTROL_TXT_CONTENT				_T("txt_content")
#define STRING_CONTROL_PIC						_T("pic_icon")
#define STRING_CONTROL_LAY_WINDOW				_T("lay_Window")
#define STRING_CONTROL_BTN_CLOSE				_T("closebtn")

CDuiMsgbox::CDuiMsgbox( LPCTSTR szTitle, LPCTSTR szContent, ENUM_ICON_TYPE enType, ENUM_BTN_TYPE enBtnType, HWND hParent /*= NULL*/ )
:CDuiBase(hParent)
,m_strTitle(szTitle)
,m_strContent1(szContent)
,m_enIconType(enType)
,m_enBtnType(enBtnType)
{
	m_SkinFile			= _T("MsgBox.xml");
	m_SkinFolder		= _T("skin\\");
	m_ResType			= UILIB_FILE;
	m_nContentCx		= 0;
	m_szWindow			= CDuiSize(0,0);
}

CDuiMsgbox::~CDuiMsgbox(void)
{
}

void CDuiMsgbox::OnShow( BOOL bShow, int nState )
{
	CDuiString strBtnOk(_T("btn_ok"));
	CDuiString strBtnCancel(_T("btn_cancel"));
	if (bShow)
	{
		OutInfo(_T("即将显示弹窗"));
		if (m_szWindow.cx && m_szWindow.cy)
		{
			ChangeSize(m_szWindow.cx, m_szWindow.cy);
		}

		SetDuiWindowText(_T("txt_title"), m_strTitle);
		SetDuiWindowText(STRING_CONTROL_TXT_CONTENT, m_strContent1);
		SetDuiWindowBkImage(STRING_CONTROL_PIC, g_mapIcon[m_enIconType]);
		
		FindControl(STRING_CONTROL_TXT_CONTENT)->EstimateSize(CPiSize(9999,9999));

		ARR_CONTROL_NAME arrCt;
		arrCt.push_back(STRING_CONTROL_PIC);
		arrCt.push_back(STRING_CONTROL_TXT_CONTENT);
		ChangeSize(CenterPosX(arrCt, 5, GetWindowSize().cx), -1);

		//调整标题和窗口尺寸
		if (m_nContentCx > 0)
		{
			FindControl(STRING_CONTROL_TXT_CONTENT)->SetFixedWidth(m_nContentCx);
		}
		
		switch(m_enBtnType)
		{
		case bt_ok:
			/*
				ok按钮x坐标:	x宽度+间隙+cancel按钮宽度

				cancel按钮x坐标: ok按钮x坐标+ok按钮宽度+间隙
			--------------------*/
			//
			{
				ARR_CONTROL arrCt;
				arrCt.push_back(FindControl(_T("btn_ok")));
				CenterPosX(arrCt, 40, GetWindowSize().cx);

				SetDuiWindowText(strBtnOk, _T("确定"));
				SetDuiWindowVisible(strBtnCancel, false);
			}
			break;
		case bt_ok_cancel:
			{
				ChangeTwoBtnPos();
				SetDuiWindowText(strBtnOk, _T("确定"));
				SetDuiWindowText(strBtnCancel, _T("取消"));
			}
			break;
		case bt_yes_no:
			ChangeTwoBtnPos();
			SetDuiWindowText(strBtnOk, _T("是"));
			SetDuiWindowText(strBtnCancel, _T("否"));
			break;

		case bt_full:
			{
				LST_STRING lst;
				lst.push_back(STRING_CONTROL_PIC);
				lst.push_back(STRING_CONTROL_TXT_CONTENT);
				ShowFilter(_T("lay_Window"), lst);

				CControlUI* pWnd = FindControl(STRING_CONTROL_LAY_WINDOW);
				if(pWnd)
				{
					pWnd->SetBkColor(0xFFFFFFFF);
				}


				CControlUI* pPic = FindControl(STRING_CONTROL_PIC);
				CControlUI* pText = FindControl(STRING_CONTROL_TXT_CONTENT);
				pPic->SetBkImage(m_strFullPic);
				pText->SetText(m_strContent1);

				ARR_CONTROL arrCt;
				arrCt.push_back(FindControl(STRING_CONTROL_TXT_CONTENT));
				CenterPosX(arrCt, 40, GetWindowSize().cx);
			}
			break;
		}
	}
	InvalidateRect(m_hWnd, NULL, TRUE);
}

UINT CDuiMsgbox::Popup( LPCTSTR szTitle, LPCTSTR szContent, ENUM_ICON_TYPE enType, ENUM_BTN_TYPE enBtnType, HWND hParent /*= NULL*/ )
{
	CDuiMsgbox dui(szTitle, szContent, enType, enBtnType, hParent);
	UINT nRet = 0;
	if(!dui.CreateModalShow(&nRet))	//创建失败视为IDCANCEL
	{
		return IDCANCEL;
	}
	return nRet;
}

void CDuiMsgbox::InitPic( const MAP_PIC& mapPic )
{
	g_mapIcon = mapPic;
}

void CDuiMsgbox::OnClick( TNotifyUI& msg )
{
	if (CDuiCommon::IsControl(msg.pSender, _T("closebtn"))
		|| CDuiCommon::IsControl(msg.pSender, _T("btn_cancel")))
	{
		//ShowWindow(false);
		Close(IDCANCEL);
	}
	else if(CDuiCommon::IsControl(msg.pSender, _T("btn_ok")))
	{
		Close(IDOK);
	}
}

void CDuiMsgbox::InitWindow()
{
	/*CDuiString strBtnOk(_T("btn_ok"));
	CDuiString strBtnCancel(_T("btn_cancel"));

	CControlUI* pCtOk = FindControl(strBtnOk);
	CControlUI* pCtCancel = FindControl(strBtnCancel);
	ARR_RECT lstRt;
	lstRt.push_back(CPiRect(pCtOk->GetFixedXY().cx, pCtOk->GetFixedXY().cy, pCtOk->GetFixedWidth(), pCtOk->GetFixedHeight()));
	lstRt.push_back(CPiRect(pCtOk->GetFixedXY().cx, pCtCancel->GetFixedXY().cy, pCtCancel->GetFixedWidth(), pCtCancel->GetFixedHeight()));
	CenterPosX(lstRt, 40, 300);*/
}

void CDuiMsgbox::ChangeTwoBtnPos()
{
	ARR_CONTROL_NAME arrCt;
	arrCt.push_back(_T("btn_ok"));
	arrCt.push_back(_T("btn_cancel"));
	CenterPosX(arrCt, 40, GetWindowSize().cx);
}

UINT CDuiMsgbox::PopupFull( LPCTSTR szBkImg, LPCTSTR szText, HWND hParent /*= NULL*/ )
{
	CDuiMsgbox dui(_T(""), _T(""), t_done,  bt_full, hParent);
	
	//背景全屏, 文字居中
	//除了两个控件, 其他全隐藏
	dui.SetFull(szBkImg, szText);
	
	UINT nRet = 0;
	if(!dui.CreateModalShow(&nRet))	//创建失败视为IDCANCEL
	{
		return IDCANCEL;
	}
	return nRet;

}

void CDuiMsgbox::SetFull( LPCTSTR szBkImg, LPCTSTR szText )
{
	m_strFullPic = szBkImg;
	m_strContent1 = szText;
}

void CDuiMsgbox::ChangeSize( int nCx, int nCy )
{
	CPiWindowPack::ChangeSize(m_hWnd, nCx, nCy);
	AdjustBtnClose();
}

void CDuiMsgbox::AdjustBtnClose()
{
	CControlUI* pCt = FindControl(STRING_CONTROL_BTN_CLOSE);
	SIZE szClose = {pCt->GetFixedWidth(), pCt->GetFixedHeight()};
	CDuiSize szPoint(GetWindowSize().cx - szClose.cx, 0);
	CDuiSize szSpace(5, 5);		//关闭按钮离窗口右上角的距离
	szPoint.cx -= szSpace.cx;
	szPoint.cy += szSpace.cy;
	SetDuiCtPos(STRING_CONTROL_BTN_CLOSE, szPoint);
}

void CDuiMsgbox::SetContentCx( int nCx )
{
	m_nContentCx = nCx;
}

void CDuiMsgbox::SetSize( CDuiSize szWnd )
{
	m_szWindow = szWnd;
}


CControlUI* CDuiMsgbox::CreateControl(LPCTSTR pstrClass)
{
	if ( _tcscmp(pstrClass, _T("PiText")) == 0 )//参数在XML中
	{
		return new CPiUIText;
	}
	return NULL;
}

