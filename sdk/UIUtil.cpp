#include "StdAfx.h"
#include "UIUtil.h"

CUIUtil::CUIUtil(void)
{
}

CUIUtil::~CUIUtil(void)
{
}

tstring CUIUtil::GetOpenDialog()
{
	tstring strPath;

	TCHAR szBuffer[MAX_PATH + 1] = {0};  
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);   
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = _T("apk文件(*.apk)\0*.apk\0所有文件(*.*)\0*.*\0\0");//要选择的文件后缀   
	//ofn.lpstrInitialDir = _T("D:\\Program Files");//默认的文件路径   
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区   
	ofn.nMaxFile = MAX_PATH;   
	ofn.nFilterIndex = 0;   

	if(!GetOpenFileName(&ofn))
	{
		return strPath;
	}
	strPath = ofn.lpstrFile;
	return strPath;
}
