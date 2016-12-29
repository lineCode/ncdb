#pragma once
#include "afxole.h"
class CPiDropTarget :
	public COleDropTarget
{
public:
	CPiDropTarget();
	~CPiDropTarget();
public:
	/*DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);

	DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);

	BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point);

	void OnDragLeave(CWnd* pWnd);*/

};

