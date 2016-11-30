#include "DllCommon.h"
#include <tchar.h>
#include "UI\PiUITool.h"
//ARR_STRING* g_pFileLst = NULL;	//选择多个文件的列表， 第一个为目录， 第二个开始为文件

NCCOMMON_API int NCCOMMON_CALL SelectFileOrDir(tagSELECT_FILE_DIR* pTag)
{
	tstring  strSelectPath;
	int nSelect = CPIUITool::SelectFileOrDir(pTag, strSelectPath);
	//g_pFileLst = pTag->pFileLst;
	return nSelect;
}

NCCOMMON_API const wchar_t* QuerySelectFile(int nIndex)
{
	return CPIUITool::QuerySelectFile((UINT)nIndex);
	
}

