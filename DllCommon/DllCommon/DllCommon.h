////////////////////////////  导出头文件 ///////////////////////////////////////
#include <windows.h>
#include <string>
#include "UI\PiUITool.h"
using namespace std;

#if defined(__cplusplus)
#define NCCOMMON_EXTERN_C extern "C"
#else
#define NCCOMMON_EXTERN_C
#endif

#ifdef DLLCOMMON_EXPORTS
#define NCCOMMON_SYMBOL __declspec(dllexport)
#else
#define NCCOMMON_SYMBOL __declspec(dllimport)
#endif // DLLCOMMON_EXPORTS

#define NCCOMMON_API NCCOMMON_EXTERN_C NCCOMMON_SYMBOL
#define NCCOMMON_CALL _cdecl


typedef CPIUITool::tagSELECT_FILE_DIR tagSELECT_FILE_DIR;

/*
struct tagSELECT_FILE_DIR
{
	HWND			hParent;			//所属窗口， 可以为空
	const wchar_t*	szBeginDir;			//初始显示目录, 可为空
	const wchar_t*	szTitle;			//窗口标题， 可为空
	const wchar_t*	szBtnOkName;		//ok按钮要替换成的名字，  如果为空则显示为"确定"
	bool			bCenterToParent;	//true相对父窗口居中， 否则屏幕居中
	tagSELECT_FILE_DIR()
		:hParent(NULL)
		, szBeginDir(NULL)
		, szTitle(NULL)
		, szBtnOkName(NULL)
		, bCenterToParent(true)
	{

	}
};*/
/************************************************************************
	fun:	弹出选择对话框，  可选择文件或目录
	param:	
	memo:	szSelectPath保存选择的路径， 最大1024字节, 需要外部分配
************************************************************************/
NCCOMMON_API bool NCCOMMON_CALL SelectFileOrDir(tagSELECT_FILE_DIR* pTag, OUT wchar_t* szSelectPath);

