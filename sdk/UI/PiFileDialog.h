#pragma once
#include "afxdlgs.h"
#include "PiTypeDef.h"

class CPiFileDialog :
	public CFileDialog
{
public:
	CPiFileDialog(tcpchar szTitle, tcpchar szFilter = nullptr, tcpchar szInitDir = nullptr, bool bSelectMulti = true);
	~CPiFileDialog();
public:
	bool Popup();
	ARR_STRING GetSelect();
	virtual void OnButtonClicked(DWORD dwIDCtl);
	bool EndSelect();
	void SetParam(void* pTag);

private:
	virtual void OnOK();
	virtual void OnInitDone();
	virtual BOOL OnFileNameOK();
	virtual void OnFileNameChange();
	static LRESULT __stdcall _WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool OnInit();
private:
	IFileOpenDialog*		m_pIOD;
	IFileDialog*			m_pIFileDialog;
	bool					m_bInit;
	ARR_STRING				m_strSelect;
	void*					m_pTag;
};
