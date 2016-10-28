#pragma once
#include "PiWkeDef.h"
#include <string>
using std::wstring;

class CWkeCommon
{
public:
	CWkeCommon();
	~CWkeCommon();
public:
	static wstring GetString(HNCwkeWebView pWeb, wkeJSValue jsV);
};

