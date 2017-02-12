#pragma once
#include "stdafx.h"
#include <guiddef.h>
#include "interface.h"
#include <iostream>
#include "REGISTRY.H"
using namespace std;

void trace(const char* szMsg){ cout << szMsg << endl; }
long g_cServerLocks;
const char g_szFriendlyName[] = "l_Inside COM Chapter 7";
const char g_szVerIndProgID[] = "l_InsideCOM.Chapter";
const char g_szProgID[] = "l_InsideCOM.Chapter 07";
HMODULE	g_hDll;

class CA : public IX
	//, public IY
{
	//IUnknown implement
public:
	CA();
	~CA();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& iid, void** ppv);

	virtual ULONG STDMETHODCALLTYPE AddRef(void);;

	virtual ULONG STDMETHODCALLTYPE Release(void);;

	//interface IX implement
	HRESULT __stdcall Init();
	virtual void __stdcall Fx()
	{
		cout << "Fx" << endl; 
	};

	//interface IY implement
	/*virtual void __stdcall Fy()
	{
		cout << "Fy" << endl;
	};*/
private:
	unsigned int		m_cRef;
	IUnknown*			m_pUnknownInner;
	IY*					m_pIY;		//todo:为何需要pIY
};

HRESULT __stdcall CA::QueryInterface(const IID& iid, void** ppv)
{
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this);
	}
	else if (iid == IID_IY && m_pUnknownInner)
	{
		return m_pUnknownInner->QueryInterface(iid, ppv);	//todo:查询， 内部组件不是会调用到外部组件？
	}
	else
	{
		trace("queryInterface: interface not supported:");
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;

}

ULONG STDMETHODCALLTYPE CA::AddRef(void)
{
	return ++m_cRef;
}

ULONG STDMETHODCALLTYPE CA::Release(void)
{
	if (--m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}


CA::CA()
:m_cRef(1)
, m_pUnknownInner(nullptr)
{
	trace("construct CA");

}


CA::~CA()
{
	trace("delete CA");
	m_cRef = 1;
	IUnknown* pUnknownOuter = this;
	pUnknownOuter->AddRef();
	m_pIY->Release();

	m_pUnknownInner->Release();
}

HRESULT __stdcall CA::Init()
{
	IUnknown* pIOuter = static_cast<IX*>(this);
	HRESULT hr = ::CoCreateInstance(CLSID_Component2, pIOuter, CLSCTX_INPROC_SERVER
		, IID_IUnknown, (void**)&m_pUnknownInner);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	hr = m_pUnknownInner->QueryInterface(IID_IY, (void**)&m_pIY);	//这里实际上是非代理的接口的queryInterface查询？
	if (FAILED(hr))
	{
		m_pUnknownInner->Release();
		return E_FAIL;
	}
	pIOuter->Release();
	return S_OK;
}

 
class CFactory : public IClassFactory
{
public:
	CFactory() :m_cRef(1){}
	~CFactory(){trace("ClassFactory Destory "); };
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
	if (pUnknownOwner)
	{
		return CLASS_E_NOAGGREGATION;
	}
	CA* pA = new CA;
	if (!pA)
	{
		return E_OUTOFMEMORY;
	}
	HRESULT hr = pA->Init();
	if (FAILED(hr))
	{
		pA->Release();
		return hr;
	}
	hr = pA->QueryInterface(iid, ppv);
	pA->Release();
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
	if (rclsid != CLSID_Component1)
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
	return RegisterServer(g_hDll, CLSID_Component1, g_szFriendlyName, g_szVerIndProgID, g_szProgID);
}
STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_Component1, g_szVerIndProgID, g_szProgID);
}

IUnknown* CreateInstance()
{
	IUnknown* pI = static_cast<IX*>(new CA);
	pI->AddRef();
	return pI;
}

BOOL APIENTRY DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hDll = (HMODULE)hDllHandle;
	}
	return TRUE;
}