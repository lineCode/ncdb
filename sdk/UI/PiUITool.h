#pragma once
#include "..\PiTypeDef.h"
#include <Windows.h>
using std::string;




class CPIUITool
{
public:
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
	};
	/************************************************************************/
	/*	fun: 获取一个拖放文件路径, 窗口需要支持拖放:通过DragAcceptFiles设置
	/************************************************************************/
	static tstring GetOneDragFilePath(const HDROP& hd);
	static tstring SelectDirectory(HWND hParent, tcpchar szBeginDir, tcpchar szTitle);
	/************************************************************************
		fun:	弹出选择对话框，  可选择文件或目录
		param:	szBeginDir, 弹窗后显示的起始目录， 如果目录不存在则显示上次的目录
		memo:	
	************************************************************************/
	//static tstring SelectFileOrDir(HWND hParent, tcpchar szBeginDir, tcpchar szTitle, tcpchar szBtnOkName);
	static bool SelectFileOrDir(tagSELECT_FILE_DIR* pTag, tstring& strPath);
};
