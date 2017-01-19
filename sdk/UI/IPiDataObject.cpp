#include "stdafx.h"
#include "IPiDataObject.h"

IPiDataObjectHelper::IPiDataObjectHelper(IDataObject* pDO)
	:m_pDO(pDO)
{

}


IPiDataObjectHelper::~IPiDataObjectHelper()
{
}


bool IPiDataObjectHelper::IsHasData(UINT cfFormat)
{
	if (!cfFormat)
	{
		return false;
	}
	FORMATETC ft = { 0 };
	STGMEDIUM stg = { 0 };
	ft.cfFormat = cfFormat;
	ft.dwAspect = DVASPECT_CONTENT;
	ft.lindex = -1;
	ft.tymed = -1;
	ft.tymed = TYMED_HGLOBAL /*| TYMED_MFPICT*/;
	int n = 0;
	return S_OK == m_pDO->GetData(&ft, &stg);
}

HGLOBAL IPiDataObjectHelper::GetData(UINT cfFormat)
{
	FORMATETC ft = { 0 };
	STGMEDIUM stg = { 0 };
	ft.cfFormat = cfFormat;
	ft.dwAspect = DVASPECT_CONTENT;
	ft.lindex = -1;
	ft.tymed = -1;
	ft.tymed = TYMED_HGLOBAL /*| TYMED_MFPICT*/;
	int n = 0;
	if (S_OK != m_pDO->GetData(&ft, &stg))
	{
		return NULL;
	}
	
	// handle just hGlobal types
	switch (stg.tymed)
	{
	case TYMED_MFPICT:
	case TYMED_HGLOBAL:
		if (stg.pUnkForRelease == NULL)
		{
			return stg.hGlobal;
		}

		// default -- falls through to error condition...
	default:
		break;
	}

	::ReleaseStgMedium(&stg);
	return NULL;
}
