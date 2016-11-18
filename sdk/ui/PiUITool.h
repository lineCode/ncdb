#pragma once
#include "..\PiTypeDef.h"
using std::string;
/************************************************************************/
/*	fun: 获取一个拖放文件路径, 窗口需要支持拖放:通过DragAcceptFiles设置
/************************************************************************/
class CPIUITool
{
public:
	static tstring GetOneDragFilePath(const HDROP& hd);
	static tstring SelectDirectory(HWND hParent, tcpchar szBeginDir, tcpchar szTitle);
	/************************************************************************
		fun:	弹出选择对话框，  可选择文件或目录
		param:	szBeginDir, 弹窗后显示的起始目录， 如果目录不存在则显示上次的目录
		memo:	
	************************************************************************/
	static tstring SelectFileOrDir(HWND hParent, tcpchar szBeginDir, tcpchar szTitle, tcpchar szBtnOkName);
};
