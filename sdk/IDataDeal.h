#pragma once

class IDataDeal
{
public:
	virtual bool Init(void*)			= 0;
	virtual bool Work(void*)			= 0;
	virtual void SetData(void* pData)	= 0;
	virtual IDataDeal* Clone()			= 0;
};