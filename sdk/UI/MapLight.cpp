#include "StdAfx.h"
#include "MapLight.h"

//template <class T>
CMapLight::CMapLight(void)
{
}

//template <class T>
CMapLight::~CMapLight(void)
{

}



bool CMapLight::FindValue(char* key, DWORD* pDW)
{
	CMapLight::iterator it = find(key);
	if (it != this->end())
	{
		*pDW = (*it).second;
		return true;
	}
	return false;
}

bool CMapLight::Insert(const char* key, DWORD dwValue)
{	
	pair<CMapLight::iterator,bool> pRet = insert(CMapLight::value_type(key, dwValue));
	return pRet.second;
}

DWORD CMapLight::GetNewValue(NewValueType type /*= TYPE_MAX_ADD_ONE*/)
{
	DWORD dwValue = 0;
	switch(type)
	{
		case TYPE_MAX_ADD_ONE:
			dwValue = GetNewValueByMaxAdd();
			break;
		case TYPE_EACH_ADD_TOTAL: 
			dwValue = GetNewValueByAddTotal();
			break;
	}
	return dwValue;
}

DWORD CMapLight::GetNewValueByMaxAdd()
{
	
	CMapLight::iterator itEnd = this->end();
	DWORD dwMax = 0;
	for (CMapLight::iterator it = this->begin();it != itEnd;++it)
	{
		dwMax = max(dwMax, (*it).second);
	}
	if (this->size() > 0)
	{
		dwMax += 1;
	}

	return dwMax;
}


DWORD CMapLight::GetNewValueByAddTotal()
{
	CMapLight::iterator itEnd = this->end();
	DWORD dwTotal = 0;
	for (CMapLight::iterator it = this->begin();it != itEnd;++it)
	{
		dwTotal += (*it).second;
	}
	if (this->size() > 0)
	{
		dwTotal += 1;
	}
	return dwTotal;
}