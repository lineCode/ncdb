#pragma once

class CPiContainerExUI :
	public CContainerUI
{
public:
	CPiContainerExUI(void);
	~CPiContainerExUI(void);
public:
	 LPCTSTR GetClass() const;
	void SetHotImage(LPCTSTR pStrImage);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoEvent(TEventUI& event);
	void PaintStatusImage(HDC hDC);

private:
	//CDuiString m_sHotImage;
	TDrawInfo m_diHot;
	UINT m_uButtonState;

};
