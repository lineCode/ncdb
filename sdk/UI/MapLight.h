#pragma once
#include <map>
#include <string>
using namespace std;



class CMapLight
	:public map<string, DWORD >
{
public:
	enum NewValueType{TYPE_MAX_ADD_ONE, TYPE_EACH_ADD_TOTAL};
	CMapLight(void);
	~CMapLight(void);
	
	bool FindValue(char* key, DWORD*);
	bool Insert(const char* key, DWORD dwValue);
	DWORD GetNewValue(NewValueType = TYPE_MAX_ADD_ONE);
	DWORD GetNewValueByAddTotal();
	DWORD GetNewValueByMaxAdd();
};
