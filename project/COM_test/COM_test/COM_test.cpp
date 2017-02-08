// COM_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "Create.h"
#include "..\COMDll\interface.h"
using namespace std;
void trace(const char* szMsg){ cout << szMsg << endl; }

int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize(NULL);
	HRESULT hr = S_FALSE;
	//IUnknown* pIUnknown = CallCreateInstance("COMDll.dll");
	IX* pIX = nullptr;
	bool bClassFactoryDirect = true;
	if (bClassFactoryDirect)
	{
		IClassFactory* pICF = nullptr;
		hr = CoGetClassObject(CLSID_Component1, CLSCTX_INPROC_SERVER, NULL, IID_IClassFactory, (void**)&pICF);
		pICF->CreateInstance(NULL, IID_IX, (void**)&pIX);
		pICF->Release();
	}
	else
	{
		hr = ::CoCreateInstance(CLSID_Component1, nullptr, CLSCTX_INPROC_SERVER, IID_IX, (void**)&pIX);
	}
	if (SUCCEEDED(hr))
	{
		trace("client: succeeded getting IX");
		pIX->Fx();

		IY* pIY = nullptr;
		hr = pIX->QueryInterface(IID_IY, (void**)&pIY);
		if (SUCCEEDED(hr))
		{
			trace("client: succeeded getting IY");
			pIY->Fy();
			pIY->Release();
			trace("client: release IY");
		}
		pIX->Release();
		trace("client: release IX");
	}
	::CoUninitialize();
	return 0;
}

