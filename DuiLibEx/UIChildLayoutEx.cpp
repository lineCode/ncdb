#include "StdAfx.h"
#include "..\..\DuiLib\UIlib.h"
#include "UIChildLayoutEx.h"

IMPLEMENT_DUICONTROL(CChildLayoutUIEx)

CChildLayoutUIEx::CChildLayoutUIEx()
{
	
}
CChildLayoutUIEx::~CChildLayoutUIEx()
{
}

//**************************************************************************************
//            由于原生CChildLayout不支持自定义扩展控件解析，所以在此进行修改
//**************************************************************************************
void CChildLayoutUIEx::Init(IDialogBuilderCallback* m_pCallback)
{
	if (!m_pstrXMLFile1.IsEmpty())
	{
		CDialogBuilder builder;
		CContainerUI* pChildWindow = static_cast<CContainerUI*>(builder.Create(m_pstrXMLFile1.GetData(), (UINT)0, m_pCallback, m_pManager));
		if (pChildWindow)
		{
			this->Add(pChildWindow);
		}
		else
		{
			this->RemoveAll();
		}
	}
}

void CChildLayoutUIEx::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("xmlfile")) == 0 )
		SetChildLayoutXML(pstrValue);
	else
		CContainerUI::SetAttribute(pstrName,pstrValue);
}

void CChildLayoutUIEx::SetChildLayoutXML(DuiLib::CDuiString pXML)
{
	m_pstrXMLFile1 = pXML;
}

DuiLib::CDuiString CChildLayoutUIEx::GetChildLayoutXML()
{
	return m_pstrXMLFile1;
}

LPVOID CChildLayoutUIEx::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, DUI_CTR_CHILDLAYOUT) == 0) return static_cast<CChildLayoutUIEx*>(this);
	return CControlUI::GetInterface(pstrName);
}

LPCTSTR CChildLayoutUIEx::GetClass() const
{
	return _T("ChildLayoutUI");
}
