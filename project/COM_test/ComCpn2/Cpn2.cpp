#pragma once
#include "stdafx.h"
#include <guiddef.h>
#include <unknwn.h>

#include <iostream>
#include "REGISTRY.H"
#include "..\COMDll\interface.h"
using namespace std;

void trace(const char* szMsg){ cout << szMsg << endl; }
long g_cServerLocks;
const char g_szFriendlyName[] = "l_Inside COM Chapter_contain 8";
const char g_szVerIndProgID[] = "l_InsideCOM.Chapter_contain";
const char g_szProgID[] = "l_InsideCOM.Chapter_contain cmpm2";
HMODULE	g_hDll;

interface INonAgentUnknown
{
	virtual HRESULT STDMETHODCALLTYPE NonAgentQueryInterface(const IID& iid, void** ppv) = 0;
	virtual ULONG STDMETHODCALLTYPE NonAgentAddRef(void) = 0;
	virtual ULONG STDMETHODCALLTYPE NonAgentRelease(void) = 0;
};

class CB : public IY
	, public INonAgentUnknown
{
	//IUnknown implement
public:
	CB(IUnknown* pIOuter = nullptr);
	~CB();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& iid, void** ppv);

	virtual ULONG STDMETHODCALLTYPE AddRef(void);;

	virtual ULONG STDMETHODCALLTYPE Release(void);;


	//interface IY implement
	virtual void __stdcall Fy() override
	{
		cout << "Fy" << endl;
	};

	//interface INonAgentUnknown implement
	virtual HRESULT STDMETHODCALLTYPE NonAgentQueryInterface(const IID& iid, void** ppv);
	virtual ULONG STDMETHODCALLTYPE NonAgentAddRef(void);
	virtual ULONG STDMETHODCALLTYPE NonAgentRelease(void);
private:
	unsigned int m_cRef;
	IUnknown*			m_pIOuter;
};

HRESULT __stdcall CB::QueryInterface(const IID& iid, void** ppv)
{
	trace("agent query");
	return m_pIOuter->QueryInterface(iid, ppv);
}

ULONG STDMETHODCALLTYPE CB::AddRef(void)
{
	trace("agent addRef");
	return m_pIOuter->AddRef();
}

ULONG STDMETHODCALLTYPE CB::Release(void)
{
	trace("agent Release");
	return m_pIOuter->Release();
}

HRESULT STDMETHODCALLTYPE CB::NonAgentQueryInterface(const IID& iid, void** ppv)
{
	trace("NonAgent qi");
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<INonAgentUnknown*>(this);
	}
	else if (iid == IID_IY)
	{
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		printf("queryInterface: interface not supported:");
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();	//todo:为何是使用IUnknown接口， 实际是是调用非代理接口？
	return S_OK;
}

ULONG STDMETHODCALLTYPE CB::NonAgentAddRef(void)
{
	trace("NonAgent ar");
	return ++m_cRef;

}

ULONG STDMETHODCALLTYPE CB::NonAgentRelease(void)
{
	trace("NonAgent rl");
	if (--m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

CB::CB(IUnknown* pIOuter /*= nullptr*/)
:m_cRef(1)
{
	if (!pIOuter)
	{
		trace("没有代理， 代理到非代理的IUnknow");
		m_pIOuter = reinterpret_cast<IUnknown*>(
			static_cast<INonAgentUnknown*>(this));
	}
	else
	{
		trace("代理到outer IUnknown");
		m_pIOuter = pIOuter;
	}
}

CB::~CB()
{
	trace("delete CB");
}


class CFactory : public IClassFactory
{
public:
	CFactory() :m_cRef(1){}
	~CFactory(){ trace("ClassFactory Destory "); };
	//IUnknown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& iid, void** ppv);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);;
	virtual ULONG STDMETHODCALLTYPE Release(void);;

	//Interface IClassFactory
	virtual HRESULT __stdcall CreateInstance(IUnknown* pUnknownOwner,
		const IID& iid,
		void** ppv);
	virtual HRESULT __stdcall LockServer(BOOL bLock);


private:
	long		m_cRef;
};

HRESULT STDMETHODCALLTYPE CFactory::QueryInterface(const IID& iid, void** ppv)
{
	if (iid == IID_IUnknown || iid == IID_IClassFactory)
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE CFactory::AddRef(void)
{
	return ++m_cRef;

}

ULONG STDMETHODCALLTYPE CFactory::Release(void)
{
	if (--m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOwner, const IID& iid, void** ppv)
{
	trace("create component");
	if (!(pUnknownOwner && iid == IID_IUnknown))
	{
		return CLASS_E_NOAGGREGATION;
	}
	CB* pB = new CB(pUnknownOwner);
	if (!pB)
	{
		return E_OUTOFMEMORY;
	}
	HRESULT hr = pB->NonAgentQueryInterface(iid, ppv);
	pB->NonAgentRelease();
	return hr;
}

HRESULT __stdcall CFactory::LockServer(BOOL bLock)
{
	if (bLock)
	{
		++g_cServerLocks;
	}
	else
	{
		--g_cServerLocks;
	}
	return S_OK;
}

STDAPI DllCanUnloadNow()
{
	if (g_cServerLocks == 0)
	{
		trace("DllCanUnload");
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv)
{
	trace("classFactory create");
	if (rclsid != CLSID_Component2)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}
	CFactory* pFc = new CFactory;
	if (!pFc)
	{
		return E_OUTOFMEMORY;
	}
	HRESULT hr = pFc->QueryInterface(riid, ppv);
	pFc->Release();
	return hr;
}

STDAPI DllRegisterServer()
{
	return RegisterServer(g_hDll, CLSID_Component2, g_szFriendlyName, g_szVerIndProgID, g_szProgID);
}
STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_Component2, g_szVerIndProgID, g_szProgID);
}


BOOL APIENTRY DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hDll = (HMODULE)hDllHandle;
	}
	return TRUE;
}