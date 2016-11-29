#include "DllCommon.h"
#include <tchar.h>
#include "UI\PiUITool.h"


NCCOMMON_API void NCCOMMON_CALL SelectFileOrDir(tagSELECT_FILE_DIR* pTag, OUT wchar_t* szSelectPath)
{
	tstring strSelectPath = CPIUITool::SelectFileOrDir(pTag);
	_tcscpy_s(szSelectPath, 1024, strSelectPath.c_str());
}
