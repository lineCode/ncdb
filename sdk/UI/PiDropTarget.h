#pragma once
#include "afxole.h"
#include <functional>
#include "PiTypeDef.h"
class CPiDropTarget :
	public IDropTarget
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
	void SetSelfFlagFormat(UINT nFormat);
	bool DragDropRegister(HWND hWnd, DWORD AcceptKeyState = MK_LBUTTON);
	bool DragDropRevoke(HWND hWnd);
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void **ppvObject);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	//进入
	HRESULT STDMETHODCALLTYPE DragEnter(__RPC__in_opt IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, __RPC__inout DWORD *pdwEffect);
	//移动
	HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, __RPC__inout DWORD *pdwEffect);
	//离开
	HRESULT STDMETHODCALLTYPE DragLeave();
	//释放
	HRESULT STDMETHODCALLTYPE Drop(__RPC__in_opt IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, __RPC__inout DWORD *pdwEffect);
	bool ReadHdropData(__RPC__in_opt IDataObject * pDataObj);
	void SetDragFun(std::function<void(const LST_STRING&)> fDrag);
private:
	HWND m_hWnd;
	IDropTargetHelper* m_piDropHelper;
	bool	m_bUseDnDHelper;
	DWORD m_dAcceptKeyState;
	ULONG  m_lRefCount;
	UINT			m_nDragSelfFlag;		//自身拖动时会有的数据格式
	IDataObject*		m_pDataObj;
private:
	std::function<void(const LST_STRING&)>		m_fDrag;
};

