#pragma once
/*------------------------------------------------------
	Fun:	对duilib窗口控件进行队team-组-个的管理, 每队里的各组互斥,同一时刻只有1组可显示
	Authon: liwanliang
	Date:	2015/09/06
------------------------------------------------------*/
#include "PiTypeDef.h"
#include "DuiHeader.h"

struct tagSHADE_CONTROL
	
{
	tagSHADE_CONTROL(const tstring strControlR, bool bShowR)
		:bShow(bShowR)
		, strControl(strControlR)
	{
	}
	bool	bShow;
	tstring	strControl;
};
typedef list<tagSHADE_CONTROL>		LST_SHADE;
typedef map<int, LST_STRING>		MAP_CONTROL_LIST;
typedef map<int, MAP_CONTROL_LIST>	MAP_CONTROL_TEAM;
typedef map<int, LST_SHADE>			MAP_CONTROL_SHADE;

class CDuiShowPack
{
public:
	enum ENUM_ELEM{All_Elem = 999999};
	CDuiShowPack(CPaintManagerUI* pPaintManager = NULL);
	~CDuiShowPack(void);
public:
	/*------------------------------------------------------------
			fun:	把指定组外的所有组的控件都隐藏, 显示指定组
			ret:	
			param: pCtn: 控件所在的容器,如果该值为0, 在全局界面中查找控件
			remark:	
	------------------------------------------------------------*/
	void ChangeUI( int nTeamID, int nGroupID, CContainerUI* pCtn = NULL);
	/*------------------------------------------------------------
			fun:	隐藏所有组, 显示指定组
			ret:	
			param: pCtn: 控件所在的容器,如果该值为0, 在全局界面中查找控件
			remark:	
	------------------------------------------------------------*/
	void Show( int nTeamID, int nGroupID, CContainerUI* pCtn = NULL);
	/*------------------------------------------------------------
			fun:	显示特定组的控件
			ret:	
			param: true, 使用添加时指定的可见性, false:使用跟添加时相反的可见性
			remark:	
	------------------------------------------------------------*/
	void ShowShade(int nGroupID, CContainerUI* pCtn = NULL, bool bShow = true);
	//void Show(CListContainerElementUI* pListCtn, int nTeamID, int nGroupID );
	void AddShow( int nTeamID, const MAP_CONTROL_LIST& mapList );
	void ShowAll( int nGroupID );
	bool HideAll();
	bool Hide(int nTeamID);
	/*------------------------------------------------------------
			fun:	增加可反转的控件操作
			ret:	
			param: bShow, 默认的显示标识, 后面显示时可以反转该标识
			remark:	使用ShowShade来显示添加的控件
	------------------------------------------------------------*/
	void AddShadeShow( int nGroup, tcpchar szControl, bool bShow);
private:
	MAP_CONTROL_TEAM				m_mapConnectPrompt;//顶部的连接提示
	MAP_CONTROL_SHADE				m_mapShowShade;	//可原样撤销整组显示隐藏
	CPaintManagerUI*					m_PaintManager;
};
