#include "stdafx.h"
#include "renderGDI.h"
#include "renderD3D.h"

CRender* CRender::create(RenderType type)
{
	CRender* p = nullptr;
	switch (type)
	{
	case CRender::GDI_RENDER:
		p = new CRenderGDI;
		break;

	case CRender::D3D_RENDER:
		p = new CRenderD3D;
		break;

	default:
		p = NULL;
		break;
	}
	return p;
}
