// COM_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "Create.h"
#include "..\COMDll\interface.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_FALSE;
	IUnknown* pIUnknown = CallCreateInstance("COMDll.dll");
	cout << "get a IUnknown pointer" << endl;
	IX* pIX = nullptr;
	hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);
	if (SUCCEEDED(hr))
	{
		cout << "client: succeeded getting IX"<< endl;
		pIX->Fx();
		pIX->Release();
	}
	hr = pIUnknown->Release();
	return 0;
}

