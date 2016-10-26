#include "StdAfx.h"
#include "define.h"
#include "ImageMap.h"
#include "MapLight.h"
#include "FileStrOp.h"

CImageMap::CImageMap(void)
{
	m_pIL	= NULL;
	m_pMap	= NULL;
}

CImageMap::~CImageMap(void)
{
	if(m_pIL )
	{
		delete m_pIL;
	}
	if (m_pMap)
	{
		delete m_pMap;
	}
}


bool CImageMap::Init()
{
	m_pIL	= new CImageList;
	m_pMap	= new CMapLight;

	if(!m_pIL->Create(16, 16, ILC_COLOR32, 0, 1))
	{
		DEBUG_ERROR(L"create CImageList fail");
		return false;
	}

	return true;
}


ilMap::iterator CImageMap::Find(const char* pSuffix)
{
	return m_pMap->find(ilMap::key_type(pSuffix));
}

bool CImageMap::Insert(const char* key, DWORD idxImage )
{
	pair<ilMap::iterator,bool> pRet = m_pMap->insert(ilMap::value_type(key , idxImage));
	if(pRet.second)
	{
		SHFILEINFOA fi = CFileStrOp::GetFileIcon(key);
		if(-1 == m_pIL->Add((HICON)fi.hIcon))
		{
			DEBUG_ERROR(L"add icon to CImageList fail");
			return false;
		}
		return true;
	}
	return false;
}

DWORD CImageMap::GetIndex(const char* pSuffix)
{
	CMapLight::iterator it = m_pMap->find(ilMap::key_type(pSuffix));

	if (m_pMap->end() == it )
	{
		DWORD dwNew = GetNewIndex();
		Insert(pSuffix, dwNew);
		return dwNew;
	}
	return it->second;
}



/************************************************************************/
/*	fun: 算出新的位置,即新插入数据项的索引位置                                                                     */
/************************************************************************/
DWORD CImageMap::GetNewIndex()
{
	return m_pMap->GetNewValueByMaxAdd();
}


CImageList* CImageMap::GetImageList()
{
	return m_pIL;
}

void CImageMap::SetImageList(CImageList* pIL)
{
	m_pIL = pIL;
}