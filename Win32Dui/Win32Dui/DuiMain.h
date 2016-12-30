#pragma once
#include "Duilib\DuiBase.h"
class CNCCef;
class CDuiMain :
	public CDuiBase
{
public:
	CDuiMain();
	~CDuiMain();
private:
	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnClick(DuiLib::TNotifyUI& msg);
	bool TestCef();
	DuiLib::CRichEditUI*		m_pSendEdit;
	CNCCef*				m_pCef;
};

