#pragma once
#include <unknwn.h>
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



//
//IIDs
//

// {8BF9BA93-7B98-49D5-9732-6DA6D1109A50}
static const IID IID_IX =
{ 0x8bf9ba93, 0x7b98, 0x49d5, { 0x97, 0x32, 0x6d, 0xa6, 0xd1, 0x10, 0x9a, 0x50 } };

// {DA751221-D4E8-48F4-879F-F2A25A2FF461}
static const IID IID_IY =
{ 0xda751221, 0xd4e8, 0x48f4, { 0x87, 0x9f, 0xf2, 0xa2, 0x5a, 0x2f, 0xf4, 0x61 } };

// {C67BECE2-4C9E-4575-9E0D-DF24FADBBF6F}
static const IID IID_IZ =
{ 0xc67bece2, 0x4c9e, 0x4575, { 0x9e, 0xd, 0xdf, 0x24, 0xfa, 0xdb, 0xbf, 0x6f } };

// {EE4B12E8-488E-46DD-A23B-F9D36021CB30}
static const CLSID CLSID_Component1 =
{ 0xee4b12e8, 0x488e, 0x46dd, { 0xa2, 0x3b, 0xf9, 0xd3, 0x60, 0x21, 0xcb, 0x30 } };
