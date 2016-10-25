#pragma once
/************************************************************************
	默认为静态库, 动态库需要定义STAT_DLL_DYNAMIC 宏
************************************************************************/
#ifdef DLL_SELF
	#define DLL_CLASS _declspec(dllexport)
#else 
#ifdef STAT_DLL_DYNAMIC
	#define DLL_CLASS _declspec(dllimport)
#else
	#define DLL_CLASS 
#endif

#ifdef _DEBUG
	// _DEBUG
	#ifdef _UNICODE
	#pragma comment(lib, "BYStat_d.lib")
	#else 
	#pragma comment(lib, "BYStat_da.lib")
	#endif
#else
	//release
	#ifdef _UNICODE
	#pragma comment(lib, "BYStat.lib")
	#else 
	#pragma comment(lib, "BYStat_a.lib")
	#endif
#endif 

#endif