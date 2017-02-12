// COM_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "Create.h"
#include "..\COMDll\interface.h"
#include "Ptr.h"
using namespace std;
void trace(const char* szMsg){ cout << szMsg << endl; }
void SpCOM();

int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize(NULL);
	SpCOM();

	::CoUninitialize();
	return 0;
}


void SpCOM()
{
	HRESULT hr = S_FALSE;
	//IUnknown* pIUnknown = CallCreateInstance("COMDll.dll");
	IPtr<IX, &IID_IX> spIX;
	bool bClassFactoryDirect = true;
	if (bClassFactoryDirect)
	{
		IClassFactory* pICF = nullptr;
		hr = CoGetClassObject(CLSID_Component1, CLSCTX_INPROC_SERVER, NULL, IID_IClassFactory, (void**)&pICF);
		pICF->CreateInstance(NULL, IID_IX, (void**)&spIX);
		pICF->Release();
	}
	else
	{
		hr = ::CoCreateInstance(CLSID_Component1, nullptr, CLSCTX_INPROC_SERVER, IID_IX, (void**)&spIX);
	}
	if (spIX)
	{
		trace("client: succeeded getting IX");
		spIX->Fx();
		IPtr<IY, &IID_IY> spIY;
		spIY = spIX;
		//hr = spIX->QueryInterface(IID_IY, (void**)&spIY);//导致多增加一次， 得不到释放？
		if (spIY)
		{
			spIY->Fy();
		}
	}
}
