#ifndef __UICOMBOEX_H__
#define __UICOMBOEX_H__

#pragma once
//class CComboWndEx;
class CComboUIEx;
class CComboWndEx : public CWindowWnd, public INotifyUI
{
public:
	void Init(CComboUIEx* pOwner);
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Notify(TNotifyUI& msg) override;

	void EnsureVisible(int iIndex);
	void Scroll(int dx, int dy);

#if(_WIN32_WINNT >= 0x0501)
	virtual UINT GetClassStyle() const;
#endif
	bool IsHitItem(POINT ptMouse);
public:
	CPaintManagerUI m_pm;
	CComboUIEx* m_pOwner;
	CVerticalLayoutUI* m_pLayout;
	int m_iOldSel;
	bool m_bHitItem;
};

class CComboUIEx : public CComboUI
{
	DECLARE_DUICONTROL(CComboUIEx)
	friend class CComboWndEx;
public:
	CComboUIEx();
	bool Activate();

protected:
	CComboWndEx* m_pWindow;
};
#endif // __UICOMBO_H__
