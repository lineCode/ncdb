#pragma once
#include "Duilib\DuiBase.h"
class CDuiMain :
	public CDuiBase
{
public:
	CDuiMain();
	~CDuiMain();
private:
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnClick(TNotifyUI& msg);

	CRichEditUI* m_pSendEdit;

};

