#include "StdAfx.h"
#include "PiWindowEffect.h"
#include "PiWindowPack.h"
#include "Math/PiEquation.h"
#include "ThreadLight.h"
#include "PiIrregularWindow.h"
#include "System/PiThread.h"
#include <math.h>
#include "Math/mathUnit.h"
static const UNINT NUM_SHOW_TIME = 20;
static const UNINT NUM_MOVE_IDLE = 30;
Pi_NameSpace_Using

CPiWindowEffect::CPiWindowEffect(void)
{
	m_nShowTime = 2000;
}

CPiWindowEffect::~CPiWindowEffect(void)
{
}

bool CPiWindowEffect::MoveToPointLine( HWND hWnd, POINT& ptDist )
{
	//异步实现窗口移动
	/*创建线程, 线程中调用函数
		函数创建窗口, 实现窗口的轨迹移动

		在移动完成后销毁窗口
		
	*/
	tagParam* tag = new tagParam;
	tag->hWnd = hWnd;
	tag->ptDist = ptDist;
	CThreadLight* pThread = CThreadLight::Run(&CPiWindowEffect::_MoveToPointLine, tag);


	return true;
}

DWORD CPiWindowEffect::_MoveToPointLine( void* paramR )
{
	tagParam* pPar = (tagParam*)paramR;
	HWND hWnd = pPar->hWnd;
	CPiPoint ptDist = pPar->ptDist;
	delete pPar;

	int nShowTime = NUM_SHOW_TIME;
	//获取窗口坐标
	//根据两点算出移动轨迹
	POINT ptTemp;
	CPiWindowPack::GetWindowPos(hWnd, ptTemp);

	CPiPoint ptOrigin(ptTemp);
	CPiPoint ptCurrent  = ptOrigin;

	tagSTRAIGHT_LINE_CONDITION tagCond;
	tagCond.pt1 = ptOrigin;
	tagCond.pt2 = ptDist;

	CPiEquation equation;
	equation.SolveStraightLine(tagCond);

	bool bVertical = ptOrigin.IsVertical(ptDist);

	float nStep = 1.0 * (ptDist.m_x - ptOrigin.m_x) /  nShowTime;
	if (bVertical)
	{
		nStep = 1.0 * (ptDist.m_y - ptOrigin.m_y) /  nShowTime;
	}

	float fMathX = ptCurrent.m_x;
	float fMathY = ptCurrent.m_y;
	CPiPoint::PointDataType nDiff = 0;
	while( ptCurrent != ptDist)
	{
		if (bVertical)
		{
			nDiff = ptDist.m_y - ptCurrent.m_y;
			fMathY += ( fabs( (float) nDiff) > fabs(nStep) ? nStep : nDiff );
			fMathX = equation.GetStraightLineX(fMathY);
		}
		else
		{
			nDiff = ptDist.m_x - ptCurrent.m_x;
			fMathX += ( fabs( (float) nDiff) > fabs(nStep) ? nStep : nDiff );
			fMathY = equation.GetStraightLineY(fMathX);
		}
		
		if ( FlowEqual(nDiff, 0))
		{
			break;
		}

		CPiPoint ptMove(fMathX, fMathY);
		CPiWindowPack::MoveWindow(hWnd, ptMove);
		ptCurrent = ptMove;
		Sleep(NUM_MOVE_IDLE);
		
	}
	::SendMessage(hWnd, WM_DESTROY, 0, 0);
	return true;
}

bool CPiWindowEffect::CreateWindowMoveToPointLine( tcpchar szPic, const POINT& ptStart, const POINT& ptDist )
{
	//异步实现窗口移动
	/*创建线程, 线程中调用函数
		函数创建窗口, 实现窗口的轨迹移动

		在移动完成后销毁窗口
		
	*/
	

	tagParam* tag = new tagParam;
	tag->strPic = szPic;
	tag->ptStart = ptStart;
	tag->ptDist = ptDist;
	CPiThread(&CPiWindowEffect::_CreateWindowMoveToPointLine, tag);
	//CThreadLight* pThread = CThreadLight::Run(&CPiWindowEffect::_CreateWindowMoveToPointLine, tag);
	return true;
}

DWORD CPiWindowEffect::_CreateWindowMoveToPointLine( void* paramR )
{
	//异步实现窗口移动
	/*创建线程, 线程中调用函数
		函数创建窗口, 实现窗口的轨迹移动

		在移动完成后销毁窗口
		
	*/

	tagParam* pParam = (tagParam*)paramR;
	CPiIrregularWindow irr;
	if(!irr.CreateIrregularWindow(pParam->strPic.c_str()))
	{
		return false;
	}
	pParam->hWnd = irr;

	//TODO:窗口在目标窗口之上
	CPiWindowPack::TopMostWindow(irr);
	CPiWindowPack::SetNoTaskWindow(irr);

	CPiWindowPack::MoveWindow(irr, pParam->ptStart);
	ShowWindow(irr,SW_SHOWNORMAL);

	CPiThread(&CPiWindowEffect::_MoveToPointLine, pParam);

	MSG msg = {0};
	while (GetMessage (&msg, NULL, 0, 0))	//移动结束后需要收到WM_QUIT消息
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}

	return true;
}
