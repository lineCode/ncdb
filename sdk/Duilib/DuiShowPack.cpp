#include "StdAfx.h"
#include "DuiShowPack.h"
#include "define_gnl.h"

//#define FindCtrlInMain(m_PaintManager->FindControl
CDuiShowPack::CDuiShowPack(CPaintManagerUI* pPaintManager /*= NULL*/)
{
	m_PaintManager = pPaintManager;
}

CDuiShowPack::~CDuiShowPack(void)
{
}

void CDuiShowPack::ChangeUI( int nTeamID, int nGroupID, CContainerUI* pCtn /*= NULL*/ )
{
	MAP_CONTROL_LIST& mapList = m_mapConnectPrompt[nTeamID];
	FOR_EACH_ITER(MAP_CONTROL_LIST, mapList, it)
	{
		if (it->first == nGroupID)
		{
			continue;
			//bShow = true;
		}
		LST_STRING& lstStr = it->second;
		FOR_EACH_STRING(lstStr, itControl)
		{
			( pCtn 
				? pCtn->FindSubControl(itControl->c_str()) 
				: m_PaintManager->FindControl(itControl->c_str()) 
				)->SetVisible(false);
		}
	}

	LST_STRING& lstStr = mapList[nGroupID];
	FOR_EACH_STRING(lstStr, itControl)
	{
		( pCtn 
			? pCtn->FindSubControl(itControl->c_str()) 
			: m_PaintManager->FindControl(itControl->c_str()) 
			)->SetVisible(true);
	}
}


void CDuiShowPack::Show( int nTeamID, int nGroupID, CContainerUI* pCtn /*= NULL*/ )
{
	MAP_CONTROL_LIST& mapList = m_mapConnectPrompt[nTeamID];
	LST_STRING& lstStr1 = mapList[All_Elem];
	CControlUI* pCt = NULL;
	FOR_EACH_STRING(lstStr1, itControl)
	{
		pCt = ( pCtn 
			? pCtn->FindSubControl(itControl->c_str()) 
			: m_PaintManager->FindControl(itControl->c_str()) 
			);
		if (pCt)
		{
			pCt->SetVisible(false);
		}
	}
	LST_STRING& lstStr2 = mapList[nGroupID];
	FOR_EACH_STRING(lstStr2, itControl)
	{
		pCt = ( pCtn 
			? pCtn->FindSubControl(itControl->c_str()) 
			: m_PaintManager->FindControl(itControl->c_str()) 
			);
		if (pCt)
		{
			pCt->SetVisible(true);
		}
	}
}


/*

void CDuiShowPack::Show( , int nTeamID, int nGroupID )
{
	Show(nTeamID, nGroupID);
}*/

void CDuiShowPack::AddShow( int nTeamID, const MAP_CONTROL_LIST& mapList )
{
	
	m_mapConnectPrompt[nTeamID] = mapList;
}

void CDuiShowPack::ShowAll( int nGroupID )
{
	FOR_EACH_ITER(MAP_CONTROL_TEAM, m_mapConnectPrompt, it)
	{
		ChangeUI(it->first, nGroupID);
	}
}

bool CDuiShowPack::HideAll()
{
	FOR_EACH_ITER(MAP_CONTROL_TEAM, m_mapConnectPrompt, it)
	{
		Hide(it->first);
	}
	return true;
}

bool CDuiShowPack::Hide( int nTeamID )
{
	MAP_CONTROL_LIST& mapList = m_mapConnectPrompt[nTeamID];
	FOR_EACH_ITER(MAP_CONTROL_LIST, mapList, it)
	{
		FOR_EACH_STRING(it->second, itControl)
		{
			m_PaintManager->FindControl(itControl->c_str())->SetVisible(false);
		}
	}
	return true;
}

void CDuiShowPack::AddShadeShow( int nGroup, tcpchar szControl, bool bShow )
{
	m_mapShowShade[nGroup].push_back(tagSHADE_CONTROL(szControl, bShow));
}

void CDuiShowPack::ShowShade( int nGroupID, CContainerUI* pCtn /*= NULL*/, bool bShow /*= true*/)
{
	LST_SHADE& lstS = m_mapShowShade[nGroupID];
	FOR_EACH_ITER(LST_SHADE, lstS, itControl)
	{
		( pCtn 
			? pCtn->FindSubControl(itControl->strControl.c_str()) 
			: m_PaintManager->FindControl(itControl->strControl.c_str()) 
			)->SetVisible(bShow ? itControl->bShow : !itControl->bShow);
	}
}

