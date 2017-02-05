#include "stdafx.h"
#include "Create.h"

IUnknown* CallCreateInstance(char* name)
{
	HINSTANCE hInst = ::LoadLibraryA(name);
	if (!hInst)
	{
		return nullptr;
	}
	FunCreateInstance pFun = (FunCreateInstance)::GetProcAddress(hInst, "CreateInstance");
	if (!pFun)
	{
		return nullptr;
	}
	return pFun();
}
