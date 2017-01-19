#include "stdAfx.h"
#include "WkeMng.h"
#include "StrCoding.h"
#include "RAIILock.h"
#include "PiNCWke.h"
#include "System\LogSystemDll.h"

CWkeMng::CWkeMng()
{
	Create_Auto_Ptr(m_pLockWkeData, CLock);
	Create_Auto_Ptr(m_pLockJsCB,	CLock);

	wkeInitialize();

}


CWkeMng::~CWkeMng()
{
	wkeFinalize();
}


HWebView CWkeMng::GetWebView(HWND hWnd)
{
	CRAIILock raii(m_pLockWkeData.get());
	if (m_mapWke.find(hWnd) == m_mapWke.end())
	{
		return NULL;
	}
	return m_mapWke[hWnd]->GetWeb();
}

void CWkeMng::Erase(CPiNCWke* pWke)
{
	CRAIILock raii(m_pLockWkeData.get());
	for (auto iter = m_mapWke.begin(); iter != m_mapWke.end(); ++iter)
	{
		if (iter->second == pWke)
		{
			m_mapWke.erase(iter);
			pWke->ClearData();
			delete pWke;
			break;
		}
	}

}

void* CWkeMng::GetJsCallBack(const string& strName)
{
	CRAIILock raii(m_pLockJsCB.get());
	if (!m_mapJsCallBack.size())
	{
		return NULL;
	}
	return m_mapJsCallBack[strName];
}

void* CWkeMng::GetJsFirstBind()
{
	CRAIILock raii(m_pLockJsCB.get());
	if (!m_mapJsCallBack.size())
	{
		return NULL;
	}
	return m_mapJsCallBack.begin()->second;
}


bool CWkeMng::AddJsBind(const wchar_t* szFunc, void* pCallBack)
{	
	CRAIILock raii(m_pLockJsCB.get());
	m_mapJsCallBack[pw2psSmart(szFunc)] = pCallBack;
	return true;
}

CPiNCWke* CWkeMng::CreateWke()
{
	CPiNCWke* pNew = new CPiNCWke;
	//m_lstWke.push_back(pNew);
	return pNew;
}

CPiNCWke* CWkeMng::CreateWke(HWND hParent, tagCallBack* pTagCallBack)
{
	CPiNCWke* pWke = CreateWke();
	if (!pWke)
	{
		LogSystem::WriteLogToFileErrorFormat(_T("create wke on parent %d faild: new Null"), hParent);
		return nullptr;
	}
	if (!pWke->Create(hParent, pTagCallBack))
	{
		return nullptr;
	}
	
	CRAIILock raii(m_pLockWkeData.get());
	m_mapWke[pWke->GetParent()] = pWke;
	return pWke;
}

CPiNCWke* CWkeMng::GetObj(HWebView pWeb)
{
	CRAIILock raii(m_pLockWkeData.get());
	for (auto it : m_mapWke)
	{
		if ((it.second)->GetWeb() == pWeb)
		{
			return it.second;
		}
	}
	return nullptr;
}
