#pragma once
#include "EFDefine.h"
class IPlugTask
{
public:
	//IPlugTask();
	//~IPlugTask();
public:
	virtual bool DoInit(FileList* pfl) = 0;
	virtual bool DoWork() = 0;
	virtual bool DoUnInit()= 0;
};