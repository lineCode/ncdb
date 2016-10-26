#ifndef __UICHILDLAYOUTEX_H__
#define __UICHILDLAYOUTEX_H__

#pragma once
class CChildLayoutUIEx : public CContainerUI
{
	DECLARE_DUICONTROL(CChildLayoutUIEx)
public:
	CChildLayoutUIEx();
	virtual ~CChildLayoutUIEx();

	virtual void Init(IDialogBuilderCallback* m_pCallback);
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	virtual void SetChildLayoutXML(CDuiString pXML);
	virtual CDuiString GetChildLayoutXML();
	virtual LPVOID GetInterface(LPCTSTR pstrName);
	virtual LPCTSTR GetClass() const;

private:
	CDuiString m_pstrXMLFile1;
};
#endif // __UICHILDLAYOUT_H__
