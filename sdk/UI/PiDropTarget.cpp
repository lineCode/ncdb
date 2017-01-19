#include "stdafx.h"
#include "PiDropTarget.h"
#include "IPiDataObject.h"
#include <shlguid.h>
#include <shlwapi.h>



CPiDropTarget::CPiDropTarget()
{
	m_lRefCount = 1;
	// Create an instance of the shell DnD helper object.
	if (SUCCEEDED(CoCreateInstance(CLSID_DragDropHelper, NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDropTargetHelper,
		(void**)&m_piDropHelper)))
	{
		m_bUseDnDHelper = false;
	}
}


CPiDropTarget::~CPiDropTarget()
{
	Revoke();
	if (m_piDropHelper)
	{
		m_piDropHelper->Release();
	}
	m_bUseDnDHelper = false;
	m_lRefCount = 0;
}


bool CPiDropTarget::DragDropRegister(HWND hWnd, DWORD AcceptKeyState)
{
	if (!IsWindow(hWnd))return false;

	HRESULT s = ::RegisterDragDrop(hWnd, this);
	m_hWnd = hWnd;
	if (SUCCEEDED(s))
	{
		m_dAcceptKeyState = AcceptKeyState;
		return true;

	}

	else { return false; }

}

bool CPiDropTarget::Revoke()
{
	if (m_hWnd == NULL)
	{
		return false;
	}

	RevokeDragDrop(m_hWnd);
	//CoLockObjectExternal((LPUNKNOWN)GetInterface(&IID_IUnknown), FALSE, TRUE);
	m_hWnd = NULL;
	return true;
}
bool CPiDropTarget::DragDropRevoke(HWND hWnd)
{
	if (!IsWindow(hWnd))return false;

	HRESULT s = ::RevokeDragDrop(hWnd);

	return SUCCEEDED(s);
}

HRESULT STDMETHODCALLTYPE CPiDropTarget::QueryInterface(REFIID riid, __RPC__deref_out void **ppvObject)
{
	static const QITAB qit[] =
	{
		QITABENT(CPiDropTarget, IDropTarget),
		{ 0 }
	};

	return QISearch(this, qit, riid, ppvObject);
}
ULONG STDMETHODCALLTYPE CPiDropTarget::AddRef()
{
	return InterlockedIncrement(&m_lRefCount);
}
ULONG STDMETHODCALLTYPE CPiDropTarget::Release()
{
	ULONG lRef = InterlockedDecrement(&m_lRefCount);
	if (0 == lRef)
	{
		delete this;
	}
	return m_lRefCount;
}

//进入
HRESULT STDMETHODCALLTYPE CPiDropTarget::DragEnter(__RPC__in_opt IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, __RPC__inout DWORD *pdwEffect)
{
	DWORD dwEffect = DROPEFFECT_NONE;

	// Check for our own custom clipboard format in the data object.  If it's
	// present, then the DnD was initiated from our own window, and we won't
	// accept the drop.
	// If it's not present, then we check for CF_HDROP data in the data object.
	m_pDataObj = pDataObj;
	IPiDataObjectHelper piDO(m_pDataObj);
	if (!piDO.IsHasData(m_nDragSelfFlag)
		&& piDO.IsHasData(CF_HDROP))
	{
		// Look for CF_HDROP data in the data object, and accept the drop if
		// it's there.
		dwEffect = DROPEFFECT_COPY;
	}

	// Call the DnD helper.
	if (m_bUseDnDHelper)
	{
		// The DnD helper needs an IDataObject interface, so get one from
		// the COleDataObject.  Note that the FALSE param means that
		// GetIDataObject will not AddRef() the returned interface, so 
		// we do not Release() it.
		m_piDropHelper->DragEnter(m_hWnd, pDataObj, (LPPOINT)&pt, *pdwEffect);
	}
	*pdwEffect = dwEffect;
	return S_OK;
}

//移动
HRESULT STDMETHODCALLTYPE CPiDropTarget::DragOver(DWORD grfKeyState, POINTL pt, __RPC__inout DWORD *pdwEffect)
{
	DWORD dwEffect = DROPEFFECT_NONE;
	IPiDataObjectHelper piDO(m_pDataObj);
	if (!piDO.IsHasData(m_nDragSelfFlag)
		&& piDO.IsHasData(CF_HDROP))
	{
		// Look for CF_HDROP data in the data object, and accept the drop if
		// it's there.
		dwEffect = DROPEFFECT_COPY;
	}

	if (m_bUseDnDHelper)
	{
		m_piDropHelper->DragOver((LPPOINT)&pt, *pdwEffect);
	}
	*pdwEffect = dwEffect;
	return S_OK;;
}

//离开
HRESULT STDMETHODCALLTYPE CPiDropTarget::DragLeave()
{
	if (m_bUseDnDHelper)
	{
		m_piDropHelper->DragLeave();
	}
	return S_OK;;
}

//释放
HRESULT STDMETHODCALLTYPE CPiDropTarget::Drop(__RPC__in_opt IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, __RPC__inout DWORD *pdwEffect)
{
	ReadHdropData(pDataObj);
	

	
	if (m_bUseDnDHelper)
	{
		m_piDropHelper->Drop(pDataObj, (LPPOINT)&pt, *pdwEffect);
	}
	return S_OK;;
}

void CPiDropTarget::SetSelfFlagFormat(UINT nFormat)
{
	if (!nFormat)
	{
		return;
	}
	m_nDragSelfFlag = nFormat;
}

bool CPiDropTarget::ReadHdropData(__RPC__in_opt IDataObject * pDataObj)
{
	IPiDataObjectHelper piDO(pDataObj);
	HGLOBAL hg = piDO.GetData(CF_HDROP);
	if (!hg)
	{
		return false;
	}

	HDROP hdrop = (HDROP)GlobalLock(hg);

	if (!hdrop)
	{
		GlobalUnlock(hg);
		return false;
	}
	//get file list
	LST_STRING lstPath;

	tstring strName;
	UINT nIndex = 0;
	UINT nTotal = 0;
	UINT nBufNeed = 0;
	nTotal = DragQueryFile(hdrop, 0xFFFFFFFF, 0, 0);
	for (UINT nIndex = 0; nIndex < nTotal; ++nIndex)
	{
		nBufNeed = DragQueryFile(hdrop, nIndex, NULL, 0);
		++nBufNeed;
		strName.resize(nBufNeed, 0);
		if (!DragQueryFile(hdrop, nIndex, &strName.at(0), nBufNeed))
		{
			continue;
		}
		lstPath.push_back(strName);
	}

	DragFinish(hdrop);

	GlobalUnlock(hg);

	if (m_fDrag && !lstPath.empty())
	{
		m_fDrag(lstPath);
	}
	return true;
}

void CPiDropTarget::SetDragFun(std::function<void(const LST_STRING&)> fDrag)
{
	m_fDrag = fDrag;
}

