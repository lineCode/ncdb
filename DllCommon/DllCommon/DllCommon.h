////////////////////////////  导出头文件 ///////////////////////////////////////
#include <windows.h>
#include <string>
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
#endif // NC_DLL_COMMON_BUILDING

#define NCCOMMON_API NCCOMMON_EXTERN_C NCCOMMON_SYMBOL
#define NCCOMMON_CALL _cdecl


struct tagSELECT_FILE_DIR
{
	HWND			hParent;		//显示时相对父窗口居中;  如果为空, 则显示窗口时相对屏幕居中
	const wchar_t*	szBeginDir;		//初始显示目录, 可为空
	const wchar_t*	szTitle;		//窗口标题， 可为空
	const wchar_t*	szBtnOkName;	//ok按钮要替换成的名字，  如果为空则显示为"确定"

	tagSELECT_FILE_DIR()
		:hParent(NULL)
		, szBeginDir(NULL)
		, szTitle(NULL)
		, szBtnOkName(NULL)
	{

	}
};

/************************************************************************
	fun:	弹出选择对话框，  可选择文件或目录
	param:	szBeginDir, 弹窗后显示的起始目录， 如果目录不存在则显示上次的目录
	memo:	szSelectPath保存选择的路径， 最大1024字节, 需要外部分配
************************************************************************/
NCCOMMON_API void NCCOMMON_CALL SelectFileOrDir(tagSELECT_FILE_DIR* pTag, OUT wchar_t* szSelectPath);

