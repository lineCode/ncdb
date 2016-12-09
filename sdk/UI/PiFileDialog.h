#pragma once
#include "afxdlgs.h"
#include "PiTypeDef.h"
class CPiFileDialog :
	public CFileDialog
{
public:
	CPiFileDialog(tcpchar szTitle, tcpchar szFilter = nullptr);
	~CPiFileDialog();
public:
	bool Popup();
	ARR_STRING GetSelect();
	virtual void OnButtonClicked(DWORD dwIDCtl);
	bool EndSelect();

private:
	virtual void OnOK();
	virtual void OnInitDone();
	virtual BOOL OnFileNameOK();
	virtual void OnFileNameChange();
private:
	IFileOpenDialog*		m_pIOD;
	IFileDialog*			m_pIFileDialog;
	bool					m_bInit;
	ARR_STRING				m_strSelect;
};

