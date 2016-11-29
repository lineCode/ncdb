#include "DllCommon.h"
#include <tchar.h>
#include "UI\PiUITool.h"


NCCOMMON_API bool NCCOMMON_CALL SelectFileOrDir(tagSELECT_FILE_DIR* pTag, OUT wchar_t* szSelectPath)
{
	tstring  strSelectPath;
	bool bRet = false;
	if (bRet = CPIUITool::SelectFileOrDir(pTag, strSelectPath))
	{
		_tcscpy_s(szSelectPath, 1024, strSelectPath.c_str());
	}
	return bRet;
}

