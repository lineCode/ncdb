#pragma once
#include <unknwn.h>
#include "..\COMDll\COMGUID.h"
//#define interface struct
extern const IID IID_IX;
extern const IID IID_IY;
extern const IID IID_IZ;


interface IX:IUnknown
{
	virtual void __stdcall Fx() = 0;
};

interface IY :IUnknown
{
	virtual void __stdcall Fy() = 0;

};

interface IZ :IUnknown
{
	virtual void __stdcall Fz() = 0;
};


extern "C"  IUnknown* CreateInstance();

