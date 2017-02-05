#pragma once
#include <unknwnbase.h>
typedef IUnknown* (*FunCreateInstance)();

IUnknown* CallCreateInstance(char* name);